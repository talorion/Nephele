#ifndef SCRIPTING_WORKER_H
#define SCRIPTING_WORKER_H

#include <QFile>
#include <QObject>
#include <QScriptEngine>

#include "script_handler/script_set_handler.hpp"
#include "script_handler/script_dialoges_handler.hpp"
#include "script_handler/script_act_handler.hpp"
#include "script_handler/script_util_handler.hpp"
#include "script_handler/script_log_handler.hpp"

namespace talorion {

    class scripting_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit scripting_worker(QObject *par = 0);
        ~scripting_worker();
        Q_DISABLE_COPY(scripting_worker)

        void initialize();


    signals:
        void script_finished();

    private slots:
        void slot_newAnalogValue(int entity);
        void slot_act_value_changed(int entity);
        void slot_set_value_changed(int entity);
        void slot_start_script(const QString &script);
        void slot_start_script_file(const QString &script);
        void slot_abort_script();

    private:
        QScriptEngine m_script_engine;
        script_set_handler m_set_hdl;
        script_act_handler m_act_hdl;
        script_dialoges_handler m_diag_hdl;
        script_util_handler m_util_hdl;
        script_log_handler m_log_hdl;

        QScriptValue m_actHdl;
        QScriptValue m_setHdl;
        QScriptValue m_diagHdl;
        QScriptValue m_utilhdl;
        QScriptValue m_loghdl;

    };
}

#endif // SCRIPTING_WORKER_H
