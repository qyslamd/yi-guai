#include "captureconfig.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include "utils.h"
#include "logging.h"

static QString GetTruePath(const QString &orig, const QString &appName)
{
    QString dirName1, dirName2;
    QDir dir(orig);
    dirName1 = dir.dirName();
    if(dir.cdUp()){
        dirName2 = dir.dirName();
    }

    if(dirName1 == dirName2 && dirName1 == appName){
        return dir.absolutePath();
    }

    return orig;
}

CaptureConfig::CaptureConfig()
{
    auto config_dir_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir config_dir(config_dir_path);
    if(!config_dir.exists()) {
        config_dir.mkpath(config_dir_path);
    }
    filepath_ = QDir(config_dir_path).absoluteFilePath(settings_file_);
#ifdef PROJ_MAKE_LIB
    auto truePath = GetTruePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                ,QCoreApplication::applicationName());
    filepath_ = QDir(truePath).absoluteFilePath(settings_file_);
#endif
    CAP_LOG(INFO_MSG) << "[Capturer] config file path: " << filepath_;

    QString text;
    QFile config_file(filepath_);
    if(config_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream in(&config_file);
        text = in.readAll();
    }

    try {
        settings_ = json::parse(text.toStdString());
    }
    catch (json::parse_error&) {
        settings_ = json::parse("{}");
    }

    // default
#ifdef PROJ_MAKE_LIB
    IF_NULL_SET(settings_["savepath"], QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
#else
    IF_NULL_SET(settings_["autorun"], true);
    IF_NULL_SET(settings_["language"], "zh_CN");
#endif
    IF_NULL_SET(settings_["detectwindow"], true);

    IF_NULL_SET(settings_["snip"]["selector"]["border"]["width"],   2);
    IF_NULL_SET(settings_["snip"]["selector"]["border"]["color"],   "#409EFF");
    IF_NULL_SET(settings_["snip"]["selector"]["border"]["style"],   Qt::DashDotLine);
    IF_NULL_SET(settings_["snip"]["selector"]["mask"]["color"],     "#88000000");

    IF_NULL_SET(settings_["record"]["selector"]["border"]["width"], 2);
    IF_NULL_SET(settings_["record"]["selector"]["border"]["color"], "#ffff5500");
    IF_NULL_SET(settings_["record"]["selector"]["border"]["style"], Qt::DashDotLine);
    IF_NULL_SET(settings_["record"]["selector"]["mask"]["color"],   "#88000000");

    IF_NULL_SET(settings_["gif"]["selector"]["border"]["width"], 2);
    IF_NULL_SET(settings_["gif"]["selector"]["border"]["color"], "#ffff00ff");
    IF_NULL_SET(settings_["gif"]["selector"]["border"]["style"], Qt::DashDotLine);
    IF_NULL_SET(settings_["gif"]["selector"]["mask"]["color"],   "#88000000");

#ifndef PROJ_MAKE_LIB
    IF_NULL_SET(settings_["snip"]["hotkey"],                "Ctrl+Shift+S");
    IF_NULL_SET(settings_["pin"]["hotkey"],                 "F3");
    IF_NULL_SET(settings_["pin"]["visiable"]["hotkey"],     "Shift+F3");
    IF_NULL_SET(settings_["record"]["hotkey"],              "Ctrl+Alt+V");
    IF_NULL_SET(settings_["gif"]["hotkey"],                 "Ctrl+Alt+G");
#endif

    IF_NULL_SET(settings_["record"]["framerate"],   30);
    IF_NULL_SET(settings_["gif"]["framerate"],      6);

    connect(this, &CaptureConfig::changed, this, &CaptureConfig::save);
    emit changed();
}

void CaptureConfig::save()
{
    QFile file(filepath_);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << settings_.dump(4).c_str();

    file.close();
}
