#ifndef ADDRESSBAR2_H
#define ADDRESSBAR2_H

#include <QFrame>

class QHBoxLayout;
class QPushButton;
class QLineEdit;
class QToolButton;
class QCompleter;
class QStringListModel;

class AddressBar final: public QFrame
{
    Q_OBJECT
public:
    explicit AddressBar(bool inprivate = false, QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *ev) override;

    void setText(const QString &text);
    void setCursorPosition(int pos);
    QString text();

    QRect gGeometryBtnSiteInfo() const;
    QRect gGeometryBtnAddBkmk() const;
    QRect gGeometryBtnZoom() const;
    void setZoomLevelValue(double str);

    void updateBtnState(bool checked);
signals:
    void viewSiteInfo();
    void showZoomBar();
    void addFavorite();
    void returnPressed();
private:
    bool inprivate_ = false;

    QCompleter *completer_ = nullptr;
    QStringListModel *model_ = nullptr;
    QHBoxLayout *layout_ = nullptr;
    QPushButton *btn_site_info_ = nullptr;    /*use pushbutton to show text and icon*/
    QLineEdit *line_edit_addr_ = nullptr;
    QToolButton *btn_zoom_hint_ = nullptr;
    QToolButton *btn_find_hint_ = nullptr;
    QToolButton *btn_add_favorite_ = nullptr;

    double zoom_value_ = 0.0;

    void initUi();
    void setIcons();

private slots:
    void onEditingFinished();
};

#endif // ADDRESSBAR2_H
