#include "scripting_worker.hpp"

#include <QDebug>
#include <QThread>

#include "core/event_manager.hpp"

namespace talorion {

    scripting_worker::scripting_worker(QObject *par) :
        QObject(par),
        m_script_engine(),
        m_set_hdl(),
        m_act_hdl(),
        m_diag_hdl(),
        m_util_hdl(),
        m_actHdl(),
        m_setHdl(),
        m_diagHdl(),
        m_utilhdl()
    {
    }

    scripting_worker::~scripting_worker(){

    }

    void scripting_worker::initialize()
    {
        qDebug()<<"scripting_worker::initialize "<<QThread::currentThreadId();

        connect(event_manager::get_instance(),SIGNAL(start_script(QString)),this,SLOT(slot_start_script(QString)));
        connect(event_manager::get_instance(),SIGNAL(act_value_changed(QString,double)),this,SLOT(slot_act_value_changed(QString,double)));
        connect(event_manager::get_instance(),SIGNAL(set_value_changed(QString,double)),this,SLOT(slot_set_value_changed(QString,double)));

        //m_script_engine = new QScriptEngine();

        //hdl = new script_set_handler();
        //connect(&m_set_hdl,SIGNAL(pcu_value(QString,double)),this,SIGNAL(value_changed(QString,double)));
        connect(&m_set_hdl,SIGNAL(val(QString,double)),event_manager::get_instance(),SIGNAL(set_value_changed(QString,double)));
        m_setHdl = m_script_engine.newQObject(&m_set_hdl);
        m_script_engine.globalObject().setProperty("set", m_setHdl);

        //connect(this,SIGNAL(set_act(QString,double)),&m_act_hdl,SLOT(slot_set_act(QString,double)));
        m_actHdl = m_script_engine.newQObject(&m_act_hdl);
        m_script_engine.globalObject().setProperty("act", m_actHdl);

        //diag = new script_dialoges_handler();
        //connect(&m_diag_hdl, SIGNAL(open_dialog()),this,SIGNAL(open_dialog()));
        //connect(this,SIGNAL(dialog_finished(double)),&m_diag_hdl,SLOT(slot_dialog_finished(double)));
        m_diagHdl = m_script_engine.newQObject(&m_diag_hdl);
        m_script_engine.globalObject().setProperty("gui", m_diagHdl);

        m_utilhdl= m_script_engine.newQObject(&m_util_hdl);
        m_script_engine.globalObject().setProperty("util", m_utilhdl);
    }

    void scripting_worker::slot_act_value_changed(const QString &name, double value)
    {
        m_actHdl.setProperty(name, value);
    }

    void scripting_worker::slot_set_value_changed(const QString &name, double value)
    {
        m_setHdl.setProperty(name, value);
    }

    void scripting_worker::slot_start_script(const QString &script)
    {
        qDebug()<<"scripting_worker::start_script "<<QThread::currentThreadId();

        if(m_script_engine.isEvaluating())
            return;

        qDebug() << "start_script is:" << m_script_engine.evaluate(script).toNumber();

        emit script_finished();
    }
}
