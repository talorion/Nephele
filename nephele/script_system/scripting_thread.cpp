#include "scripting_thread.h"

#include <QDebug>
#include "scripting_worker.h"
#include "core/event_manager.hpp"

namespace talorion {

    scripting_thread::scripting_thread(QObject *par) :
        QThread(par)
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    scripting_thread::~scripting_thread()
    {

    }


    void scripting_thread::run()
    {
        qDebug()<<"scripting_thread::run "<<QThread::currentThreadId();

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
}
