#include "gui_system.hpp"

#include "nephele_main_window.hpp"
#include <QDebug>

namespace talorion{
    gui_system::gui_system(QObject *par) : QObject(par), abstract_system()
    {

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

        nephele_main_window* window= new nephele_main_window();
        QDesktopWidget *desktop = QApplication::desktop();

        screenWidth = desktop->width();
        screenHeight = desktop->height();

        x = (screenWidth - W) / 2;
        y = (screenHeight - H) / 2;

        window->resize(W, H);
        window->move( x, y );
        window->show();

        splash.finish(window);
    }
}
