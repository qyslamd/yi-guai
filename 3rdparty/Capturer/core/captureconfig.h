#ifndef CAPTURE_CONFIG_H
#define CAPTURE_CONFIG_H

#include <QObject>
#include "json.h"
#include "utils.h"

#define IF_NULL_SET(X, default_value) st(if(X.is_null())  X = default_value;)

class CaptureConfig : public QObject
{
    Q_OBJECT

public:
    static CaptureConfig& instance()
    {
        static CaptureConfig instance;
        return instance;
    }

    CaptureConfig(const CaptureConfig&) = delete;
    CaptureConfig& operator=(const CaptureConfig&) = delete;

    QString getFilePath() const { return filepath_; }

    template <typename T> void set(json& key, T value)
    { 
        key = value; 
        emit changed(); 
    }

    decltype(auto) operator[](const std::string& key) { return settings_[key]; }
    decltype(auto) operator[](const std::string& key) const { return settings_[key]; }

public slots:
	void save();
signals:
    void changed();

private:
    CaptureConfig();
    const QString settings_file_{"capture_config.json"};

    QString filepath_;
    json settings_ = json::parse("{}");
};


#endif // CAPTURE_CONFIG_H
