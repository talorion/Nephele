#include "qvmbackend.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    qvmbackend::qvmbackend(QObject *par) :
        QObject(par),
        analog()
    {
        connect(this,SIGNAL(avSetChangeCommand(QByteArray)),event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)));
        connect(this,SIGNAL(newAnalogValue(analogValue*)),event_manager::get_instance(),SIGNAL(newAnalogValue(analogValue*)));
        connect(event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType, int)), this, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType,int)));
        connect(event_manager::get_instance(),SIGNAL(error(QString)), this, SLOT(logError(QString)));
    }

    qvmbackend::~qvmbackend()
    {

    }

    int qvmbackend::count()
    {
        return analog.count();
    }

    void qvmbackend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)
    {
        switch (type)
        {
        case tcpDriverDataTypes::ALLDATA:
        {
            foreach (analogValue* av, analog)
            {
                av->deleteLater();
            }

            analog.clear();
            if((desc.find("AV").value().canConvert<QVariantList>()))
            {
                for(int i=0; i < desc.find("AV").value().toList().length(); i++)
                {
                    QVariantMap tmp = desc.find("AV").value().toList()[i].toMap();
                    if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("set") && tmp.contains("id"))
                    {
                        //                        analogValue* av = new analogValue(tmp.find("name").value().toString(),
                        //                                                          tmp.find("units").value().toString(),
                        //                                                          tmp.find("smin").value().toDouble(),
                        //                                                          tmp.find("smax").value().toDouble(),
                        //                                                          tmp.find("amin").value().toDouble(),
                        //                                                          tmp.find("amax").value().toDouble(),
                        //                                                          tmp.find("set").value().toDouble(),
                        //                                                          tmp.find("id").value().toInt()
                        //                                                          );

                        analogValue* av = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
                                                                                               tmp.find("units").value().toString(),
                                                                                               tmp.find("smin").value().toDouble(),
                                                                                               tmp.find("smax").value().toDouble(),
                                                                                               tmp.find("amin").value().toDouble(),
                                                                                               tmp.find("amax").value().toDouble(),
                                                                                               tmp.find("set").value().toDouble(),
                                                                                               tmp.find("id").value().toInt(),
                                                                                               box_id
                                                                                               );
                        analog.append(av);
                        emit newAnalogValue(av);
                        connect(av,SIGNAL(setChangedByGui(double,int)), this, SLOT(avSetChangeProxy(double,int)));
                        qDebug() << "Found Analog Value: " << desc.find("AV").value().toList()[i].toMap().find("name").value().toString();
                    }
                }
            }
        }
        default:
        {
            if ( (desc.find("AV").value().canConvert<QVariantList>()) && (analog.length()>0))
            {
                for(int i=0; i < desc.find("AV").value().toList().length(); i++)
                {
                    QVariantMap tmp = desc.find("AV").value().toList()[i].toMap();
                    if (i<analog.length())
                    {
                        //                        if (tmp.contains("act"))
                        //                            analog[i]->updateActByConnection(tmp.find("act").value().toDouble());
                        //                        if (tmp.contains("set"))
                        //                            analog[i]->updateSetByConnection(tmp.find("set").value().toDouble());
                        int hash= analog[i]->getHashVal();
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
                    //                    qDebug() << "AV_" << QString::number(i) << " act: " << tmp.find("act").value().toString();
                }
            }

        }
        }
    }

    void qvmbackend::logError(QString errorString)
    {
        qDebug() << errorString << endl;
    }

    void qvmbackend::avSetChangeProxy(double value, int id)
    {
        QByteArray command = "uibkav setById ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit avSetChangeCommand(command);
    }
}
