#define _USE_MATH_DEFINES       //this must be first due some bug with windows mingw
#include <cmath>                //i don't know why but it works

#include "octave_worker.hpp"

#include <oct.h>
#include <parse.h>
#include <octave.h>
#include <toplev.h>

#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QThread>

namespace talorion {

    octave_worker::octave_worker(QObject *par) : QObject(par)
    {

    }

    octave_worker::~octave_worker()
    {

    }

    void octave_worker::initialize()
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
        //        oct_args << QCoreApplication::arguments().at(0);    //is the name by which the program was called
        //        oct_args << "--silent";                               //Don't print message at startup.

        //        int argc = oct_args.length();
        //        string_vector argv (argc);
        //        for(int i=0; i<argc; i++){
        //            argv[i] = oct_args.at(i).toStdString();
        //        }

        //        int embedded = 1;

        //either does not close deadlock or SEGFAULT
        //octave_main (argc, (char **) argv.c_str_vec(), embedded);

        //QThread::currentThread()->msleep(5000);


        //        octave_value_list  version=feval("version",octave_value_list( ) ,1 );
        //        qDebug()<<"using octave version: " << QString::fromStdString(version(0).string_value());


        //        octave_idx_type n = 2;
        //        octave_value_list in;

        //        for (octave_idx_type i = 0; i < n; i++)
        //            in(i) = octave_value (5 * (i + 2));

        //        octave_value_list out = feval ("gcd", in, 1);

        //        if (! error_state && out.length () > 0)
        //            qDebug() << "GCD of ["
        //                     << in(0).int_value ()
        //                     << ", "
        //                     << in(1).int_value ()
        //                     << "] is "
        //                     << out(0).int_value ();
        //        else
        //            qDebug() << "invalid";

    }

    void octave_worker::dispose()
    {
        //either does not close deadlock or SEGFAULT
        //clean_up_and_exit (0, true);
    }

    void octave_worker::slot_eval_function(const QString &script, bool debug)
    {
        Q_UNUSED(script);
        Q_UNUSED(debug);

    }

    void octave_worker::slot_eval_file(const QString &script, bool debug)
    {
        Q_UNUSED(script);
        Q_UNUSED(debug);
    }

    void octave_worker::debug_script_file(const QString &script)
    {
        Q_UNUSED(script);
    }

    void octave_worker::slot_abort_script()
    {

    }

} // namespace talorion

