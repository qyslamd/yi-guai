#include "widget.h"

#include <QApplication>

#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>

#include "ButtonLineEdit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
