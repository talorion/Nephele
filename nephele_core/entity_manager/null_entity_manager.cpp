#include "null_entity_manager.hpp"

#include <QVariant>

namespace talorion {

    null_entity_manager::null_entity_manager(QObject *par)
        : abstract_entity_manager(par)
    {

    }

    null_entity_manager::~null_entity_manager()
    {

    }

    void null_entity_manager::initialize()
    {

    }

    void null_entity_manager::dispose()
    {
    }

    int null_entity_manager::createNewEntity(QString , int , bool )
    {
        return 0;
    }

    bool null_entity_manager::entity_exists(int ) const
    {
        return false;
    }

    void null_entity_manager::delete_entity(int )
    {
    }

    void null_entity_manager::createComponentAndAddTo(int , int )
    {
    }

    void null_entity_manager::removeComponentFrom(int , int )
    {
    }

    bool null_entity_manager::component_exists(int ) const
    {
        return false;
    }

    bool null_entity_manager::hasComponent(int , int ) const
    {
        return false;
    }

    QVariant null_entity_manager::getComponentDataForEntity(int , int ) const
    {
        return QVariant();
    }

    void null_entity_manager::setComponentDataForEntity(int , int , const QVariant &)
    {
    }

    int null_entity_manager::createNewSystem(QUuid , QString , abstract_configuration_widget *)
    {
        return 0;
    }

    int null_entity_manager::createNewAnalogInputValue(QString , QString , double , double , int , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewAnalogOutputValue(QString , QString , double , double , double , int , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewAnalogValue(QString , QString , double , double , double , double , double , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewDigitalInputValue(QString , QString , int , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewDigitalOutputValue(QString , bool , int , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewDigitalValue(QString , QString , bool , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewDigitalValue(QString , bool , int , int )
    {
        return 0;
    }

    int null_entity_manager::createNewTcpBox(QString , QString , quint16 )
    {
        return 0;
    }

    int null_entity_manager::createQtScriptEngine(QString , QScriptEngine *)
    {
        return 0;
    }

    int null_entity_manager::createTofDaqDll(QString , QString , int , int , QString )
    {
        return 0;
    }

    int null_entity_manager::createScriptableObject(QString , abstract_scriptable_object *)
    {
        return 0;
    }

    bool null_entity_manager::is_analog_value(int ) const
    {
        return false;
    }

    bool null_entity_manager::is_digital_value(int ) const
    {
        return false;
    }

    QList<int> null_entity_manager::get_entities_with_scriptable_components() const
    {
        return QList<int>();
    }

    QList<int> null_entity_manager::get_entities_with_userdata_components() const
    {
        return QList<int>();
    }

    abstract_configuration_widget *null_entity_manager::get_systemConfigurationWidget_component(int ) const
    {
        return NULL;
    }

    QScriptEngine *null_entity_manager::get_qt_script_engine_component(int ) const
    {
        return NULL;
    }

    abstract_scriptable_object *null_entity_manager::get_scriptable_object_component(int ) const
    {
        return NULL;
    }

    double null_entity_manager::get_analogActValue_component(int ) const
    {
        return 0;
    }

    double null_entity_manager::get_analogSetValue_component(int ) const
    {
        return 0;
    }

    bool null_entity_manager::get_digitalActValue_component(int ) const
    {
        return 0;
    }

    bool null_entity_manager::get_digitalSetValue_component(int ) const
    {
        return 0;
    }

    double null_entity_manager::get_setMin_component(int ) const
    {
        return 0;
    }

    double null_entity_manager::get_setMax_component(int ) const
    {
        return 0;
    }

    double null_entity_manager::get_actMin_component(int ) const
    {
        return 0;
    }

    double null_entity_manager::get_actMax_component(int ) const
    {
        return 0;
    }

    int null_entity_manager::get_id_component(int ) const
    {
        return 0;
    }

    QString null_entity_manager::get_name_component(int ) const
    {
        return QString();
    }

    QString null_entity_manager::get_units_component(int ) const
    {
        return QString();
    }

    QString null_entity_manager::get_ip_address_component(int ) const
    {
        return QString();
    }

    quint16 null_entity_manager::get_port_component(int ) const
    {
        return 0;
    }

    int null_entity_manager::get_box_id_component(int ) const
    {
        return 0;
    }

    bool null_entity_manager::get_connection_state_component(int ) const
    {
        return false;
    }

    QUuid null_entity_manager::get_serialVersionUID_component(int ) const
    {
        return QUuid();
    }

    QUuid null_entity_manager::get_systemVersionUID_component(int ) const
    {
        return QUuid();
    }

    int null_entity_manager::get_tcp_box_backend_component(int ) const
    {
        return 0;
    }

    QString null_entity_manager::get_script_file_component(int ) const
    {
        return QString();
    }

    QString null_entity_manager::get_data_aquistion_dll_component(int ) const
    {
        return QString();
    }

    int null_entity_manager::get_timeout_component(int ) const
    {
        return 0;
    }

    int null_entity_manager::get_updaterate_component(int ) const
    {
        return 0;
    }

    int null_entity_manager::get_userdata_component(int ) const
    {
        return 0;
    }

    QString null_entity_manager::get_user_data_path_component(int ) const
    {
        return QString();
    }

    QList<int> null_entity_manager::get_entity_by_name(const QString &) const
    {
        return QList<int>();
    }

    QList<int> null_entity_manager::get_entity_by_serialVersionUID(const QUuid &) const
    {
        return QList<int>();
    }

    QList<int> null_entity_manager::get_entity_by_systemVersionUID(const QUuid &) const
    {
        return QList<int>();
    }

    QList<int> null_entity_manager::get_entity_by_component(int ) const
    {
        return QList<int>();
    }

    QList<int> null_entity_manager::get_all_entities() const
    {
        return QList<int>();
    }

    QList<int> null_entity_manager::get_all_components_of_entity(int ) const
    {
        return QList<int>();
    }

    void null_entity_manager::slot_change_name_component(int , QString )
    {

    }

    void null_entity_manager::slot_change_ip_address_component(int , QString )
    {

    }

    void null_entity_manager::slot_change_port_component(int , quint16 )
    {

    }

    void null_entity_manager::slot_connection_state_component(int , bool )
    {

    }

    void null_entity_manager::slot_change_tcp_box_backend_component(int , int )
    {

    }
} // namespace talorion
