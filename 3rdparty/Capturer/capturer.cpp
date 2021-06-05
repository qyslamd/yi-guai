﻿#include "capturer.h"
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QTextEdit>
#include <QScrollBar>
#include <QtDebug>
#include <QMimeData>
#include <QPixmap>
#include <QMenu>
#include "logging.h"
#include "imagewindow.h"
#include "screenshoter.h"
#include "screenrecorder.h"
#include "gifcapturer.h"
#include "capsettingdialog.h"
#include "./3rdparty/qxtglobalshortcut/include/qxtglobalshortcut.h"
#include "./3rdparty/json.hpp"
#include "./snip/clipboardhistorywindow.h"

#define SET_HOTKEY(X, Y)    st(                                 \
                                if(!X->setShortcut(Y))          \
                                    showMessage("Capturer", QObject::tr("Failed to register shortcut <%1>").arg(Y.toString()), QSystemTrayIcon::Critical); \
                            )

Capturer::Capturer(QWidget *parent)
    : QWidget(parent)
{
    CAP_LOG(INFO_MSG) << "[Capturer] initializing.";

    LOAD_QSS(this, ":/qss/capturer.qss");

    sniper_ = new ScreenShoter(this);
    recorder_ = new ScreenRecorder(this);
    gifcptr_ = new GifCapturer(this);

    connect(sniper_, &ScreenShoter::FIX_IMAGE, [this](const QPixmap& image, const QPoint& pos) {
        auto window = make_shared<ImageWindow>(image, pos, this);
        clipboard_history_.push(window);
        window->fix();
    });
#ifndef PROJ_MAKE_LIB
    sys_tray_icon_ = new QSystemTrayIcon(this);

    snip_sc_ = new QxtGlobalShortcut(this);
    connect(snip_sc_, &QxtGlobalShortcut::activated, sniper_, &ScreenShoter::start);

    pin_sc_ = new QxtGlobalShortcut(this);
    connect(pin_sc_, &QxtGlobalShortcut::activated, this, &Capturer::pinLastImage);

    show_pin_sc_ = new QxtGlobalShortcut(this);
    connect(show_pin_sc_, &QxtGlobalShortcut::activated, this, &Capturer::showImages);

    video_sc_ = new QxtGlobalShortcut(this);
    connect(video_sc_, &QxtGlobalShortcut::activated, recorder_, &ScreenRecorder::record);

    gif_sc_ = new QxtGlobalShortcut(this);
    connect(gif_sc_, &QxtGlobalShortcut::activated, gifcptr_, &GifCapturer::record);
#endif

    connect(&CaptureConfig::instance(), &CaptureConfig::changed, this, &Capturer::updateConfig);

    // setting
    setting_dialog_ = new CapSettingWindow(this);

    // System tray icon
    // @attention Must after setting.
    setupSystemTrayIcon();

    updateConfig();

    // show message
    connect(sniper_, &ScreenShoter::SHOW_MESSAGE, this, &Capturer::showMessage);
    connect(recorder_, &ScreenRecorder::SHOW_MESSAGE, this, &Capturer::showMessage);
    connect(recorder_, &ScreenRecorder::recordFinished, [this](const QString &filePath){
        emit captureFinished(tr("Record Vedio Finished"), filePath);
    });
    connect(gifcptr_, &GifCapturer::SHOW_MESSAGE, this, &Capturer::showMessage);
    connect(gifcptr_, &GifCapturer::recordFinished, [this](const QString &filePath){
        emit captureFinished(tr("Record Gif Finished"), filePath);
    });

    // clipboard
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &Capturer::clipboardChanged);

    CAP_LOG(INFO_MSG) << "[Capturer] initialized.";
}

Capturer::~Capturer()
{
    qInfo()<<__FUNCTION__;
}

void Capturer::updateConfig()
{
    auto &config = CaptureConfig::instance();

#ifndef PROJ_MAKE_LIB
    SET_HOTKEY(snip_sc_, config["snip"]["hotkey"].get<QKeySequence>());
    SET_HOTKEY(pin_sc_, config["pin"]["hotkey"].get<QKeySequence>());
    SET_HOTKEY(show_pin_sc_, config["pin"]["visiable"]["hotkey"].get<QKeySequence>());
    SET_HOTKEY(gif_sc_, config["gif"]["hotkey"].get<QKeySequence>());
    SET_HOTKEY(video_sc_, config["record"]["hotkey"].get<QKeySequence>());
#endif
    recorder_->setFramerate(config["record"]["framerate"].get<int>());
    gifcptr_->setFramerate(config["gif"]["framerate"].get<int>());

    sniper_->updateTheme();
    recorder_->updateTheme();
    gifcptr_->updateTheme();
}

