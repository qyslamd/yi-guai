#ifndef QT_WIN_FRAMELESS_H
#define QT_WIN_FRAMELESS_H

#include <qsystemdetection.h>
#include <QMainWindow>

#if defined(Q_OS_WIN)

class QtWinFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QtWinFramelessWindow(QWidget *parent = 0);
    virtual bool event(QEvent *ev) override;
signals:
    void DwmCompsitionChanged();
    void dpiChanged(const int dpi);
protected:
    virtual bool nativeEvent(const QByteArray &eventType,
                             void *message, long *result) override;
    ///
    /// \brief Implemention in subclass to judge HTCAPTION
    /// \param gPos - global cursor postion
    /// \return return true means HTCAPTION, otherwise HTCLIENT
    ///
    virtual bool hitTestCaption(const QPoint &gPos);
private:
    bool maximized_ = false;
};
#else
class QtWinFramelessWindow : public QMainWindow
{
public:
    explicit QtWinFramelessWindow(QWidget * parent = nullptr) : QMainWindow(parent){}
};
#endif

#endif // QT_WIN_FRAMELESS_H
