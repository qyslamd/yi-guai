#include "screenshoter.h"
#include <cmath>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QClipboard>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QShortcut>
#include <QDateTime>
#include <QMimeData>
#include "circlecursor.h"
#include "logging.h"

ScreenShoter::ScreenShoter(QWidget *parent)
    : Selector(parent)
{
    menu_ = new ImageEditMenu(this);
    magnifier_ = new Magnifier(this);
    canvas_ = QPixmap(size());

    connect(menu_, &ImageEditMenu::save, this, &ScreenShoter::save);
    connect(menu_, &ImageEditMenu::ok, this, &ScreenShoter::copy);
    connect(menu_, &ImageEditMenu::fix,  this, &ScreenShoter::pin);
    connect(menu_, &ImageEditMenu::exit, this, &ScreenShoter::exit);

    connect(menu_, &ImageEditMenu::undo, this, &ScreenShoter::undo);
    connect(menu_, &ImageEditMenu::redo, this, &ScreenShoter::redo);

    connect(menu_, &ImageEditMenu::paint, [this](Graph graph) {
        switch (graph) {
        case Graph::NONE:
            edit_status_ = EditStatus::NONE;
            if(commands_.empty())
                CAPTURED();
            focusOn(nullptr);
            break;

        default:
            edit_status_ = (edit_status_ & 0xffff0000) | graph;
            LOCKED();
            focusOn((focus_cmd_ && focus_cmd_->graph() == graph) ? focus_cmd_ : nullptr);
            break;
        }
    });

    connect(this, &ScreenShoter::focusOnGraph, menu_, &ImageEditMenu::paintGraph);

    // attrs changed
    connect(menu_, &ImageEditMenu::changed, [this](Graph graph){
        if(focus_cmd_ && focus_cmd_->graph() == graph) {
            switch (graph) {
            case Graph::ERASER:
            case Graph::MOSAIC: break;
            default:
                focus_cmd_->pen(menu_->pen(graph));
                focus_cmd_->setFill(menu_->fill(graph));
                break;
            }
        }

        // change the cursor
        if(graph == Graph::ERASER || graph == Graph::MOSAIC) {
            circle_cursor_.setWidth(menu_->pen(graph).width());
            setCursor(QCursor(circle_cursor_.cursor()));
        }

        if(focus_cmd_ && focus_cmd_->graph() == graph && graph == Graph::TEXT) {
            focus_cmd_->font(menu_->font(Graph::TEXT));
        }
    });

    // repaint when stack changed
    connect(&commands_, &CommandStack::increased, [this]() { modified(PaintType::DRAW_FINISHED); });
    connect(&commands_, &CommandStack::decreased, [this]() { modified(PaintType::REPAINT_ALL); });
    connect(&commands_, &CommandStack::emptied, [this](bool empty) {
        if(empty) CAPTURED(); else LOCKED();
    });

    // disable redo/undo
    connect(&commands_, &CommandStack::emptied, menu_, &ImageEditMenu::disableUndo);
    connect(&redo_stack_, &CommandStack::emptied, menu_, &ImageEditMenu::disableRedo);

    // show menu
    connect(this, &ScreenShoter::captured, [this](){ /*LOG(INFO_MSG) << "captured";*/ menu_->show(); moveMenu(); });

    // reset menu
    connect(&commands_, &CommandStack::emptied, [this](bool empty) { if(empty) menu_->reset(); });

    // move menu
    connect(this, &ScreenShoter::moved, this, &ScreenShoter::moveMenu);
    connect(this, &ScreenShoter::resized, this, &ScreenShoter::moveMenu);

    // shortcuts
    registerShortcuts();
}

void ScreenShoter::start()
{
    if(status_ != INITIAL) return;

    auto virtual_geometry = QGuiApplication::primaryScreen()->virtualGeometry();
    captured_screen_ = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId(),
                                                                    virtual_geometry.left(),
                                                                    virtual_geometry.top(),
                                                                    virtual_geometry.width(),
                                                                    virtual_geometry.height());
    modified(PaintType::REPAINT_ALL);

    Selector::start();
}

