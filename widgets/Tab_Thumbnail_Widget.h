#ifndef TAB_THUMBNAIL_WIDGET_H
#define TAB_THUMBNAIL_WIDGET_H

#include <QWidget>

namespace Ui {
class Tab_Thumbnail_Widget;
}

class Tab_Thumbnail_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Tab_Thumbnail_Widget(QWidget *parent = nullptr);
    ~Tab_Thumbnail_Widget();

    void setDescriptionTxt(const QString &text);
    void setImgVisible(bool visible);
    void setThumbnailImg(const QPixmap &pix);

private:
    Ui::Tab_Thumbnail_Widget *ui;
};

#endif // TAB_THUMBNAIL_WIDGET_H
