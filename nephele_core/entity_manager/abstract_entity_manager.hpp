#ifndef TALORION_ABSTRACT_ENTITY_MANAGER_HPP
#define TALORION_ABSTRACT_ENTITY_MANAGER_HPP

#include <QObject>
#include <QUuid>

QT_BEGIN_NAMESPACE
class QVariant;
class QScriptEngine;
QT_END_NAMESPACE

namespace talorion {

    class abstract_configuration_widget;
    class abstract_scriptable_object;

    class abstract_entity_manager : public QObject
    {
        Q_OBJECT
    protected:
        explicit abstract_entity_manager(QObject *par = 0);
        virtual ~abstract_entity_manager();
        Q_DISABLE_COPY(abstract_entity_manager)

    public:
        virtual void initialize()=0;
        virtual void dispose()=0;

        //=== DB Functions
        virtual int createNewEntity(QString human_readable_label=QString(), int entity=-1, bool isSystem = false) =0;
        virtual bool entity_exists(int entity_id) const=0;
        virtual void delete_entity(int entity_id)=0;
        //virtual void createComponentAndAddTo(static_component_id comp_id, int entity_id)=0;
        virtual void createComponentAndAddTo(int comp_id, int entity_id)=0;
        virtual void removeComponentFrom(int comp_id, int entity_id)=0;
        virtual bool component_exists(int comp_id) const=0;
        virtual bool hasComponent(int comp_id, int entity_id) const=0;
        virtual QVariant getComponentDataForEntity(int component_id, int entity_id) const=0;
        virtual void setComponentDataForEntity(int component_id, int entity_id,  const QVariant &component_data)=0;

        //===