void ScreenShoter::focusOn(shared_ptr<PaintCommand> cmd)
{
    if(focus_cmd_ == cmd) return;

    auto previous_focus_cmd = focus_cmd_;
    if(previous_focus_cmd) {
        previous_focus_cmd->setFocus(false);
        if(!previous_focus_cmd->isValid()) {
            commands_.remove(previous_focus_cmd);
        }
    }

    // switch
    focus_cmd_ = cmd;

    if(focus_cmd_) {
        focus_cmd_->setFocus(true);
        menu_->style(focus_cmd_->graph(), focus_cmd_->pen(), focus_cmd_->isFill());
    }

    if(!previous_focus_cmd || (focus_cmd_ && previous_focus_cmd->graph() != focus_cmd_->graph())) {
        emit focusOnGraph(focus_cmd_->graph());  // must after focus_cmd_ = cmd
    }
}

#define COPY_AND_MODIFY_CMD(CMD)                         \
        do {                                             \
            auto pre_cmd = CMD;                          \
            pre_cmd->visible(false);                     \
            CMD = make_shared<PaintCommand>(*pre_cmd);   \
            CMD->previous(pre_cmd);                      \
        } while(0)

void ScreenShoter::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    auto mouse_pos = event->pos();

    if(status_ == LOCKED) {
        // Border => move
        if((hover_position_ & Resizer::BORDER) && hover_cmd_) {
            move_begin_ = mouse_pos;

            COPY_AND_MODIFY_CMD(hover_cmd_);

            edit_status_ |= EditStatus::GRAPH_MOVING;
        }
        // Anchor => resize
        else if((hover_position_ & Resizer::ANCHOR) && hover_cmd_) {
            resize_begin_ = mouse_pos;

            COPY_AND_MODIFY_CMD(hover_cmd_);

            edit_status_ |= GRAPH_RESIZING;
        }
        // rotate
        else if((hover_position_ == Resizer::ROTATE_ANCHOR) && hover_cmd_) {
            COPY_AND_MODIFY_CMD(hover_cmd_);

            edit_status_ |= GRAPH_ROTATING;
        }

        // Not borders and anchors => Create
        else {
            auto graph = Graph(edit_status_ & GRAPH_MASK);
            auto pen = menu_->pen(graph);
            auto fill = menu_->fill(graph);

            switch (graph) {
            case Graph::RECTANGLE:
            case Graph::CIRCLE:
            case Graph::ARROW:
            case Graph::LINE:
            case Graph::CURVES:
                hover_cmd_ = make_shared<PaintCommand>(graph, pen, fill, event->pos());
                break;

            case Graph::MOSAIC:
                pen.setBrush(QBrush(mosaic(canvas_.toImage())));
                hover_cmd_ = make_shared<PaintCommand>(graph, pen, false, event->pos());
                break;

            case Graph::TEXT:
                hover_cmd_ = make_shared<PaintCommand>(graph, pen, false, event->pos(), this);
                hover_cmd_->font(menu_->font(Graph::TEXT));
                break;

            case Graph::ERASER:
                pen.setBrush(QBrush(captured_screen_));
                hover_cmd_ = make_shared<PaintCommand>(graph, pen, false, event->pos());
                break;

            default: CAP_LOG(ERROR_MSG) << "Error type:" << graph; break;
            }
            edit_status_ |= GRAPH_CREATING;
        }

        if(hover_cmd_) {
            hover_cmd_->visible(true);
            focusOn(hover_cmd_);
            modified(PaintType::DRAW_MODIFING);
            connect(hover_cmd_.get(), &PaintCommand::modified, this, &ScreenShoter::modified);
        }
    }

    Selector::mousePressEvent(event);
}
#undef COPY_AND_MODIFY_CMD

