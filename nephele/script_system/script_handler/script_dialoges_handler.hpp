#ifndef SCRIPT_DIALOGES_HANDLER_H
#define SCRIPT_DIALOGES_HANDLER_H

#include <QObject>

namespace talorion {

    class script_dialoges_handler : public QObject
    {
        Q_OBJECT
    public:
        explicit script_dialoges_handler(QObject *par = 0);
        ~script_dialoges_handler();
        Q_DISABLE_COPY(script_dialoges_handler)

    public:
        Q_INVOKABLE double setDouble();

    signals:
        void open_dialog();
        void dialog_finished();

    private slots:
        void slot_dialog_finished(double val);

    private:
        double m_tmp;

    };
}

#endif // SCRIPT_DIALOGES_HANDLER_H
