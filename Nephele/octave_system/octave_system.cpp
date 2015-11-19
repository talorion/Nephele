//#define _USE_MATH_DEFINES       //this must be first due some bug with windows mingw
//#include <cmath>                //i don't know why but it works

#include "octave_system.hpp"

//#include <oct.h>
//#include <parse.h>
//#include <octave.h>
//#include <toplev.h>

//#include <QCoreApplication>
//#include <QStringList>
//#include <QString>
//#include <QTimer>
//#include <QDebug>

#include "octave_worker.hpp"

namespace talorion {

    octave_system::octave_system(QObject *par) :
        QThread(par),
        abstract_system("{ddc5b569-12b4-4754-9fa7-444660f6f82e}"),
        config_wdg(NULL)
    {

    }

    octave_system::~octave_system()
    {
        //clean_up_and_exit (0);
        //exit(0);
    }

    void octave_system::do_init_system()
    {
        connect(event_manager_locator::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    void octave_system::do_dispose_system()
    {

    }

    void octave_system::run()
    {

        //octave_worker wrk;
        //wrk.initialize();

        exec();

        //wrk.dispose();

    }

    void octave_system::do_start_system()
    {
        this->start();
    }

    abstract_configuration_widget *octave_system::do_get_configuration_widget()
    {
        return config_wdg;
    }

} // namespace talorion