void ScreenShoter::mouseMoveEvent(QMouseEvent* event)
{
    auto mouse_pos = event->pos();

    if(event->buttons() & Qt::LeftButton) {
        if(status_ == LOCKED) {
            switch (edit_status_ & OPERATION_MASK) {
            case GRAPH_CREATING:
                hover_cmd_->push_point(mouse_pos);
                break;

            case GRAPH_MOVING:
                hover_cmd_->move(mouse_pos - move_begin_);
                move_begin_ = mouse_pos;
                break;

            case GRAPH_RESIZING:
                hover_cmd_->resize(hover_position_, mouse_pos - resize_begin_);
                resize_begin_ = mouse_pos;
                break;

            default:  break;
            }
        }
        else if(status_ == MOVING) {
            redo_stack_.clear();
            commands_.clear();
        }
    }

    // setCursor
    if(edit_status_ & Graph::ERASER || edit_status_ & Graph::MOSAIC) {
        circle_cursor_.setWidth(menu_->pen(Graph(edit_status_ & GRAPH_MASK)).width());
        setCursor(QCursor(circle_cursor_.cursor()));
    }
    else if(event->buttons() == Qt::NoButton && status_ == LOCKED) {
        updateHoverPos(event->pos());
        setCursorByHoverPos(hover_position_);
    }

    moveMagnifier();
    Selector::mouseMoveEvent(event);
}

void ScreenShoter::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && status_ == LOCKED) {
        if(!focus_cmd_) CAP_LOG(ERROR_MSG) << "Focused cmd is nullptr!";

        if((!focus_cmd_->previous() && focus_cmd_->isValid())           // created
                || (focus_cmd_->previous() && focus_cmd_->adjusted())   // moved/resized..
                || ( (edit_status_ & GRAPH_MASK) == Graph::TEXT)) {     // check text is valid when it loses focus
            commands_.push(focus_cmd_);
            redo_stack_.clear();
        }
        else {
            if(focus_cmd_->previous()) {
                focus_cmd_->previous()->visible(true);
            }
            // foucs on null or the previous cmd
            focusOn(focus_cmd_->previous());
        }

        edit_status_ &= ~OPERATION_MASK;
    }

    Selector::mouseReleaseEvent(event);
}

void ScreenShoter::wheelEvent(QWheelEvent * event)
{
    if((edit_status_ & Graph::ERASER) || (edit_status_ & Graph::MOSAIC)) {
        auto delta = event->delta() / 120;
        auto graph = static_cast<Graph>(edit_status_ & GRAPH_MASK);
        auto pen = menu_->pen(graph);

        pen.setWidth(std::min(pen.width() + delta, 49));
        menu_->pen(graph, pen);
    }
}

void ScreenShoter::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && status_ >= CAPTURED) {
        snipped();

        exit();
    }

    Selector::mouseDoubleClickEvent(event);
}

void ScreenShoter::updateHoverPos(const QPoint& pos)
{
    hover_position_ = Resizer::DEFAULT;
    hover_cmd_ = shared_ptr<PaintCommand>(nullptr);

    for(auto& command: commands_.commands()) {
        if(!command->visible()) continue;
        switch (command->graph()) {
        case Graph::RECTANGLE:
        {
            hover_position_ = command->resizer().absolutePos(pos);

            if(hover_position_ & Resizer::ADJUST_AREA) {
                hover_cmd_ = command;
                return;
            }
            break;
        }

        case Graph::CIRCLE:
        {
            auto resizer = command->resizer();

            QRegion r1(resizer.rect().adjusted(-2, -2, 2, 2), QRegion::Ellipse);
            QRegion r2(resizer.rect().adjusted(2, 2, -2, -2), QRegion::Ellipse);

            if(r1.contains(pos) && !r2.contains(pos)) {
                hover_position_ = Resizer::BORDER;
                hover_cmd_ = command;
                return;
            }
            else {
                hover_position_ = resizer.absolutePos(pos);
                if(hover_position_ & Resizer::ANCHOR) {
                    hover_cmd_ = command;
                    return;
                }
            }
            break;
        }

        case Graph::ARROW:
        case Graph::LINE:
        {
            auto x1y1_anchor = command->resizer().X1Y1Anchor();
            auto x2y2_anchor = command->resizer().X2Y2Anchor();
            QPolygon polygon;
            polygon.push_back(x1y1_anchor.topLeft());
            polygon.push_back(x1y1_anchor.bottomRight());
            polygon.push_back(x2y2_anchor.bottomRight());
            polygon.push_back(x2y2_anchor.topLeft());


            if(x1y1_anchor.contains(pos)) {
                hover_position_ = Resizer::X1Y1_ANCHOR;
                hover_cmd_ = command;
                return;
            }
            else if(x2y2_anchor.contains(pos)) {
                hover_position_ = Resizer::X2Y2_ANCHOR;
                hover_cmd_ = command;
                return;
            }
            else if(polygon.containsPoint(pos, Qt::OddEvenFill)) {
                hover_position_ = Resizer::BORDER;
                hover_cmd_ = command;
                return;
            }
            break;
        }

        case Graph::TEXT:
        {
            Resizer resizer(command->geometry().adjusted(-5, -5, 5, 5));
            resizer.enableRotate(true);
            hover_position_ = resizer.absolutePos(pos);
            switch (hover_position_) {
            case Resizer::INSIDE:
            case Resizer::X1_ANCHOR:
            case Resizer::Y2_ANCHOR:
            case Resizer::Y1_ANCHOR:
            case Resizer::X2_ANCHOR: hover_position_ = Resizer::BORDER; break;
            default: break;
            }

            if(hover_position_ & Resizer::ADJUST_AREA || hover_position_ == Resizer::ROTATE_ANCHOR) {
                hover_cmd_ = command;
                return;
            }
            break;
        }

        default: break;
        }
    }
}

