#ifndef TALORION_EVENT_MANAGER_HPP
#define TALORION_EVENT_MANAGER_HPP

#include <QFile>
#include <QObject>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
class QMutex;
QT_END_NAMESPACE

#include "tcpdriverdatatypes.hpp"
#include "analogvalue.hpp"

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

        void act_value_changed(QString, double);
        void set_value_changed(QString, double);

        void open_dialog();
        void dialog_finished( double);

        void start_script(const QString &);
        void start_script_file(const QString & );
        void script_finished();

        void script_message(const QString& string, const QString& color);

        //qvmbackend
        void avSetChangeCommand(QByteArray);
        void newAnalogValue(analogValue*);

        //tcpDriver
        void receivedData(QVariantMap, tcpDriverDataTypes::dataType);
        void error(QString);

    private:
        static QAtomicPointer<event_manager> _instance;
        static QMutex _mutex;
    };

} // namespace talorion

#endif // TALORION_EVENT_MANAGER_HPP