        //=== Factory methods
        virtual int createNewSystem(QUuid suid, QString nameVal, abstract_configuration_widget* sys_cfg_wdg)=0;
        virtual int createNewAnalogInputValue(QString nameVal, QString unitsVal, double amin, double amax, int id, int box_id , int entity =-1 ) =0;
        virtual int createNewAnalogOutputValue(QString nameVal, QString unitsVal, double smin, double smax, double setVal, int id, int box_id , int entity =-1) =0;
        virtual int createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) =0;
        virtual int createNewDigitalInputValue(QString nameVal, QString unitsVal,  int id, int box_id , int entity =-1) =0;
        virtual int createNewDigitalOutputValue(QString nameVal,  bool setVal, int id, int box_id , int entity =-1) =0;
        virtual int createNewDigitalValue(QString nameVal, QString unitsVal,  bool setVal, int id, int box_id ) =0;
        virtual int createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id ) =0;
        virtual int createNewTcpBox(QString nameVal="New Box", QString ip="localhost", quint16 port=2701)=0;
        virtual int createQtScriptEngine(QString nameVal="Qt Script Engine", QScriptEngine* engine=NULL)=0;
        virtual int createTofDaqDll(QString nameVal="TofDaqDll", QString pathVal="C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll", int timeout = 5000, int updaterate=1000, QString user_data_path="/USER_DATA")=0;

        virtual int createScriptableObject(QString nameVal, abstract_scriptable_object* comp)=0;
        //===

        virtual bool is_analog_value(int entity) const=0;
        virtual bool is_digital_value(int entity) const=0;

        //        int add_scriptable_component(int entity, abstract_scriptable_object* comp)=0;
        //        int remove_scriptable_component(int entity)=0;

        //=== Factory constants
        QUuid get_AnalogInputValue_uid()const   {return ("{340c5f10-6d53-4c15-8fc4-4c559580cfbc}");}
        QUuid get_AnalogOutputValue_uid()const  {return ("{c81ab50c-417c-404f-87b3-f9a8e474a6b1}");}
        QUuid get_AnalogValue_uid()const        {return ("{6ddc030e-2001-4a38-a8ce-57b309f902ff}");}
        QUuid get_DigitalInputValue_uid()const  {return ("{231fe499-ed93-4f66-a2ed-5ca5063a0d86}");}
        QUuid get_DigitalOutputValue_uid()const {return ("{bb3e0e52-fb29-429b-9668-b2e3414e9db3}");}
        QUuid get_DigitalValue_uid()const       {return ("{837c326e-e5fb-4271-97e8-8a3161cfc02c}");}
        QUuid get_TcpBox_uid()const             {return ("{99060fb8-676f-47d8-b9f1-c9c492721009}");}
        QUuid get_System_uid()const             {return ("{b50224fa-8908-4503-8296-8b0c8531e1ce}");}
        QUuid get_Qt_Script_Engine_uid()const   {return ("{3a31ae25-c7f4-4e79-93c4-2a4f7b675298}");}
        QUuid get_TofDaqDll_uid()const          {return ("{e16e6a36-4f76-4746-995d-62996c86f74a}");}
        QUuid get_ScriptableObject_uid()const   {return ("{592400d5-7684-4c31-9bfa-7cd806c40589}");}
        //

        QList<int> get_all_tcpBoxes()const{return get_entity_by_serialVersionUID(get_TcpBox_uid());}
        QList<int> get_all_AnalogInputValues()const{return get_entity_by_serialVersionUID(get_AnalogInputValue_uid());}
        QList<int> get_all_AnalogOutputValues()const{return get_entity_by_serialVersionUID(get_AnalogOutputValue_uid());}
        QList<int> get_all_AnalogValues()const{return get_entity_by_serialVersionUID(get_AnalogValue_uid());}
        QList<int> get_all_Values()const;
        QList<int> get_all_DigitalInputValues()const{return get_entity_by_serialVersionUID(get_DigitalInputValue_uid());}
        QList<int> get_all_DigitalOutputValues()const{return get_entity_by_serialVersionUID(get_DigitalOutputValue_uid());}
        QList<int> get_all_DigitalValues()const{return get_entity_by_serialVersionUID(get_DigitalValue_uid());}
        QList<int> get_all_DValues()const;

        QList<int> get_all_Systems()const{return get_entity_by_serialVersionUID(get_System_uid());}
        QList<int> get_all_Qt_Script_Engines()const{return get_entity_by_serialVersionUID(get_Qt_Script_Engine_uid());}
        QList<int> get_all_TofDaqDlls()const{return get_entity_by_serialVersionUID(get_TofDaqDll_uid());}
        QList<int> get_all_ScriptableObjects()const{return get_entity_by_serialVersionUID(get_ScriptableObject_uid());}

        virtual QList<int> get_entities_with_scriptable_components()const=0;
        virtual QList<int> get_entities_with_userdata_components()const=0;

        virtual abstract_configuration_widget* get_systemConfigurationWidget_component(int entity_id) const=0;
        virtual QScriptEngine* get_qt_script_engine_component(int entity_id) const=0;
        virtual abstract_scriptable_object* get_scriptable_object_component(int entity_id) const=0;


        virtual double get_analogActValue_component(int entity)const=0;
        virtual double get_analogSetValue_component(int entity)const=0;
        virtual bool get_digitalActValue_component(int entity)const=0;
        virtual bool get_digitalSetValue_component(int entity)const=0;
        virtual double get_setMin_component(int entity)const=0;
        virtual double get_setMax_component(int entity)const=0;
        virtual double get_actMin_component(int entity)const=0;
        virtual double get_actMax_component(int entity)const=0;
        virtual int get_id_component(int entity)const=0;
        virtual QString get_name_component(int entity)const=0;
        virtual QString get_units_component(int entity)const=0;
        virtual QString get_ip_address_component(int entity)const=0;
        virtual quint16 get_port_component(int entity)const=0;
        virtual int get_box_id_component(int entity)const=0;
        virtual bool get_connection_state_component(int entity)const=0;
        virtual QUuid get_serialVersionUID_component(int entity)const=0;
        virtual QUuid get_systemVersionUID_component(int entity)const=0;
        virtual int get_tcp_box_backend_component(int entity)const=0;
        virtual QString get_script_file_component(int entity)const=0;
        virtual QString get_data_aquistion_dll_component(int entity)const=0;
        virtual int get_timeout_component(int entity)const=0;
        virtual int get_updaterate_component(int entity)const=0;
        virtual int get_userdata_component(int entity)const=0;
        virtual QString get_user_data_path_component(int entity)const=0;


        //QList<int> get_all_systems()const=0;
        virtual QList<int> get_entity_by_name(const QString& name) const=0;
        virtual QList<int> get_entity_by_serialVersionUID(const QUuid& uid) const=0;
        virtual QList<int> get_entity_by_systemVersionUID(const QUuid& uid) const=0;
        virtual QList<int> get_entity_by_component(int comp_id) const=0;

        virtual QList<int> get_all_entities()const=0;
        virtual QList<int> get_all_components_of_entity(int entity)const=0;

    public slots:
        virtual void slot_change_name_component(int entity, QString value) =0;
        virtual void slot_change_ip_address_component(int entity, QString value) =0;
        virtual void slot_change_port_component(int entity, quint16 value) =0;
        virtual void slot_connection_state_component(int entity, bool value) =0;
        virtual void slot_change_tcp_box_backend_component(int entity, int value) =0;


    };

} // namespace talorion

#endif // TALORION_ABSTRACT_ENTITY_MANAGER_HPP
