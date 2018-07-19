#include "scripting_worker.hpp"


#include <QDebug>
#include <QThread>
#include <QAction>

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"
#include "core/abstract_scriptable_object.hpp"


namespace talorion {
    scripting_worker::scripting_worker(QObject *par) :
        QObject(par),
        m_script_engine(),
        //m_debugger(NULL),
        m_set_hdl(NULL),
        m_act_hdl(NULL),
        m_diag_hdl(NULL),
        m_util_hdl(NULL),
        m_log_hdl(NULL),
        m_daq_hdl(NULL),
        m_scrp_usr_dta(NULL),
        mscrpt_lod_hdl(NULL),
        //m_actHdl(),
        //m_setHdl(),
        //m_diagHdl(),
        //m_utilhdl(),
        //m_loghdl(),
        //m_daqhdl(),
        script_values()
    {
        qScriptRegisterSequenceMetaType<QVector<int> >(&m_script_engine);
        qScriptRegisterSequenceMetaType<QVector<float> >(&m_script_engine);
        qScriptRegisterSequenceMetaType<QStringList>(&m_script_engine);

    }

    scripting_worker::~scripting_worker(){
        if(m_set_hdl)
            delete m_set_hdl;
        if(m_act_hdl)
            delete m_act_hdl;
        if(m_diag_hdl)
            delete m_diag_hdl;
        if(m_util_hdl)
            delete m_util_hdl;
        if(m_log_hdl)
            delete m_log_hdl;
        if(m_daq_hdl)
            delete m_daq_hdl;
        if(m_scrp_usr_dta)
            delete m_scrp_usr_dta;
        if(mscrpt_lod_hdl)
          delete mscrpt_lod_hdl;


        foreach (QScriptValue* var, script_values) {
            if(var)
                delete var;
        }
    }

    void scripting_worker::initialize()
    {
        EntityManager::get_instance()->createQtScriptEngine("Qt Script Engine", &m_script_engine);

        connect(EventManager::get_instance(),SIGNAL(start_script(QString)),this,SLOT(slot_start_script(QString)));
        connect(EventManager::get_instance(),SIGNAL(start_script_file(QString)),this,SLOT(slot_start_script_file(QString)));
        connect(EventManager::get_instance(),SIGNAL(debug_script_file(QString)),this,SLOT(debug_script_file(QString)));
        //connect(event_manager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(slot_act_value_changed(int)));
        //connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(slot_set_value_changed(int)));
        //connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this,SLOT(slot_newAnalogValue(int)));
        connect(EventManager::get_instance(),SIGNAL(abort_script()),this,SLOT(slot_abort_script()));
        connect(this,SIGNAL(script_finished()),EventManager::get_instance(),SIGNAL(script_finished()));

        foreach (int ent, EntityManager::get_instance()->get_all_ScriptableObjects()) {
            slot_register_scritable_component(ent);
        }

        //connect(event_manager::get_instance(),SIGNAL(register_scritable_component(int)),this,SLOT(slot_register_scritable_component(int)));
        //connect(event_manager::get_instance(),SIGNAL(unregister_scritable_component(int)),this,SLOT(slot_unregister_scritable_component(int)));
        connect(EventManager::get_instance(),SIGNAL(newScriptableObject(int)),this, SLOT(slot_register_scritable_component(int)));


        m_set_hdl = new script_set_handler();
        EntityManager::get_instance()->createScriptableObject(m_set_hdl->script_name(), m_set_hdl);

        m_act_hdl = new script_act_handler();
        EntityManager::get_instance()->createScriptableObject(m_act_hdl->script_name(), m_act_hdl);

        m_diag_hdl = new script_dialoges_handler();
        connect(this, SIGNAL(abort_all_dialoges()),m_diag_hdl,SIGNAL(dialog_finished()));
        EntityManager::get_instance()->createScriptableObject(m_diag_hdl->script_name(), m_diag_hdl);

        m_util_hdl = new script_util_handler();
        //connect(this, SIGNAL(abort_all_dialoges()),m_util_hdl,SIGNAL(dialog_finished()));
        EntityManager::get_instance()->createScriptableObject(m_util_hdl->script_name(), m_util_hdl);
        m_log_hdl = new script_log_handler();
        EntityManager::get_instance()->createScriptableObject(m_log_hdl->script_name(), m_log_hdl);

        m_daq_hdl = new script_daq_handler();
        EntityManager::get_instance()->createScriptableObject(m_daq_hdl->script_name(), m_daq_hdl);

        m_scrp_usr_dta = new scriptable_usr_data_handler();
        EntityManager::get_instance()->createScriptableObject(m_scrp_usr_dta->script_name(), m_scrp_usr_dta);

        mscrpt_lod_hdl = new script_load_handler(&m_script_engine);
        EntityManager::get_instance()->createScriptableObject(mscrpt_lod_hdl->script_name(), mscrpt_lod_hdl);

        //scriptable_usr_data_handler* m_scrp_usr_dta;

        //m_script_engine = new QScriptEngine();

        //hdl = new script_set_handler();
        //connect(&m_set_hdl,SIGNAL(pcu_value(QString,double)),this,SIGNAL(value_changed(QString,double)));
        //connect(&m_set_hdl,SIGNAL(val(QString,double)),event_manager::get_instance(),SIGNAL(set_value_changed(QString,double)));
        //m_setHdl = m_script_engine.newQObject(&m_set_hdl, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);
        //        m_setHdl = m_script_engine.newQObject(&m_set_hdl);
        //m_script_engine.globalObject().setProperty("set", m_setHdl);

        //connect(this,SIGNAL(set_act(QString,double)),&m_act_hdl,SLOT(slot_set_act(QString,double)));
        //m_actHdl = m_script_engine.newQObject(&m_act_hdl, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);
        //        m_actHdl = m_script_engine.newQObject(&m_act_hdl);
        // m_script_engine.globalObject().setProperty("act", m_actHdl);

        //diag = new script_dialoges_handler();
        //connect(&m_diag_hdl, SIGNAL(open_dialog()),this,SIGNAL(open_dialog()));
        //connect(this,SIGNAL(dialog_finished(double)),&m_diag_hdl,SLOT(slot_dialog_finished(double)));
        //m_diagHdl = m_script_engine.newQObject(&m_diag_hdl, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);
        //        m_diagHdl = m_script_engine.newQObject(&m_diag_hdl);
        //m_script_engine.globalObject().setProperty("gui", m_diagHdl);
        //connect(this, SIGNAL(abort_all_dialoges()),&m_diag_hdl,SIGNAL(dialog_finished()));

        //m_utilhdl= m_script_engine.newQObject(&m_util_hdl, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);
        //        m_utilhdl= m_script_engine.newQObject(&m_util_hdl);
        // m_script_engine.globalObject().setProperty("util", m_utilhdl);

        //m_loghdl = m_script_engine.newQObject(&m_log_hdl, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);
        //        m_loghdl = m_script_engine.newQObject(&m_log_hdl);
        //m_script_engine.globalObject().setProperty("console", m_loghdl);

        //m_daqhdl = m_script_engine.newQObject(&m_daq_hdl, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);
        //        m_daqhdl = m_script_engine.newQObject(&m_daq_hdl);
        //m_script_engine.globalObject().setProperty("daq", m_daqhdl);



    }

