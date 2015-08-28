#include "console.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QThread>

#include "core/event_manager.hpp"

namespace talorion{
    Console::Console(QWidget *par) :
        QTextEdit(par)
    {
        qRegisterMetaType<QTextCursor>("QTextCursor");
        qRegisterMetaType<QTextCharFormat>("QTextCharFormat");

        setReadOnly(true);
        setFontFamily("monospace");
        //setStyleSheet("QTextEdit { background-color: 'black'; }");
        setStyleSheet("QTextEdit { font-family: Courier; font-size: 12pt; background-color: 'white'; }");
        setTextColor(QColor("white"));

        connect(event_manager::get_instance(),SIGNAL(script_message(QString,QString)),this,SLOT(write(QString,QString)));

    }

    void Console::write(QString string)
    {
        write(string, "black");
    }

    void Console::write(QString string, QString color)
    {
        QMetaObject::invokeMethod(this,"setTextColor",Qt::QueuedConnection,Q_ARG(QColor,QColor(color)));
        QMetaObject::invokeMethod(this,"append",Qt::QueuedConnection,Q_ARG(QString, string));
    }

    void Console::cls()
    {
        QMetaObject::invokeMethod(this,"clear",Qt::QueuedConnection);
    }
}
