#ifndef CAPTURER_MAINWINDOW_H
#define CAPTURER_MAINWINDOW_H

#include <vector>
#include <queue>
#include <memory>
#include <QSystemTrayIcon>
#include "./core/imagewindow.h"

template <typename T, int MAX_SIZE = 100>
class LimitSizeVector : public std::vector<T> {
public:
    void push(const T& value)
    {
        this->push_back(value);

        if(this->size() > MAX_SIZE) {
            this->erase(this->begin());
        }
    }
};

class QSystemTrayIcon;
class QMenu;
class ScreenShoter;
class ScreenRecorder;
class GifCapturer;
class CapSettingWindow;
class QxtGlobalShortcut;
class Capturer : public QWidget
{
    Q_OBJECT

public:
    explicit Capturer(QWidget *parent = nullptr);
    virtual ~Capturer() override;


    void snip();
    void recordScreen();
    void recordGif();
    void captureSettings();

signals:
    void captureFinished(const QString &msg, const QString &filePath);
    void recordVedioFinished(const QString &msg, const QString &filePath);

private slots:
    void pinLastImage();
    void showImages();

    void updateConfig();

    void showMessage(const QString &title, const QString &msg,
                     QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information,
                     int msecs = 10000);

    void clipboardChanged();

private:
    ScreenShoter * sniper_ = nullptr;
    ScreenRecorder * recorder_ = nullptr;
    GifCapturer * gifcptr_ = nullptr;

    LimitSizeVector<std::shared_ptr<ImageWindow>> clipboard_history_;
    CapSettingWindow * setting_dialog_ = nullptr;

    void setupSystemTrayIcon();
#ifndef PROJ_MAKE_LIB
    QSystemTrayIcon *sys_tray_icon_ = nullptr;
    QMenu * sys_tray_icon_menu_ = nullptr;
    // hotkey
    QxtGlobalShortcut *snip_sc_ = nullptr;
    QxtGlobalShortcut *show_pin_sc_ = nullptr;
    QxtGlobalShortcut *pin_sc_ = nullptr;
    QxtGlobalShortcut *gif_sc_ = nullptr;
    QxtGlobalShortcut *video_sc_ = nullptr;
#endif

    bool images_visible_ = true;
};

#endif // MAINWINDOW_H
