#ifndef TALORION_EVENT_MANAGER_HPP
#define TALORION_EVENT_MANAGER_HPP


#include <QObject>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
class QMutex;
class QFile;
QT_END_NAMESPACE

#include "tcpdriverdatatypes.hpp"

namespace talorion {

    class analogValue;

    class event_manager : public QObject
    {
        Q_OBJECT
    private:
        explicit event_manager(QObject *par = 0);
        ~event_manager(void);                                  // not desctructable
        Q_DISABLE_COPY(event_manager)

    public:
        static event_manager *get_instance();
        static void destroy();

    signals:
        void application_aboutToQuit();

        void act_value_changed(int);
        void set_value_changed(int);

        void change_act_value(int, double);
        void change_set_value(int, double);

        void send_custom_command(const QString&);
        void receivedCustomData(const QString &);

        void open_numeric_dialog();
        void open_string_dialog();
        void open_file_dialog();
        void open_info_dialog(const QString &msg);
        void open_plot_dialog();

        void dialog_finished();
        void dialog_finished(double val);
        void dialog_finished(QString val);


        void start_script(const QString &);
        void start_script_file(const QString & );
        void abort_script();
        void script_finished();

        void script_message(const QString&, const QString&);

        //qvmbackend
        void avSetChangeCommand(QByteArray);
        void newAnalogValue(int);

        //tcpDriver
        void receivedData(QVariantMap, tcpDriverDataTypes::dataType, int);
        void error(QString);

    private:
        static QAtomicPointer<event_manager> _instance;
        static QMutex _mutex;
    };

} // namespace talorion

#endif // TALORION_EVENT_MANAGER_HPP
