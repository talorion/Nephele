#include "gui_system.hpp"


#include <QDebug>
#include <QInputDialog>

#include "nephele_main_window.hpp"
#include "core/event_manager.hpp"

namespace talorion{
    gui_system::gui_system(QObject *par) :
        QObject(par),
        abstract_system(),
        window(NULL)
    {

    }

    gui_system::~gui_system()
    {

    }

    void gui_system::slot_open_dialog()
    {
        if(!window){
            emit dialog_finished(NAN);
            return;
        }

        bool ok;
        double d = QInputDialog::getDouble(NULL, tr("QInputDialog::getDouble()"), tr("Amount:"), 37.56, -10000, 10000, 2, &ok);

        if (!ok)
           d=NAN;

        emit dialog_finished(d);
    }

    void talorion::gui_system::do_start_system()
    {
        int W = 250;
        int H = 250;

        int screenWidth;
        int screenHeight;

        int x, y;

        QPixmap pixmap(":/images/images/splash.png");
        QPixmap pixmapForSplash = pixmap.scaledToHeight(600);

        QSplashScreen splash(pixmapForSplash);
        splash.show();
        //QApplication::instance()->processEvents();

        QTime dieTime= QTime::currentTime().addSecs(5); //wait for 5 s
        while( QTime::currentTime() < dieTime )
            QApplication::instance()->processEvents();

        window= new nephele_main_window();
        QDesktopWidget *desktop = QApplication::desktop();

        screenWidth = desktop->width();
        screenHeight = desktop->height();

        x = (screenWidth - W) / 2;
        y = (screenHeight - H) / 2;

        window->resize(W, H);
        window->move( x, y );
        window->show();

        splash.finish(window);

        // connect dialoges
        connect(event_manager::get_instance(),SIGNAL(open_dialog()),this,SLOT(slot_open_dialog()));
        connect(this, SIGNAL(dialog_finished(double)),event_manager::get_instance(),SIGNAL(dialog_finished(double)));
        // connect dialoges
    }
}
