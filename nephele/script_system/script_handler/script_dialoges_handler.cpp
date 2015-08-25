#include "script_dialoges_handler.h"

#include <QWidget>
#include <QtWidgets>
#include <QInputDialog>

#include "core/event_manager.hpp"

namespace talorion {

    script_dialoges_handler::script_dialoges_handler(QObject *par) :
        QObject(par),
        m_tmp(NAN)
    {
        connect(event_manager::get_instance(),SIGNAL(dialog_finished(double)),this,SLOT(slot_dialog_finished(double)));
        connect(this,SIGNAL(open_dialog()),event_manager::get_instance(),SIGNAL(open_dialog()));
    }

    script_dialoges_handler::~script_dialoges_handler()
    {

    }

    double script_dialoges_handler::setDouble()
    {
        qDebug()<<"script_dialoges_handler::setDouble "<<QThread::currentThreadId();

        m_tmp=NAN;

        QEventLoop loop;
        connect(this,SIGNAL(dialog_finished()),&loop, SLOT(quit()));
        emit open_dialog();
        loop.exec();

        return m_tmp;
    }

    void script_dialoges_handler::slot_dialog_finished(double val)
    {
        m_tmp=val;
        emit dialog_finished();
    }
}
