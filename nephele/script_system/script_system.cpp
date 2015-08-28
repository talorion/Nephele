#include "script_system.hpp"

#include <QDebug>
#include "scripting_worker.hpp"
#include "core/event_manager.hpp"

namespace talorion {

    script_system::script_system(QObject *par) :
        QThread(par)
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    script_system::~script_system()
    {

    }


    void script_system::run()
    {
        scripting_worker worker;
        worker.initialize();
        //    connect(this,SIGNAL(start_script(QString)),&worker,SLOT(start_script(QString)));
        //    connect(this,SIGNAL(set_act(QString,double)),&worker,SLOT(slot_set_act(QString,double)));
        //    connect(this,SIGNAL(set_set(QString,double)),&worker,SLOT(slot_set_set(QString,double)));
        //    //connect(this,SIGNAL(set_act(QString,double)),&worker,SIGNAL(set_act(QString,double)));
        //    connect(&worker, SIGNAL(value_changed(QString,double)),this,SIGNAL(value_changed(QString,double)));
        //    connect(this,SIGNAL(dialog_finished(double)),&worker,SIGNAL(dialog_finished(double)));
        //    connect(&worker,SIGNAL(open_dialog()),this,SIGNAL(open_dialog()));

        exec();
    }

    void script_system::do_start_system()
    {
//        QThread* thread = new QThread;
//        Worker* worker = new Worker();
//        worker->moveToThread(thread);
//        connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
//        connect(thread, SIGNAL(started()), worker, SLOT(process()));
//        connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
//        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
//        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//        thread->start();

        this->start();

    }
}
