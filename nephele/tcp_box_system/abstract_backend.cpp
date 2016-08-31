#include "abstract_backend.hpp"

#include "core/event_manager.hpp"

namespace talorion {

    abstract_backend::abstract_backend(QObject *par) : QObject(par)
    {
        //connect(this,SIGNAL(fcSetChangeCommand(QByteArray)),drv,SLOT(setDataCommand(QByteArray)));
        //connect(this,SIGNAL(fcSetChangeCommand(QByteArray)),event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)));
        //connect(this,SIGNAL(newFlowcontroller(int)),event_manager::get_instance(),SIGNAL(newAnalogValue(int)));

        //connect(event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType, int)), this, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType, int)));
        connect(event_manager::get_instance(),SIGNAL(error(QString)), this, SLOT(logError(QString)));
        connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)), this, SLOT(fcSetChangeProxy(int)));
        connect(event_manager::get_instance(),SIGNAL(digitalSet_component_changed(int)), this, SLOT(dvSetChangeProxy(int)));

        connect(this,SIGNAL(change_act_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogAct_component(int,double)));
        connect(this,SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);

        connect(this,SIGNAL(change_digital_act_value(int,bool)),event_manager::get_instance(),SIGNAL(change_digitalAct_component(int,bool)));
        connect(this,SIGNAL(change_digital_set_value(int,bool)),event_manager::get_instance(),SIGNAL(change_digitalSet_component(int,bool)),Qt::UniqueConnection);

    }

    abstract_backend::~abstract_backend()
    {

    }


} // namespace talorion

