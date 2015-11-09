#define _USE_MATH_DEFINES       //this must be first due some bug with windows mingw
#include <cmath>                //i don't know why but it works

#include "octave_system.hpp"

#include <oct.h>
#include <parse.h>
#include <octave.h>
#include <toplev.h>

#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QTimer>
#include <QDebug>

namespace talorion {

    octave_system::octave_system(QObject *par) :
        QThread(par),
        abstract_system("{ddc5b569-12b4-4754-9fa7-444660f6f82e}")
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
        //        QTimer::singleShot(0,this,SLOT(quit()));

        //        wait();
        //clean_up_and_exit (0);
        //exit(0);
    }

    void octave_system::run()
    {
        //setlocale(LC_ALL, "en_US.UTF-8");

        //        --built-in-docstrings-file FILE Use docs for built-ins from FILE.
        //        --debug, -d             Enter parser debugging mode.
        //        --debug-jit             Enable JIT compiler debugging/tracing.
        //        --doc-cache-file FILE   Use doc cache file FILE.
        //        --echo-commands, -x     Echo commands as they are executed.
        //        --eval CODE             Evaluate CODE.  Exit when done unless --persist.
        //        --exec-path PATH        Set path for executing subprograms.
        //        --force-gui             Force graphical user interface to start.
        //        --help, -h,             Print short help message and exit.
        //        --image-path PATH       Add PATH to head of image search path.
        //        --info-file FILE        Use top-level info file FILE.
        //        --info-program PROGRAM  Use PROGRAM for reading info files.
        //        --interactive, -i       Force interactive behavior.
        //        --jit-compiler          Enable the JIT compiler.
        //        --line-editing          Force readline use for command-line editing.
        //        --no-gui                Disable the graphical user interface.
        //        --no-history, -H        Don't save commands to the history list
        //        --no-init-file          Don't read the ~/.octaverc or .octaverc files.
        //        --no-init-path          Don't initialize function search path.
        //        --no-line-editing       Don't use readline for command-line editing.
        //        --no-site-file          Don't read the site-wide octaverc file.
        //        --no-window-system, -W  Disable window system, including graphics.
        //        --norc, -f              Don't read any initialization files.
        //        --path PATH, -p PATH    Add PATH to head of function search path.
        //        --persist               Go interactive after --eval or reading from FILE.
        //        --silent, --quiet, -q   Don't print message at startup.
        //        --texi-macros-file FILE Use Texinfo macros in FILE for makeinfo command.
        //        --traditional           Set variables for closer MATLAB compatibility.
        //        --verbose, -V           Enable verbose output in some cases.
        //        --version, -v           Print version number and exit.

//        QStringList oct_args;
//        oct_args << QCoreApplication::arguments().at(0); //is the name by which the program was called
//        oct_args << "--silent";            // Don't print message at startup.
//        oct_args << "--no-line-editing";   // Don't use readline for command-line editing.
//        oct_args << "--no-gui";            // Disable the graphical user interface.
//        oct_args << "--no-window-system";  // Disable window system, including graphics.
//        //oct_args << "--norc";               // Don't read any initialization files.
//        oct_args << "--no-history";               // Don't save commands to the history list
//        //oct_args << "--no-init-path";               // Don't initialize function search path.
//        //oct_args << "--interactive";  // Disable window system, including graphics.
//        //oct_args << "--verbose";           // Enable verbose output in some cases.


//        int argc = oct_args.length();
//        string_vector argv (argc);
//        for(int i=0; i<argc; i++){
//            argv[i] = oct_args.at(i).toStdString();

            //            c[i] = new char[s.toLocal8Bit().size()+1];
            //            strcpy(c[i],s.toLocal8Bit().constData());
            //            c[strlen(s[i])] = '\0';
        //}

        //int embedded = 1;

        //octave_main (argc, (char **) argv.c_str_vec(), embedded);

        //octave_value_list  version=feval("version",octave_value_list( ) ,1 );
        //std::cout<<"using octave version: "<< version(0).string_value()<<std::endl;

        //clean_up_and_exit (0);

        exec();

        //        try{
        //octave_quit();
        //feval("quit",octave_value_list( ) ,0);
        //std::cout<<"using octave version: "<< oct_q(0).string_value()<<std::endl;

        //clean_up_and_exit (0,true);
        //sleep(1000);
        //        } catch(const std::runtime_error &ex){
        //            qDebug()<<ex.what();
        //        } catch (const std::exception& ex) {
        //            qDebug()<<ex.what();
        //            //    } catch (const std::string& ex) {
        //            //        qDebug()<<ex.what();
        //            //        ret = -4;
        //        } catch (...) {
        //            qDebug()<<"OMG! an unexpected exception has been caught";
        //        }
    }

    void octave_system::do_start_system()
    {
        this->start();
    }

    abstract_configuration_widget *octave_system::do_get_configuration_widget()
    {
        return NULL;
    }

} // namespace talorion

