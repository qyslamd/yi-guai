#ifndef TABPAGE_TOOLBAR_H
#define TABPAGE_TOOLBAR_H

#include <QFrame>
#include "globaldef.h"

class CaptionFrame : public QFrame
{
    Q_OBJECT
public:
    explicit CaptionFrame(bool inprivate, QWidget *parent = nullptr);

    virtual bool event(QEvent *ev) override;

    int reservedWidth() const;
    QRect windowBtnRect();
signals:
    void minBtnClicked();
    void normalMaxBtnClicked();
    void closeBtnClicked();
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
private:
    bool inprivate_ = false;
    ///
    /// \brief The CaptionButtons enum
    ///
    enum class CaptionButtons{
        Button_None = -1,
        Button_Mini,
        Button_NormalMax,
        Button_Close,
    };

    const int BtnWidth = 45;
    const int BtnHeight = 28;
    const int BtnSpacing = 1;

    bool min_button_hover_ = false,
    max_button_hover_ = false,
    close_button_hover_ = false;

    bool min_button_press_ = false,
    max_buttton_press_ = false,
    close_button_press_ = false;

    // Get rectangle of caption buttons
    QRectF btnRect(CaptionButtons button);
    QPixmap btnPixmap(CaptionButtons button, const QColor &color);
    QColor getIconColor(bool isClose = false);
    CaptionButtons buttonAt(const QPoint &pos);

    void drawButtons(QPainter *p);

    void clearButtonHover();
};

class QToolButton;
class TabBar;
class QHBoxLayout;
class TabPageToolBar final: public CaptionFrame
{
    Q_OBJECT
public:
    explicit TabPageToolBar(bool inprivate, QWidget *parent = nullptr);
    ~TabPageToolBar();

    int insertTab(int index, const QString &text);
    int count()const;
    int currentIndex() const;
    void moveTab(int from, int to);
    void removeTab(int index);
    void setTabIcon(int index, const QIcon &icon);
    void setTabText(int index, const QString &text);
    QString tabText(int index) const;

    bool hitTestCaption(const QPoint &gPos);

    const int rightReserved = 180;
signals:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
    void showPreview(const QPoint &g_pos, const int index);
    void addPage();
    void showDockPage();
    void tabbarMenuTriggered(TabBarCmd cmd, const QVariant &data);
    void testBtnClicked();
public slots:
    void setCurrentIndex(int index);
    void onDwmColorChanged();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    bool inprivate_;
    QHBoxLayout *layout_;
    QToolButton *btn_dock_tabs_ = nullptr;
    TabBar *tab_bar_;
    QToolButton *btn_add_page_;

private:
    void initUi();
};

#endif // TABPAGE_TOOLBAR_H
