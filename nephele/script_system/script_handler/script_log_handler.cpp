#include "script_log_handler.hpp"

#include <QDateTime>

#include "event_manager/event_manager_locator.hpp"

namespace talorion{
    script_log_handler::script_log_handler(QObject *par) :
        abstract_scriptable_object("console", par)
    {
        connect(this,SIGNAL(script_message(QString,QString)),event_manager_locator::get_instance(),SIGNAL(script_message(QString,QString)));
    }

    script_log_handler::~script_log_handler()
    {

    }

    void script_log_handler::write(const QString &string, const QString &color)
    {
        emit script_message(string, color);
    }

    void script_log_handler::write(const QString &string)
    {
        write(string, "black");
    }

    void script_log_handler::log(script_log_handler::severity sev, const QString &msg, const QString &color)
    {
        QString tmp = time_to_string()+" "+severity_to_string(sev)+" "+msg;

        write(tmp, color);
    }

    void script_log_handler::log_info(const QString& msg)
    {
        log(severity::INFO,msg, "black");
    }

    void script_log_handler::log_debug(const QString &msg)
    {
        log(severity::DEBUG,msg, "darkGray");
    }

    void script_log_handler::log_warning(const QString &msg)
    {
        //Qt::darkBlue;
        log(severity::WARNING,msg, "magenta");
    }

    void script_log_handler::log_error(const QString &msg)
    {
        log(severity::ERROR,msg, "red");
    }

    void script_log_handler::log_fatal(const QString &msg)
    {
        log(severity::FATAL,msg, "darkRed");
    }

    QString script_log_handler::time_to_string()
    {
        // [YYYY-MM-DD hh:mm:ss.sss] [DEBUG]
        return QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]");
    }

    QString script_log_handler::severity_to_string(script_log_handler::severity sev)
    {
        switch(sev){
        case UNKNOWN:   return "[UNKNOWN]";
        case INFO:      return "[INFO]";
        case DEBUG:     return "[DEBUG]";
        case WARNING:   return "[WARNING]";
        case ERROR:     return "[ERROR]";
        case FATAL:     return "[FATAL]";
        default:        return "";
        }
        return "";
    }
}

