#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

// Log
#ifndef CAP_LOG
    #define ERROR_MSG       QMessageLogger(__FILE__, __LINE__, __FUNCTION__).critical().nospace().noquote()
    #define INFO_MSG        QMessageLogger(__FILE__, __LINE__, __FUNCTION__).info().nospace().noquote()
    #define DEBUG_MSG       QMessageLogger(__FILE__, __LINE__, __FUNCTION__).debug().nospace().noquote()
    #define WARNING_MSG     QMessageLogger(__FILE__, __LINE__, __FUNCTION__).warning().nospace().noquote()
    #define FATAL_MSG       QMessageLogger(__FILE__, __LINE__, __FUNCTION__).fatal().nospace().noquote()

    #define CAP_LOG(X)      X
#endif

inline QDebug operator<<(QDebug out, const std::string& str)
{
    out << QString::fromStdString(str);
    return out;
}

#endif // LOGGING_H
