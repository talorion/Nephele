#include "flowcontrollerbackend.h"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {
    flowControllerBackend::flowControllerBackend(QObject *par) :
        QObject(par),
        flowcontroller()
    {

        connect(this,SIGNAL(fcSetChangeCommand(QByteArray)),event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)));
        connect(this,SIGNAL(newFlowcontroller(analogValue*)),event_manager::get_instance(),SIGNAL(newAnalogValue(analogValue*)));
        connect(event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType, int)), this, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType, int)));
        connect(event_manager::get_instance(),SIGNAL(error(QString)), this, SLOT(logError(QString)));
        connect(event_manager::get_instance(),SIGNAL(set_value_changed(int)), this, SLOT(fcSetChangeProxy(int)));
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
            foreach (analogValue* av, flowcontroller)
            {
                av->deleteLater();
            }

            flowcontroller.clear();
            if((desc.find("FC").value().canConvert<QVariantList>()))
            {
                for(int i=0; i < desc.find("FC").value().toList().length(); i++)
                {
                    QVariantMap tmp = desc.find("FC").value().toList()[i].toMap();
                    if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("set") && tmp.contains("id"))
                    {
                        //                        analogValue* fc = new analogValue(tmp.find("name").value().toString(),
                        //                                                          tmp.find("units").value().toString(),
                        //                                                          tmp.find("smin").value().toDouble(),
                        //                                                          tmp.find("smax").value().toDouble(),
                        //                                                          tmp.find("amin").value().toDouble(),
                        //                                                          tmp.find("amax").value().toDouble(),
                        //                                                          tmp.find("set").value().toDouble(),
                        //                                                          tmp.find("id").value().toInt()
                        //                                                          );

                        analogValue* fc = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
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
                        emit newFlowcontroller(fc);
                        //connect(fc,SIGNAL(setChangedByGui(double,int)), this, SLOT(fcSetChangeProxy(double,int)));
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
                        //if (tmp.contains("id")){
                            //int id=tmp.find("id").value().toInt();
                        //}
                        int hash= flowcontroller[i]->getHashVal();
                        if (tmp.contains("act")){
                            double val = tmp.find("act").value().toDouble();
                            entity_manager::get_instance()->set_actValue_component(hash,val);
                            //flowcontroller[i]->updateActByConnection(tmp.find("act").value().toDouble());
                        }
                        if (tmp.contains("set")){
                            double val = tmp.find("set").value().toDouble();
                            entity_manager::get_instance()->set_setValue_component(hash,val);
                            //flowcontroller[i]->updateSetByConnection(tmp.find("set").value().toDouble());
                        }
                    }
                    //                    qDebug() << "FC_" << QString::number(i) << " act: " << tmp.find("act").value().toString();
                }
            }

        }
        }
    }

    void flowControllerBackend::logError(QString errorString)
    {
        qDebug() << errorString << endl;
    }

    void flowControllerBackend::fcSetChangeProxy(int hash)
    {
        double value =entity_manager::get_instance()->get_setValue_component(hash);
        int id = entity_manager::get_instance()->get_id_component(hash);
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
