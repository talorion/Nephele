#ifndef TALORION_EVENT_MANAGER_HPP
#define TALORION_EVENT_MANAGER_HPP


#include <QObject>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
class QMutex;
class QFile;
QT_END_NAMESPACE

//#include "tcpdriverdatatypes.hpp"

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
        void newSystem(int);
        void newTcpBox(int);
        void newQtScriptEngine(int);
        void newTofDaqDll(int);

        void application_aboutToQuit();

        void connect_tcp_box(int );
        void disconnect_tcp_box(int );

        void connection_state_component_changed(int );

        void analogAct_component_changed(int);
        void analogSet_component_changed(int);       
        void digitalSet_component_changed(int );
        void digitalAct_component_changed(int );

        void data_aquistion_dll_component_changed(int );
        void timeout_component_changed(int entity);

        void name_component_changed(int);
        void script_file_component_changed(int);

        void change_analogAct_component(int, double);
        void change_analogSet_component(int, double);
        void change_digitalAct_component(int, bool);
        void change_digitalSet_component(int, bool);
        void change_script_file_component(int , QString );
        void change_data_aquistition_dll_component(int , QString );
        void change_timeout_component(int , int );

        void send_custom_command(const QString&);
        void receivedCustomData(const QString &);
        void newAnalogValue(int);
        void newDigitalValue(int);
        void error(QString);


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
        void debug_script_file(const QString & );
        void abort_script();
        void script_finished();
        void script_skip_sleep();
        void script_message(const QString&, const QString&);

        void start_aquisition();
        void stop_aquisition();

        //void register_scritable_component(int);
        //void unregister_scritable_component(int);
        void newScriptableObject(int);

    private:
        //static QAtomicPointer<event_manager> _instance;
        static event_manager* _instance;
        static QMutex _mutex;
    };

} // namespace talorion

#endif // TALORION_EVENT_MANAGER_HPP