void Capturer::setupSystemTrayIcon()
{
#ifndef PROJ_MAKE_LIB
    // SystemTrayIcon
    sys_tray_icon_menu_ = new QMenu(this);
    sys_tray_icon_menu_->setObjectName("sys_tray_menu");

    auto screen_shot = new QAction(QIcon(":/icon/res/screenshot"), tr("Snip"), this);
    connect(screen_shot, &QAction::triggered, sniper_, &ScreenShoter::start);
    sys_tray_icon_menu_->addAction(screen_shot);

    auto record_screen = new QAction(QIcon(":/icon/res/sr"),tr("Screen Record"), this);
    connect(record_screen, &QAction::triggered, recorder_, &ScreenRecorder::record);
    sys_tray_icon_menu_->addAction(record_screen);

    auto gif = new QAction(QIcon(":/icon/res/gif"), tr("GIF Record"), this);
    connect(gif, &QAction::triggered, gifcptr_, &GifCapturer::record);
    sys_tray_icon_menu_->addAction(gif);

    sys_tray_icon_menu_->addSeparator();

    auto setting = new QAction(QIcon(":/icon/res/setting"), tr("Settings"), this);
    connect(setting, &QAction::triggered, setting_dialog_, &CapSettingWindow::show);
    sys_tray_icon_menu_->addAction(setting);

    sys_tray_icon_menu_->addSeparator();

    auto exit_action = new QAction(QIcon(":/icon/res/exit"), tr("Quit"), this);
    connect(exit_action, &QAction::triggered, qApp, &QCoreApplication::exit);
    sys_tray_icon_menu_->addAction(exit_action);

    sys_tray_icon_->setContextMenu(sys_tray_icon_menu_);
    sys_tray_icon_->setIcon(QIcon(":/icon/res/icon.png"));
    setWindowIcon(QIcon(":/icon/res/icon.png"));
    sys_tray_icon_->show();

#endif
}


void Capturer::showMessage(const QString &title, const QString &msg, QSystemTrayIcon::MessageIcon icon, int msecs)
{
#ifndef PROJ_MAKE_LIB
    sys_tray_icon_->showMessage(title, msg, icon, msecs);

    if(icon == QSystemTrayIcon::Critical) CAP_LOG(ERROR_MSG) << msg;
#endif
}

void Capturer::clipboardChanged()
{
    const auto mimedata = QApplication::clipboard()->mimeData();

    // only image
    if(mimedata->hasHtml() && mimedata->hasImage()) {
        CAP_LOG(INFO_MSG) << "IAMGE";

        auto image_rect = mimedata->imageData().value<QPixmap>().rect();
        image_rect.moveCenter(DisplayInfo::screens()[0]->geometry().center());
        clipboard_history_.push(make_shared<ImageWindow>(mimedata->imageData().value<QPixmap>(), image_rect.topLeft()));
    }
    else if(mimedata->hasHtml()) {
        CAP_LOG(INFO_MSG) << "HTML";

        QTextEdit view;
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setLineWrapMode(QTextEdit::NoWrap);

        view.setHtml(mimedata->html());
        view.setFixedSize(view.document()->size().toSize());

        clipboard_history_.push(make_shared<ImageWindow>(view.grab(), QCursor::pos(), this));
    }
    else if(mimedata->hasFormat("application/qpoint") && mimedata->hasImage()) {
        CAP_LOG(INFO_MSG) << "SNIP";

        auto pos = *reinterpret_cast<QPoint *>(mimedata->data("application/qpoint").data());
        for(const auto& item : clipboard_history_) {
            if(item->image().cacheKey() == mimedata->imageData().value<QPixmap>().cacheKey()) {
                return;
            }
        }
        clipboard_history_.push(make_shared<ImageWindow>(mimedata->imageData().value<QPixmap>(), pos, this));
    }

    else if(mimedata->hasUrls() && QString("jpg;jpeg;png;JPG;JPEG;PNG;bmp;BMP").contains(QFileInfo(mimedata->urls()[0].fileName()).suffix())) {
        CAP_LOG(INFO_MSG) << "IMAGE URL";

        QPixmap pixmap;
        pixmap.load(mimedata->urls()[0].toLocalFile());
        auto image_rect = pixmap.rect();
        image_rect.moveCenter(DisplayInfo::screens()[0]->geometry().center());
        clipboard_history_.push(make_shared<ImageWindow>(pixmap, image_rect.topLeft(), this));
    }
    else if(mimedata->hasText()) {
        CAP_LOG(INFO_MSG) << "TEXT";

        auto label = new QLabel(mimedata->text());
        label->setWordWrap(true);
        label->setMargin(10);
        label->setStyleSheet("background-color:white");
        label->setFont({"Consolas", 12});

        clipboard_history_.push(make_shared<ImageWindow>(label->grab(), QCursor::pos(), this));
    }
    else if(mimedata->hasColor()) {
        // Do nothing.
        CAP_LOG(WARNING_MSG) << "COLOR";
    }
}

void Capturer::snip()
{
#ifdef PROJ_MAKE_LIB
    sniper_->start();
#endif
}

void Capturer::recordScreen()
{
#ifdef PROJ_MAKE_LIB
    recorder_->record();
#endif
}

void Capturer::recordGif()
{
#ifdef PROJ_MAKE_LIB
    gifcptr_->record();
#endif
}

void Capturer::captureSettings()
{
#ifdef PROJ_MAKE_LIB
    setting_dialog_->exec();
#endif
}

void Capturer::pinLastImage()
{
    if(clipboard_history_.empty()) return;

    const auto& last = clipboard_history_.back();
    last->fix();
}

void Capturer::showImages()
{
    images_visible_ = !images_visible_;
    for(auto& window: clipboard_history_) {
        images_visible_ ? window->show() : window->hide();
    }
}
