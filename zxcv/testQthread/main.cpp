#include "widget.h"

#include <QApplication>
#include <QStackedLayout>


#include "frame.h"
#include "frame2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();

    QWidget w;

    auto layout = new QStackedLayout;
    auto f1= new Frame;
    layout->addWidget(f1);
    auto f2 = new Frame2;

    layout->setStackingMode(QStackedLayout::StackAll);
    w.setLayout(layout);

    QObject::connect(f1, &Frame::btnClicked, [=](){
        layout->addWidget(f2);
        layout->setCurrentWidget(f2);
    });

    QObject::connect(f2, &Frame2::btnOkClicked, [=]()
    {
        layout->setCurrentWidget(f1);
        layout->removeWidget(f2);
    });

    QObject::connect(f2, &Frame2::btnCancelClicked, [=]()
    {
        layout->setCurrentWidget(f1);
        layout->removeWidget(f2);
    });


    w.show();
    return a.exec();
}
