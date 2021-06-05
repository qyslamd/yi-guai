#ifndef QT_WIN_FRAMELESS_H
#define QT_WIN_FRAMELESS_H

#include <qsystemdetection.h>
#include <QMainWindow>

#if defined(Q_OS_WIN)
#include <QList>
#include <QMargins>
#include <QRect>
class QtWinFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QtWinFramelessWindow(QWidget *parent = 0);
public:
    void setContentsMargins(const QMargins &margins);
    void setContentsMargins(int left, int top, int right, int bottom);
    QMargins contentsMargins() const;
    QRect contentsRect() const;
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
signals:
    void DwmCompsitionChanged();
    void dpiChanged(const int dpi);
protected:
    virtual bool nativeEvent(const QByteArray &eventType,
                             void *message, long *result) override;
    virtual void showEvent(QShowEvent *event) override;

    ///
    /// \brief Implemention in subclass to judge HTCAPTION
    /// \param gPos - global cursor postion
    /// \return return true means HTCAPTION, otherwise HTCLIENT
    ///
    virtual bool hitTestCaption(const QPoint &gPos) {return false;}
public slots:
    void showFullScreen();
private:
    QMargins m_margins;
    QMargins m_frames;
    bool m_bJustMaximized;
    bool first_shown_ = true;
};
#else
class CFramelessWindow : public QMainWindow
{
public:
    explicit CFramelessWindow(QWidget * parent = nullptr) : QMainWindow(parent){}
};
#endif

#endif // QT_WIN_FRAMELESS_H
