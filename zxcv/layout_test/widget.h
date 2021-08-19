#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QFrame;
class QVBoxLayout;
namespace Ui {
    class FrameCaption;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QVBoxLayout *layout_;
    QFrame *frame_caption;
    Ui::FrameCaption *ui_frame_caption_;
};

#endif // WIDGET_H