    //    void scripting_worker::slot_newAnalogValue(int entity)
    //    {
    //        slot_act_value_changed(entity);
    //        slot_set_value_changed(entity);
    //    }

    //    void scripting_worker::slot_act_value_changed(int entity)
    //    {
    //        QString nme = entity_manager::get_instance()->get_name_component(entity);
    //        double val = entity_manager::get_instance()->get_analogActValue_component(entity);
    //        m_actHdl.setProperty(nme, val,QScriptValue::ReadOnly);
    //    }

    //    void scripting_worker::slot_set_value_changed(int entity)
    //    {
    //        QString nme = entity_manager::get_instance()->get_name_component(entity);
    //        double val = entity_manager::get_instance()->get_analogSetValue_component(entity);
    //        m_setHdl.setProperty(nme, val, QScriptValue::ReadOnly);
    //    }

    void scripting_worker::slot_start_script(const QString &script, bool debug)
    {
        Q_UNUSED(debug);

        if(m_script_engine.isEvaluating()){
            m_log_hdl->log_fatal("another script is already running");
            qDebug()<<"another script is already running";
            return;
        }

        if (!m_script_engine.canEvaluate(script)){
            m_log_hdl->log_fatal("cannot evaluate script");
            qDebug()<<"cannot evaluate script";
            return;
        }

        //if(m_debugger!=NULL && !debug){
        //    m_debugger->detach();
        //}

        m_log_hdl->log_info("Starting script");
        QScriptValue ret = m_script_engine.evaluate(script).toNumber();
        m_log_hdl->log_info("script returned " + ret.toString());

        if (m_script_engine.hasUncaughtException()){

            m_log_hdl->log_fatal("Script had Uncaught Exceptions at line: "+ QString::number(m_script_engine.uncaughtExceptionLineNumber())+" EXception:" +m_script_engine.uncaughtException().toString());
            qDebug()<<"Script had Uncaught Exceptions at line: "+ QString::number(m_script_engine.uncaughtExceptionLineNumber())+" EXception:" +m_script_engine.uncaughtException().toString();
        }else{
            m_log_hdl->log_info("script finished successfully");
            qDebug()<<"script finished successfully";
          }

        emit script_finished();
    }

    void scripting_worker::slot_start_script_file(const QString &script, bool debug)
    {
        QFile script_file(script);

        if(!script_file.open(QIODevice::ReadOnly)) {
            m_log_hdl->log_error("error:" + script_file.errorString());
            qDebug()<<"error:" + script_file.errorString();
        }
        QTextStream in(&script_file);
        QString sc;
        while(!in.atEnd()) {
            sc += in.readLine();
            sc += "\n";
        }
        script_file.close();

        m_log_hdl->log_info("Starting "+script);
        qDebug()<<"Starting "+script;

        slot_start_script(sc,debug);
    }

    void scripting_worker::debug_script_file(const QString &script)
    {
        //if(!m_debugger){
        //m_debugger = new QScriptEngineDebugger();
        //m_debugger->attachTo(&m_script_engine);
        //m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
        //}
        slot_start_script_file(script, true);

    }

    void scripting_worker::slot_abort_script()
    {
        if(m_script_engine.isEvaluating()){

            emit abort_all_dialoges();

            m_script_engine.abortEvaluation();
            m_log_hdl->log_fatal("script aborted");
            qDebug()<<"script aborted";
            return;
        }

        emit script_finished();
    }

    void scripting_worker::slot_register_scritable_component(int ent)
    {
        abstract_scriptable_object* obj = EntityManager::get_instance()->get_scriptable_object_component(ent);
        if(!obj)
            return;
        QString script_name = obj->script_name();

        QScriptValue tmp = m_script_engine.newQObject(obj, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);

        QScriptValue* ptmp = new QScriptValue(tmp);
        //QScriptValue* ptmp = new QScriptValue(tmp);
        obj->setScrip_value(ptmp);
        m_script_engine.globalObject().setProperty(script_name, *ptmp);
        script_values.append(ptmp);
        //qDebug()<< "slot_register_scritable_component";
    }

    //    void scripting_worker::slot_unregister_scritable_component(int)
    //    {

    //    }
}
