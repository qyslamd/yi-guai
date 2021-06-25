#include "UserInfoPopup.h"
#include "ui_UserInfoPopup.h"
#include <QSysInfo>
#include <QSettings>
#include <QAction>
#include <QtDebug>


UserInfoPopup::UserInfoPopup(QWidget *parent) :
    PopupBase(parent),
    ui(new Ui::UserInfoPopup)
{
    ui->setupUi(contentFrame());
    ui->lineEditName->installEventFilter(this);

    connect(ui->btnClose, &QToolButton::clicked, this, [this]()
    {
        auto window = this->window();
        window->close();
    });

    connect(ui->btnRename, &QToolButton::clicked, this,[this](){
        ui->lineEditName->setReadOnly(true);
        ui->lineEditName->setReadOnly(false);
//        ui->lineEditName->setFocus();
//        ui->lineEditName->selectAll();
    });
}

UserInfoPopup::~UserInfoPopup()
{
    delete ui;
}

bool UserInfoPopup::eventFilter(QObject *obj, QEvent *ev)
{
//    if(obj == ui->lineEditName){
//        if(ev->type() == QEvent::FocusOut){
//            ui->lineEditName->setReadOnly(true);
//        }
//    }
    return PopupBase::eventFilter(obj, ev);
}

void UserInfoPopup::showEvent(QShowEvent *)
{
    auto osName = QSysInfo::prettyProductName(), kernelVer = QSysInfo::kernelVersion();
    ui->editOS->clear();
    ui->editOS->appendPlainText(osName + " " + kernelVer);
    ui->editDevice->clear();
    ui->editDevice->appendPlainText(QSysInfo::machineHostName());
#if defined Q_OS_WIN
    ui->editCPU->clear();
    ui->editCPU->appendPlainText(QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",QSettings::NativeFormat)
                                 .value("ProcessorNameString")
                                 .toString()
                                 );
#else
    ui->editCPU->clear();
    ui->editCPU->appendPlainText("unknown");
#endif
}