void ScreenShoter::setCursorByHoverPos(Resizer::PointPosition pos, const QCursor & default_cursor)
{
    switch (pos) {
    case Resizer::X1_ANCHOR:    setCursor(Qt::SizeHorCursor); break;
    case Resizer::X2_ANCHOR:    setCursor(Qt::SizeHorCursor); break;
    case Resizer::Y1_ANCHOR:    setCursor(Qt::SizeVerCursor); break;
    case Resizer::Y2_ANCHOR:    setCursor(Qt::SizeVerCursor); break;
    case Resizer::X1Y1_ANCHOR:  setCursor(Qt::SizeFDiagCursor); break;
    case Resizer::X1Y2_ANCHOR:  setCursor(Qt::SizeBDiagCursor); break;
    case Resizer::X2Y1_ANCHOR:  setCursor(Qt::SizeBDiagCursor); break;
    case Resizer::X2Y2_ANCHOR:  setCursor(Qt::SizeFDiagCursor); break;
    case Resizer::BORDER:
    case Resizer::X1_BORDER:
    case Resizer::X2_BORDER:
    case Resizer::Y1_BORDER:
    case Resizer::Y2_BORDER:    setCursor(Qt::SizeAllCursor); break;
    case Resizer::ROTATE_ANCHOR: setCursor(QPixmap(":/icon/res/rotate").scaled(22, 22, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)); break;

    default: setCursor(default_cursor); break;
    }
}

void ScreenShoter::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && status_ >= CAPTURED) {
        copy();
        exit();
    }

    if(event->key() == Qt::Key_Escape) {
        exit();
    }

    Selector::keyPressEvent(event);
}

void ScreenShoter::moveMenu()
{
    auto area = selected();
    auto right = area.right() - menu_->width() + 1;
    if(right < 0) right = 0;

    if(area.bottom() + (menu_->height() * 2 + 5/*margin*/) < rect().height()) {
        menu_->move(right, area.bottom() + 3);
        menu_->setSubMenuShowBelow();
    }
    else if(area.top() - (menu_->height() * 2 + 5) > 0) {
        menu_->move(right, area.top() - menu_->height() - 3);
        menu_->setSubMenuShowAbove();
    }
    else {
        menu_->move(right, area.bottom() - menu_->height() - 3);
        menu_->setSubMenuShowAbove();
    }
}

void ScreenShoter::moveMagnifier()
{
    if(status_ < CAPTURED || status_ == RESIZING) {
        magnifier_->pixmap(captured_screen_.copy(magnifier_->mrect()));
        magnifier_->show();
        magnifier_->move(QCursor::pos().x() + 10, QCursor::pos().y() + 10);
    } else {
        magnifier_->hide();
    }
}

void ScreenShoter::exit()
{
    commands_.clear();
    redo_stack_.clear();

    menu_->reset();
    menu_->hide();
    text_edit_ = nullptr;
    focus_cmd_ = nullptr;

    Selector::exit();
}

