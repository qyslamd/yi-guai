#include "gifcapturer.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>
#include <QStandardPaths>
#include "detectwidgets.h"
#include "logging.h"

GifCapturer::GifCapturer(QWidget * parent)
    : Selector(parent)
{
    process_ = new QProcess(this);
    record_menu_ = new RecordMenu();

    connect(record_menu_, &RecordMenu::STOP, this, &GifCapturer::exit);

    QDir dir(QCoreApplication::applicationDirPath());
#ifdef Q_OS_WIN
    ffmpeg_path_ = dir.absoluteFilePath("ffmpeg.exe");
#else
    ffmpeg_path_ = dir.absoluteFilePath("ffmpeg");
#endif
}

void GifCapturer::record()
{
    status_ == INITIAL ? start() : exit();
}

void GifCapturer::setup()
{
    record_menu_->start();

    status_ = LOCKED;
    hide();

    QString save_path = CaptureConfig::instance()["savepath"].get<QString>();
    if(save_path.isEmpty()){
        save_path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
        CaptureConfig::instance().set(CaptureConfig::instance()["savepath"], save_path);
    }
    QDateTime dateTime;
    dateTime.setDate(QDate::currentDate());
    dateTime.setTime(QTime::currentTime());
    current_time_str_ = dateTime.toString("yyyyMMdd_hhmmss_zzz");

    filename_ = QDir(save_path).absoluteFilePath("Capturer_gif_" + current_time_str_ + ".gif");

    QStringList args;
    auto selected_area = selected();
    auto temp_dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    temp_video_path_ = QDir(temp_dir).absoluteFilePath("Capturer_gif_" + current_time_str_ + ".mp4");
    temp_palette_path_ = QDir(temp_dir).absoluteFilePath("Capturer_palette_" + current_time_str_ + ".png");

#ifdef __linux__
    args << "-video_size"   << QString("%1x%2").arg(selected_area.width()).arg(selected_area.height())
         << "-framerate"    << QString("%1").arg(framerate_)
         << "-f"            << "x11grab"
         << "-i"            << QString(":0.0+%1x%2").arg(selected_area.x()).arg(selected_area.y())
         << temp_video_path_;
#elif _WIN32
    args << "-f"            << "gdigrab"
         << "-framerate"    << QString("%1").arg(framerate_)
         << "-offset_x"     << QString("%1").arg(selected_area.x())
         << "-offset_y"     << QString("%1").arg(selected_area.y())
         << "-video_size"   << QString("%1x%2").arg(selected_area.width()).arg(selected_area.height())
         << "-i"            << "desktop"
         << temp_video_path_;
#endif
    process_->start(ffmpeg_path_, args);
}

void GifCapturer::exit()
{
    process_->write("q\n\r");
    process_->waitForFinished();

    QStringList args;
    args << "-y"
         << "-i"    << temp_video_path_
         << "-vf"   << QString("fps=%1,palettegen").arg(fps_)
         << temp_palette_path_;
    process_->start(ffmpeg_path_, args);
    process_->waitForFinished();

    args.clear();
    args << "-i" << temp_video_path_
         << "-i" << temp_palette_path_
         << "-filter_complex" << QString("fps=%1,paletteuse").arg(fps_)
         << filename_;

    process_->start(ffmpeg_path_, args);
    process_->waitForFinished();

    record_menu_->stop();
    CAP_LOG(INFO_MSG)<<"[Capture] Record Gif Finished."<<filename_;

    //删除临时文件
    QFile::remove(temp_video_path_);
    QFile::remove(temp_palette_path_);
    emit recordFinished(filename_);
    emit SHOW_MESSAGE("Capturer<GIF>", tr("Path: ") + filename_);

    Selector::exit();
}


void GifCapturer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        Selector::exit();
    }

    if(event->key() == Qt::Key_Return) {
        setup();
    }
}

