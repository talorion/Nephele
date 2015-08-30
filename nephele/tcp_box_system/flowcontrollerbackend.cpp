#include "flowcontrollerbackend.h"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {
    flowControllerBackend::flowControllerBackend(QObject *par) :
        QObject(par),
        flowcontroller(),
        actbuffer(),
        setbuffer()
    {

        connect(this,SIGNAL(fcSetChangeCommand(QByteArray)),event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)));
        //connect(this,SIGNAL(newFlowcontroller(int)),event_manager::get_instance(),SIGNAL(newAnalogValue(int)));

        connect(event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType, int)), this, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType, int)));
        connect(event_manager::get_instance(),SIGNAL(error(QString)), this, SLOT(logError(QString)));
        connect(event_manager::get_instance(),SIGNAL(set_value_changed(int)), this, SLOT(fcSetChangeProxy(int)));

        connect(this,SIGNAL(change_act_value(int,double)),event_manager::get_instance(),SIGNAL(change_act_value(int,double)));
        connect(this,SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_set_value(int,double)));

    }

    int flowControllerBackend::count()
    {
        return flowcontroller.count();
    }

    void flowControllerBackend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)
    {
        switch (type)
        {
        case tcpDriverDataTypes::ALLDATA:
        {


            flowcontroller.clear();
            actbuffer.clear();
            setbuffer.clear();
            if((desc.find("FC").value().canConvert<QVariantList>()))
            {
                for(int i=0; i < desc.find("FC").value().toList().length(); i++)
                {
                    QVariantMap tmp = desc.find("FC").value().toList()[i].toMap();
                    if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("set") && tmp.contains("id"))
                    {
                        int fc = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
                                                                                      tmp.find("units").value().toString(),
                                                                                      tmp.find("smin").value().toDouble(),
                                                                                      tmp.find("smax").value().toDouble(),
                                                                                      tmp.find("amin").value().toDouble(),
                                                                                      tmp.find("amax").value().toDouble(),
                                                                                      tmp.find("set").value().toDouble(),
                                                                                      tmp.find("id").value().toInt(),
                                                                                      box_id
                                                                                      );
                        flowcontroller.append(fc);
                        setbuffer.append(tmp.find("set").value().toDouble());
                        actbuffer.append(0);
                        //emit newFlowcontroller(fc);
                        qDebug() << "Found FC: " << desc.find("FC").value().toList()[i].toMap().find("name").value().toString();
                    }
                }
            }
        }
        default:
        {
            if ( (desc.find("FC").value().canConvert<QVariantList>()) && (flowcontroller.length()>0))
            {
                for(int i=0; i < desc.find("FC").value().toList().length(); i++)
                {
                    QVariantMap tmp = desc.find("FC").value().toList()[i].toMap();
                    if (i<flowcontroller.length())
                    {
                        int entity= flowcontroller[i];
                        if (tmp.contains("act")){
                            double val = tmp.find("act").value().toDouble();
                            if(actbuffer[i] != val){
                                actbuffer[i] =val;
                                emit change_act_value(entity,val);
                            }
                        }
                        if (tmp.contains("set")){
                            double val = tmp.find("set").value().toDouble();
                            if(setbuffer[i] != val){
                                setbuffer[i] =val;
                                emit change_set_value(entity,val);
                            }
                        }
                    }
                }
            }

        }
        }
    }

    void flowControllerBackend::logError(QString errorString)
    {
        qDebug() << errorString << endl;
    }

    void flowControllerBackend::fcSetChangeProxy(int entity)
    {
        double value =entity_manager::get_instance()->get_setValue_component(entity);
        int id = entity_manager::get_instance()->get_id_component(entity);
        if(id >= 0)
            fcSetChangeProxy(value, id);
    }

    void flowControllerBackend::fcSetChangeProxy(double value, int id)
    {
        QByteArray command = "uibkafc set ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit fcSetChangeCommand(command);
    }
}