QImage ScreenShoter::mosaic(const QImage& _image)
{
    return _image.copy()
            .scaled(_image.width() / 9, _image.height() / 9, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
            .scaled(_image.width(), _image.height());
}

void ScreenShoter::updateCanvas()
{
    painter_.begin(&canvas_);

    switch (modified_) {
    case PaintType::DRAW_FINISHED:
        if(focus_cmd_ == nullptr) {
            CAP_LOG(ERROR_MSG) << "nullptr";
            return;
        }

        focus_cmd_->repaint(&painter_);
        break;

    case PaintType::DRAW_MODIFIED:
        if(focus_cmd_ == nullptr) {
            CAP_LOG(ERROR_MSG) << "nullptr";
            return;
        }

        focus_cmd_->draw_modified(&painter_);
        break;

    case PaintType::REPAINT_ALL:
        painter_.drawPixmap(0, 0, captured_screen_);
        for(auto& command: commands_.commands()) {
            command->repaint(&painter_);
        }
        break;

    default: break;
    }
    painter_.end();
}

void ScreenShoter::paintEvent(QPaintEvent *event)
{
    // 1. canvas
    updateCanvas();

    // 2. window
    painter_.begin(this);
    painter_.drawPixmap(0, 0, canvas_);

    if(modified_ == PaintType::DRAW_MODIFING || modified_ == PaintType::REPAINT_ALL) {
        if(focus_cmd_) {
            focus_cmd_->draw_modified(&painter_);
        }
    }

    //
    if(focus_cmd_ != nullptr) {
        painter_.save();
        switch (focus_cmd_->graph()) {
        case Graph::RECTANGLE:
        case Graph::ELLIPSE:
            drawSelector(&painter_, focus_cmd_->resizer());
            break;

        case Graph::LINE:
        case Graph::ARROW:
            painter_.setPen(QPen(Qt::black, 1, Qt::DashLine));
            painter_.setRenderHint(QPainter::Antialiasing, true);
            painter_.drawLine(focus_cmd_->resizer().point1(), focus_cmd_->resizer().point2());

            painter_.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            painter_.drawRect(focus_cmd_->resizer().X1Y1Anchor());
            painter_.drawRect(focus_cmd_->resizer().X2Y2Anchor());
            break;

        case Graph::TEXT:
        {
            Resizer resizer(focus_cmd_->geometry().adjusted(-5, -5, 5, 5));

            painter_.save();
            painter_.setPen(QPen(QColor("#333"), 2, Qt::SolidLine));
            painter_.setRenderHint(QPainter::Antialiasing, true);
            painter_.setBrush(Qt::white);
            painter_.drawEllipse(resizer.rotateAnchor());
            painter_.restore();

            // box border
            painter_.setPen(QPen(Qt::black, 1, Qt::DashLine));
            painter_.drawRect(resizer.rect());

            // anchors
            painter_.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            painter_.drawRects(resizer.cornerAnchors());
            break;
        }

        default: break;
        }
        painter_.restore();
    }
    painter_.end();

    // clear the modified flag
    modified_ = PaintType::UNMODIFIED;

    Selector::paintEvent(event);
}

QPixmap ScreenShoter::snipped()
{
    auto mimedata = new QMimeData();
    auto position = selected().topLeft();
    auto&& image = snippedImage();
    mimedata->setData("application/qpoint", QByteArray().append(reinterpret_cast<char*>(&position), sizeof (QPoint)));
    mimedata->setImageData(QVariant(image));
    QApplication::clipboard()->setMimeData(mimedata);

    history_.push_back({captured_screen_, selected(), commands_});
    history_idx_ = history_.size() - 1;

    return image;
}

QPixmap ScreenShoter::snippedImage()
{
    return canvas_.copy(selected());
}

void ScreenShoter::save()
{
    QString save_path = CaptureConfig::instance()["savepath"].get<QString>();
    if(save_path.isEmpty()){
        save_path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
        CaptureConfig::instance().set(CaptureConfig::instance()["savepath"], save_path);
    }

    QDateTime dateTime;
    // Qt's Bug in Qt5.8.0 on windows, can't find 'QDateTime::currentDateTime()'
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    dateTime = QDateTime::currentDateTime();
#else
    dateTime.setDate(QDate::currentDate());
    dateTime.setTime(QTime::currentTime());
#endif

    QString default_filename = "Capturer_picture_" + dateTime.toString("yyyyMMdd_hhmmss_zzz") + ".png";
#ifdef _WIN32
    auto filename = QFileDialog::getSaveFileName(this,
                                                 tr("Save Image"),
                                                 save_path + QDir::separator() + default_filename,
                                                 "PNG(*.png);;JPEG(*.jpg *.jpeg);;BMP(*.bmp)");

    if(!filename.isEmpty()) {
        snippedImage().save(filename);
        emit SHOW_MESSAGE("Capturer<PICTURE>", "Path: " + filename);

        snipped();

        exit();
    }
#elif __linux__
    auto filename = QDir(save_path).absoluteFilePath(default_filename);

    snippedImage().save(filename);
    emit SHOW_MESSAGE("Capturer<PICTURE>", "Path: " + filename);

    snipped();

    exit();
#endif
}

void ScreenShoter::copy()
{
    snipped();

    exit();
}

void ScreenShoter::pin()
{
    auto pix = snipped();
    auto pos = selected().topLeft();
    emit FIX_IMAGE(pix, pos);

    exit();
}

void ScreenShoter::registerShortcuts()
{
    connect(new QShortcut(Qt::CTRL + Qt::Key_S, this), &QShortcut::activated, [this](){
        if(status_ == CAPTURED) {
            save();
        }
    });

    connect(new QShortcut(Qt::Key_P, this), &QShortcut::activated, [this](){
        if(status_ == CAPTURED) {
            pin();
        }
    });

    connect(new QShortcut(Qt::CTRL + Qt::Key_Z, this), &QShortcut::activated, this, &ScreenShoter::undo);
    connect(new QShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z, this), &QShortcut::activated, this, &ScreenShoter::redo);

    auto setCurrent = [this](const History& history) {
        captured_screen_ = history.image_;
        box_.reset(history.rect_);
        commands_ = history.commands_;
        redo_stack_.clear();
        modified(PaintType::REPAINT_ALL);

        CAPTURED();

        if(!commands_.empty()) {
            focusOn(commands_.back());
            LOCKED();
        }
    };

    connect(new QShortcut(Qt::Key_PageUp, this), &QShortcut::activated, [=](){
        if(history_idx_ < history_.size()) {
            setCurrent(history_[history_idx_]);
            if(history_idx_ > 0) history_idx_--;
        }
    });

    connect(new QShortcut(Qt::Key_PageDown, this), &QShortcut::activated, [=](){
        if(history_idx_ < history_.size()) {
            setCurrent(history_[history_idx_]);
            if(history_idx_ < history_.size() - 1) history_idx_++;
        }
    });

    connect(new QShortcut(Qt::CTRL + Qt::Key_C, this), &QShortcut::activated, [=](){
        if(status_ < CAPTURED) {
            QApplication::clipboard()->setText(magnifier_->getColorStringValue());
        }
    });

    connect(new QShortcut(Qt::Key_Delete, this), &QShortcut::activated, [=](){
        if(status_ >= CAPTURED && focus_cmd_) {
            commands_.remove(focus_cmd_);
            focusOn(nullptr);
        }
    });
}

///////////////////////////////////////////////////// UNDO / REDO ////////////////////////////////////////////////////////////
void ScreenShoter::undo()
{
    while(!commands_.empty()) {
        auto ptr = commands_.back();

        // text cmd may be invalid
        if(ptr->isValid()) {
            redo_stack_.push(ptr);
            commands_.pop();

            focusOn(commands_.empty() ? nullptr : commands_.back());

            if(ptr->previous() != nullptr) ptr->previous()->visible(true);

            break;
        }
        commands_.pop();
    }
}

void ScreenShoter::redo()
{
    if(redo_stack_.empty()) return;

    auto ptr = redo_stack_.back();
    commands_.push(ptr);
    redo_stack_.pop();

    focusOn(commands_.back());

    if(ptr->previous() != nullptr) ptr->previous()->visible(false);
}
