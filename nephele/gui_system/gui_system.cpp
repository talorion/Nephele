#include "gui_system.hpp"


#include <QDebug>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QSplashScreen>
#include <QTime>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#include "nephele_main_window.hpp"
#include "core/event_manager.hpp"

namespace talorion{
    gui_system::gui_system(QObject *par) :
        QObject(par),
        abstract_system("{93647299-a839-4cb6-ada7-7dabf01f297d}"),
        window(NULL)
    {

    }

    gui_system::~gui_system()
    {

    }

    void gui_system::slot_open_numeric_dialog()
    {

        bool ok;
        double d = QInputDialog::getDouble(NULL, tr("QInputDialog::getDouble()"), tr("Amount:"), 37.56, -10000, 10000, 2, &ok);

        if (!ok)
            d=NAN;

        emit dialog_finished(d);
    }

    void gui_system::slot_open_string_dialog()
    {
        bool ok;
        QString d = QInputDialog::getText(NULL, tr("QInputDialog::getText()"),tr("User name:"), QLineEdit::Normal,QDir::home().dirName(), &ok);

        if (!ok)
            d="";

        emit dialog_finished(d);
    }

    void gui_system::slot_open_file_dialog()
    {

        QString d = QFileDialog::getOpenFileName(NULL, tr("Open File"), "", "any file (*.*)");

        emit dialog_finished(d);
    }

    void gui_system::slot_open_info_dialog(const QString& msg)
    {
        QMessageBox::information(NULL, tr("Info"),msg);
        emit dialog_finished();
    }

    void gui_system::slot_open_plot_dialog()
    {
        QMessageBox::critical(NULL, tr("Plot Dilaog"),tr("Plot Dialog not implemented"));
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
        connect(event_manager::get_instance(),SIGNAL(open_numeric_dialog()),this,SLOT(slot_open_numeric_dialog()));
        connect(event_manager::get_instance(),SIGNAL(open_string_dialog()),this,SLOT(slot_open_string_dialog()));
        connect(event_manager::get_instance(),SIGNAL(open_file_dialog()),this,SLOT(slot_open_file_dialog()));
        connect(event_manager::get_instance(),SIGNAL(open_info_dialog(QString)),this,SLOT(slot_open_info_dialog(QString)));
        connect(event_manager::get_instance(),SIGNAL(open_plot_dialog()),this,SLOT(slot_open_plot_dialog()));

        connect(this, SIGNAL(dialog_finished(double)),event_manager::get_instance(),SIGNAL(dialog_finished(double)));
        connect(this, SIGNAL(dialog_finished(QString)),event_manager::get_instance(),SIGNAL(dialog_finished(QString)));
        connect(this, SIGNAL(dialog_finished()),event_manager::get_instance(),SIGNAL(dialog_finished()));

        // connect dialoges
    }
}
