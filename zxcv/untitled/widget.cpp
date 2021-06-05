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

#include "menubarstyle.h"
#include "listtabstyle.h"
#include "myframe.h"

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

    MyFrame *ff1 =  new MyFrame;
    QTextEdit *ff2 =  new QTextEdit;
    mainLayout->addWidget(ff1);
    mainLayout->addWidget(ff2);

    setLayout(mainLayout);
}

void Widget::testTabShape()
{
    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout *tabLayout = new QVBoxLayout;
    QToolButton *btnMenu = new QToolButton;
    tabLayout->addWidget(btnMenu);

    QTabBar *tabbar = new QTabBar(this);
    tabbar->setDrawBase(false);
    tabbar->setStyle(new ListTabStyle);
    tabbar->setShape(QTabBar::RoundedWest);
    tabbar->setTabsClosable(true);
    tabbar->setMovable(true);
    tabbar->addTab("ASDFFSS");
    tabbar->addTab("ASDFFSS");
    tabbar->addTab("ASDFFSS");

    tabLayout->addWidget(tabbar);

    QToolButton *btnAdd = new QToolButton;
    tabLayout->addWidget(btnAdd);
    tabLayout->addStretch();

    layout->addLayout(tabLayout);
    QFrame *frame = new QFrame;
    frame->setStyleSheet("border:1px solid red;");
    layout->addWidget(frame);

    setLayout(layout);
}
