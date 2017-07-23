#include "script_dialoges_handler.hpp"

#include <QWidget>
#include <QtWidgets>
#include <QInputDialog>

#include "core/EventManager.hpp"

namespace talorion {

    script_dialoges_handler::script_dialoges_handler(QObject *par) :
        abstract_scriptable_object("gui", par),
        m_tmp_num(0),
        m_tmp_str()
    {
        connect(this,SIGNAL(open_numeric_dialog()),EventManager::get_instance(),SIGNAL(open_numeric_dialog()));
        connect(this,SIGNAL(open_string_dialog()),EventManager::get_instance(),SIGNAL(open_string_dialog()));
        connect(this,SIGNAL(open_file_dialog()),EventManager::get_instance(),SIGNAL(open_file_dialog()));
        connect(this,SIGNAL(open_info_dialog(QString)),EventManager::get_instance(),SIGNAL(open_info_dialog(QString)));
        connect(this,SIGNAL(open_plot_dialog()),EventManager::get_instance(),SIGNAL(open_plot_dialog()));

        connect(EventManager::get_instance(), SIGNAL(dialog_finished(double)),this,SLOT(slot_dialog_finished(double)));
        connect(EventManager::get_instance(), SIGNAL(dialog_finished(QString)),this,SLOT(slot_dialog_finished(QString)));
        connect(EventManager::get_instance(), SIGNAL(dialog_finished()),this,SIGNAL(dialog_finished()));

        connect(EventManager::get_instance(), SIGNAL(application_aboutToQuit()),this,SIGNAL(dialog_finished()));

    }

    script_dialoges_handler::~script_dialoges_handler()
    {

    }

    double script_dialoges_handler::showNumericDialog()
    {
        m_tmp_num=0;

        QEventLoop loop;
        connect(this,SIGNAL(dialog_finished()),&loop, SLOT(quit()));
        emit open_numeric_dialog();
        loop.exec();

        return m_tmp_num;
    }

    QString script_dialoges_handler::showStringDialog()
    {
        m_tmp_str="";

        QEventLoop loop;
        connect(this,SIGNAL(dialog_finished()),&loop, SLOT(quit()));
        connect(this,SIGNAL(script_finished()),&loop, SLOT(quit()));
        emit open_string_dialog();
        loop.exec();

        return m_tmp_str;
    }

    QString script_dialoges_handler::showFileDialog()
    {
        m_tmp_str="";

        QEventLoop loop;
        connect(this,SIGNAL(dialog_finished()),&loop, SLOT(quit()));
        emit open_file_dialog();
        loop.exec();

        return m_tmp_str;
    }

    void script_dialoges_handler::showInfoDialog(QString msg)
    {

        QEventLoop loop;
        connect(this,SIGNAL(dialog_finished()),&loop, SLOT(quit()));
        emit open_info_dialog(msg);
        loop.exec();

    }

    void script_dialoges_handler::showPlotDialog()
    {
        //QEventLoop loop;
        //connect(this,SIGNAL(dialog_finished()),&loop, SLOT(quit()));
        emit open_plot_dialog();
        //loop.exec();
    }

    void script_dialoges_handler::slot_dialog_finished(double val)
    {
        m_tmp_num=val;
        emit dialog_finished();
    }

    void script_dialoges_handler::slot_dialog_finished(QString val)
    {
        m_tmp_str = val;
        emit dialog_finished();
    }
}
