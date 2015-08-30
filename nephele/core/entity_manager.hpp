#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include <QObject>

#include <QMap>

QT_BEGIN_NAMESPACE
class QMutex;
class QSignalMapper;
class QVariant;
QT_END_NAMESPACE



namespace talorion {

    class analogValue;

    class entity_manager : public QObject
    {
        Q_OBJECT

    private:
        explicit entity_manager(QObject *parent = 0);
        ~entity_manager();
        Q_DISABLE_COPY(entity_manager)

    public:
        static entity_manager *get_instance();
        static void destroy();
        //        int createNewEntity(QString human_readable_label=QString());

        //        void createComponentAndAddTo(int component_id, int entity_id);

        //        QVariantMap getComponentDataForEntity(int component_id, int entity_id);

        //        void setComponentDataForEntity(int component_id, int entity_id,  QVariantMap component_data);

        //analogValue* createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) ;
        int createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) ;

        double get_actValue_component(int entity)const;
        double get_setValue_component(int entity)const;
        double get_setMin_component(int entity)const;
        double get_setMax_component(int entity)const;
        double get_actMin_component(int entity)const;
        double get_actMax_component(int entity)const;
        int get_id_component(int entity)const;
        QString get_name_component(int entity)const;
        QString get_units_component(int entity)const;

        int get_entity_by_name(const QString& name) const;

    signals:
        void set_value_changed(int entity );
        void act_value_changed(int entity);

        void newAnalogValue(int);

    private:
        analogValue* get_analogValue(int entity)const;
        int generate_Hash(int box_id, int value_id)const;

        void set_actValue_component(int entity, double val);
        void set_setValue_component(int entity, double val);
        void set_setMin_component(int entity, double val);
        void set_setMax_component(int entity, double val);
        void set_actMin_component(int entity, double val);
        void set_actMax_component(int entity, double val);
        void set_name_component(int entity, QString val);
        void set_units_component(int entity, QString val);

    private slots:
        void slot_change_act_value(int entity, double value);
        void slot_change_set_value(int entity, double value);

    private:
        QMap<int, analogValue*> analog_values;
        QSignalMapper *act_value_signalMapper;
        QSignalMapper *set_value_signalMapper;

    private:
        static QAtomicPointer<entity_manager> _instance;
        static QMutex _mutex;

        static const int P1=997;
        static const int P2=1009;
    };

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP
