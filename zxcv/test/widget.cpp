#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QPainterPath>

#include <QMenuBar>
#include <QVBoxLayout>
#include <QMenu>
#include <QFrame>
#include <QtDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QFrame>
#include <QTabBar>
#include <QTimerEvent>
#include <QScrollArea>

#include "menubarstyle.h"
#include "VerticalTabbarStyle.h"
#include "ButtonLineEdit.h"
#include "ttabbar.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)/*
        , ui(new Ui::Widget)*/
{
    //    ui->setupUi(this);
//    testMenuBar();
//    testShortcut();
//    testLineEdit();
    testTabShape();
}

Widget::~Widget()
{
    //    delete ui;
}

bool Widget::eventFilter(QObject *obj, QEvent *ev)
{
    return QWidget::eventFilter(obj, ev);
}

void Widget::timerEvent(QTimerEvent *event)
{
    return QWidget::timerEvent(event);
}

QPixmap Widget::switchPix(bool on)
{
    QPixmap pix(64,64);
    pix.fill(Qt::transparent);

    auto x = 0;
    auto y = 0;
    auto w = pix.width();
    auto h = pix.height();
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);

    auto circlr_diameter = h / 2.0f;
    auto circle2_radius = circlr_diameter / 4.0f;
    QRectF leftRect(x,
                    y + h / 4.0,
                    circlr_diameter,
                    circlr_diameter);
    QRectF centerRect(x + circlr_diameter / 2.0,
                      y + h / 4.0,
                      w - circlr_diameter,
                      circlr_diameter);

    QRectF rightRect(x + w - circlr_diameter,
                     y + h / 4.0,
                     circlr_diameter,
                     circlr_diameter);

    QRectF leftSmallRect = leftRect.adjusted(circle2_radius,
                                             circle2_radius,
                                             -circle2_radius,
                                             -circle2_radius);
    QRectF rightSmallRect = rightRect.adjusted(circle2_radius,
                                               circle2_radius,
                                               -circle2_radius,
                                               -circle2_radius);

    if(on){
        p.save();
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0x0078D4));
        p.drawEllipse(leftRect);
        p.restore();

        p.save();
        p.fillRect(centerRect, QColor(0x0078D4));
        p.restore();

        p.save();
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0x0078D4));

        p.drawEllipse(rightRect);
        p.restore();


        p.save();
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0xFFFFFF));
        p.drawEllipse(rightSmallRect);
        p.restore();
    }else{
        p.save();
        p.setPen(QColor(0x666666));
        p.drawArc(leftRect, 90 *16, 180 * 16);
        p.drawLine(QPointF(x + circlr_diameter / 2.0, circlr_diameter / 2.0),
                   QPointF(x + w - circlr_diameter / 2.0, circlr_diameter / 2.0));
        p.drawArc(rightRect, 270 *16, 180 * 16);
        p.drawLine(QPointF(x + w - circlr_diameter / 2.0, y + h - circlr_diameter / 2.0),
                   QPointF(x + circlr_diameter / 2.0, y + h - circlr_diameter / 2.0)
                   );
        p.restore();

        p.save();
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.drawEllipse(leftSmallRect);
        p.restore();
    }

    return pix;
}

void Widget::testMenuBar()
{
    resize(500,300);
    QMenuBar *menubar = new QMenuBar;
    menubar->setStyle(new MenuBarStyle);
    QMenu *menuFile = new QMenu("File");
    menuFile->setIcon(QIcon(":/res/imgs/1.png"));

    menuFile->addAction("open");
    menuFile->addAction("open");
    menuFile->addAction("open");
    menubar->addMenu(menuFile);
    menubar->addAction(new QAction(QIcon(":/res/imgs/1.png"),"NIMA", this));

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setMenuBar(menubar);

    QFrame *frame = new QFrame(this);
    frame->setStyleSheet("border:1px solid red;");
    layout->addWidget(frame);

    qInfo()<<menuFile->style();
}

void Widget::testShortcut()
{
    QAction *acClose = new QAction(this);
    connect(acClose, &QAction::triggered, this, &Widget::close);
    acClose->setShortcut(QKeySequence::FullScreen);
    auto bindings = QKeySequence::keyBindings(QKeySequence::FullScreen);
    qInfo()<<bindings;

    addAction(acClose);
}

void Widget::testLineEdit()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(9,9,9,9);
    mainLayout->setSpacing(6);

    ButtonLineEdit *ff1 =  new ButtonLineEdit;
    QTextEdit *ff2 =  new QTextEdit;
    mainLayout->addWidget(ff1);
    mainLayout->addWidget(ff2);

    setLayout(mainLayout);
}

void Widget::testTabShape()
{
    resize(300,300);
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QToolButton *btnMenu = new QToolButton;
    layout->addWidget(btnMenu);

    TTabBar *tabbar = new TTabBar;
    connect(tabbar, &QTabBar::tabCloseRequested, this,[=](int index)
    {
        tabbar->removeTab(index);
    });
    connect(tabbar, &QTabBar::currentChanged, this,[=](int index)
    {
//        qInfo()<<"L:"<<tabbar->tabRect(index);
    });
    tabbar->setStyle(new VerticalTabbarStyle);
    auto icon = style()->standardIcon(QStyle::SP_ComputerIcon);
    int a = 0;
    tabbar->addTab(icon, "ASDFFSS" + QString::number(a++));
    tabbar->addTab(icon, "ASDFFSS" + QString::number(a++));
    tabbar->addTab(icon, "ASDFFSS" + QString::number(a++));
    tabbar->setCurrentIndex(2);

    QScrollArea *scrollarea = new QScrollArea(this);
    scrollarea->setWidget(tabbar);
    scrollarea->setWidgetResizable(true);
    layout->addWidget(scrollarea);

    QFrame *line = new QFrame;
    line->setGeometry(QRect(160, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    QToolButton *btnAdd = new QToolButton;
    connect(btnAdd, &QToolButton::clicked, this, [=](){
        static int a = 3;
        tabbar->addTab(icon, "QWERTTY" + QString::number(a++));
    });
    layout->addWidget(btnAdd);
//    layout->addStretch();
}
