#ifndef TALORION_NULL_ENTITY_MANAGER_HPP
#define TALORION_NULL_ENTITY_MANAGER_HPP

#include "abstract_entity_manager.hpp"

namespace talorion {

    class null_entity_manager : public abstract_entity_manager
    {
        Q_OBJECT
    public:
        explicit null_entity_manager(QObject *par = 0);
        ~null_entity_manager();
        Q_DISABLE_COPY(null_entity_manager)


        // abstract_entity_manager interface
    public:
        virtual void initialize() Q_DECL_OVERRIDE;
        virtual void dispose() Q_DECL_OVERRIDE;

        virtual int createNewEntity(QString, int, bool) Q_DECL_OVERRIDE;
        virtual bool entity_exists(int) const Q_DECL_OVERRIDE;
        virtual void delete_entity(int) Q_DECL_OVERRIDE;
        virtual void createComponentAndAddTo(int, int) Q_DECL_OVERRIDE;
        virtual void removeComponentFrom(int, int) Q_DECL_OVERRIDE;
        virtual bool component_exists(int comp_id) const Q_DECL_OVERRIDE;
        virtual bool hasComponent(int, int) const Q_DECL_OVERRIDE;
        virtual QVariant getComponentDataForEntity(int, int) const Q_DECL_OVERRIDE;
        virtual void setComponentDataForEntity(int, int, const QVariant &) Q_DECL_OVERRIDE;

        virtual int createNewSystem(QUuid, QString, abstract_configuration_widget *) Q_DECL_OVERRIDE;
        virtual int createNewAnalogInputValue(QString, QString, double, double, int, int, int) Q_DECL_OVERRIDE;
        virtual int createNewAnalogOutputValue(QString, QString, double, double, double, int, int, int) Q_DECL_OVERRIDE;
        virtual int createNewAnalogValue(QString, QString, double, double, double, double, double, int, int) Q_DECL_OVERRIDE;
        virtual int createNewDigitalInputValue(QString, QString, int, int, int) Q_DECL_OVERRIDE;
        virtual int createNewDigitalOutputValue(QString nameVal, bool, int, int, int) Q_DECL_OVERRIDE;
        virtual int createNewDigitalValue(QString nameVal, QString, bool, int, int) Q_DECL_OVERRIDE;
        virtual int createNewDigitalValue(QString nameVal, bool, int, int) Q_DECL_OVERRIDE;
        virtual int createNewTcpBox(QString nameVal, QString ip, quint16) Q_DECL_OVERRIDE;
        virtual int createQtScriptEngine(QString, QScriptEngine *) Q_DECL_OVERRIDE;
        virtual int createTofDaqDll(QString nameVal, QString, int, int, QString) Q_DECL_OVERRIDE;
        virtual int createScriptableObject(QString, abstract_scriptable_object *) Q_DECL_OVERRIDE;

        virtual bool is_analog_value(int) const Q_DECL_OVERRIDE;
        virtual bool is_digital_value(int) const Q_DECL_OVERRIDE;

        virtual QList<int> get_entities_with_scriptable_components() const Q_DECL_OVERRIDE;
        virtual QList<int> get_entities_with_userdata_components() const Q_DECL_OVERRIDE;
        virtual abstract_configuration_widget *get_systemConfigurationWidget_component(int) const Q_DECL_OVERRIDE;
        virtual QScriptEngine *get_qt_script_engine_component(int) const Q_DECL_OVERRIDE;
        virtual abstract_scriptable_object *get_scriptable_object_component(int) const Q_DECL_OVERRIDE;
        virtual double get_analogActValue_component(int) const Q_DECL_OVERRIDE;
        virtual double get_analogSetValue_component(int) const Q_DECL_OVERRIDE;
        virtual bool get_digitalActValue_component(int) const Q_DECL_OVERRIDE;
        virtual bool get_digitalSetValue_component(int) const Q_DECL_OVERRIDE;
        virtual double get_setMin_component(int) const Q_DECL_OVERRIDE;
        virtual double get_setMax_component(int ) const Q_DECL_OVERRIDE;
        virtual double get_actMin_component(int) const Q_DECL_OVERRIDE;
        virtual double get_actMax_component(int) const Q_DECL_OVERRIDE;
        virtual int get_id_component(int) const Q_DECL_OVERRIDE;
        virtual QString get_name_component(int) const Q_DECL_OVERRIDE;
        virtual QString get_units_component(int) const Q_DECL_OVERRIDE;
        virtual QString get_ip_address_component(int) const Q_DECL_OVERRIDE;
        virtual quint16 get_port_component(int) const Q_DECL_OVERRIDE;
        virtual int get_box_id_component(int) const Q_DECL_OVERRIDE;
        virtual bool get_connection_state_component(int) const Q_DECL_OVERRIDE;
        virtual QUuid get_serialVersionUID_component(int) const Q_DECL_OVERRIDE;
        virtual QUuid get_systemVersionUID_component(int) const Q_DECL_OVERRIDE;
        virtual int get_tcp_box_backend_component(int ) const Q_DECL_OVERRIDE;
        virtual QString get_script_file_component(int) const Q_DECL_OVERRIDE;
        virtual QString get_data_aquistion_dll_component(int) const Q_DECL_OVERRIDE;
        virtual int get_timeout_component(int) const Q_DECL_OVERRIDE;
        virtual int get_updaterate_component(int) const Q_DECL_OVERRIDE;
        virtual int get_userdata_component(int) const Q_DECL_OVERRIDE;
        virtual QString get_user_data_path_component(int) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_name(const QString &) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_serialVersionUID(const QUuid &) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_systemVersionUID(const QUuid &) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_component(int) const Q_DECL_OVERRIDE;
        virtual QList<int> get_all_entities() const Q_DECL_OVERRIDE;
        virtual QList<int> get_all_components_of_entity(int) const Q_DECL_OVERRIDE;

    public slots:
        virtual void slot_change_name_component(int, QString) Q_DECL_OVERRIDE;
        virtual void slot_change_ip_address_component(int, QString) Q_DECL_OVERRIDE;
        virtual void slot_change_port_component(int, quint16) Q_DECL_OVERRIDE;
        virtual void slot_connection_state_component(int, bool) Q_DECL_OVERRIDE;
        virtual void slot_change_tcp_box_backend_component(int, int) Q_DECL_OVERRIDE;

    };

} // namespace talorion

#endif // TALORION_NULL_ENTITY_MANAGER_HPP
