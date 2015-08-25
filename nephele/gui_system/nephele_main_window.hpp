#ifndef NEPHELE_MAIN_WINDOW_HPP
#define NEPHELE_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QJsonDocument>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

//#include "tcpdriver.h"
//#include "qvmbackend.h"
#include "flowcontrollerview.hpp"

namespace talorion {
    class nephele_main_window : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit nephele_main_window(QWidget *par = 0);
        ~nephele_main_window();
        Q_DISABLE_COPY(nephele_main_window)

    private:
        //tcpDriver* dev1;
        //qvmbackend* dcs;
        QLineEdit* cmd;
        QLabel* response;
        QGridLayout* mainLayout;

    private slots:
        void displayCustomResponse(QByteArray res);
        void dispatchCommand();
        void addAV(analogValue* av);
    };
}

#endif // NEPHELE_MAIN_WINDOW_HPP
