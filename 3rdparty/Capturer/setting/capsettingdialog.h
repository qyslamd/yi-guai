#ifndef CAP_SETTINGDIALOG_H
#define CAP_SETTINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>
#include <QLabel>
#include <QLineEdit>
#include "apptabcontrol.h"
#include "captureconfig.h"

class CapSettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CapSettingWindow(QWidget * parent = nullptr);
    ~CapSettingWindow() = default;

private slots:
    void setAutoRun(int);

private:
    void setupGeneralWidget();
    void setupAppearanceWidget();
    void setupSnipWidget();
    void setupRecordWidget();
    void setupGIFWidget();
    void setupHotkeyWidget();

    CaptureConfig &config_ = CaptureConfig::instance();

    AppTabControl *tabwidget_ = nullptr;
};

#endif // CAP_SETTINGDIALOG_H
