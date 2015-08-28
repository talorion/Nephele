#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QTextEdit>

Q_DECLARE_METATYPE(QTextCursor)
Q_DECLARE_METATYPE(QTextCharFormat)

namespace talorion{
    class Console : public QTextEdit
    {
        Q_OBJECT

    public:
        explicit Console(QWidget *par = 0);
    signals:

    public slots:
        void write(QString string);
        void write(QString string, QString color);
        void cls();

    };
}

#endif // CONSOLE_H
