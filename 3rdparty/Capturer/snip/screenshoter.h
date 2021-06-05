﻿#ifndef TESTCAPTURER_H
#define TESTCAPTURER_H

#include <selector.h>
#include <QTextEdit>
#include <QPixmap>
#include <QSystemTrayIcon>
#include "imageeditmenu.h"
#include "magnifier.h"
#include "textedit.h"
#include "command.h"
#include "resizer.h"
#include "circlecursor.h"
#include "captureconfig.h"

class ScreenShoter : public Selector
{
    Q_OBJECT

public:
    explicit ScreenShoter(QWidget *parent = nullptr);
    virtual ~ScreenShoter() override{}
signals:
    void focusOnGraph(Graph);
    void FIX_IMAGE(const QPixmap& image, const QPoint& pos);
    void SHOW_MESSAGE(const QString &title, const QString &msg,
                      QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information, int msecs = 10000);
public:
    enum EditStatus: std::uint32_t {
        NONE            = 0x00000000,
        READY           = 0x00010000,
        GRAPH_CREATING  = 0x00100000,
        GRAPH_MOVING    = 0x00200000,
        GRAPH_RESIZING  = 0x00400000,
        GRAPH_ROTATING  = 0x00800000,

        READY_MASK      = 0x000f0000,
        OPERATION_MASK  = 0xfff00000,
        GRAPH_MASK      = 0x0000ffff
    };

public slots:
    virtual void start() override;
    virtual void exit() override;

    void save();
    void copy();
    void pin();
    QPixmap snipped();

    void undo();
    void redo();

    void updateTheme()
    {
        Selector::updateTheme(CaptureConfig::instance()["snip"]["selector"]);
    }

public slots:
    void modified(PaintType type) { modified_ = (type > modified_) ? type : modified_; update(); }

private slots:
    void moveMenu();

protected:
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void wheelEvent(QWheelEvent *) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *) override;

private:
    void updateCanvas();

    QImage mosaic(const QImage&);
    void registerShortcuts();

    void updateHoverPos(const QPoint&);
    void setCursorByHoverPos(Resizer::PointPosition, const QCursor & default_cursor = Qt::CrossCursor);

    void moveMagnifier();

    QPixmap snippedImage();

    void focusOn(shared_ptr<PaintCommand> cmd);

    QPixmap captured_screen_;
    QImage mosaic_background_;

    Resizer::PointPosition hover_position_ = Resizer::OUTSIDE;

    shared_ptr<PaintCommand> hover_cmd_ = nullptr;    // hover
    shared_ptr<PaintCommand> focus_cmd_ = nullptr;    // focus

    std::uint32_t edit_status_ = EditStatus::NONE;

    TextEdit * text_edit_ = nullptr;

    ImageEditMenu * menu_ = nullptr;
    Magnifier * magnifier_ = nullptr;

    QPoint move_begin_{0, 0};
    QPoint resize_begin_{0, 0};
    std::vector<QPoint> curves_;

    CommandStack commands_;
    CommandStack redo_stack_;

    CircleCursor circle_cursor_{20};
    QPixmap canvas_;

    struct History{
        QPixmap image_;
        QRect rect_;
        CommandStack commands_;
    };

    std::vector<History> history_;
    size_t history_idx_ = 0;
};

#endif // TESTCAPTURER_H
