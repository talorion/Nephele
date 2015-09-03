#include "qvmbackend.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    qvmbackend::qvmbackend(QObject *par) :
        abstract_backend(par),
        analog(),
        actbuffer(),
        setbuffer()
    {
//        connect(this,SIGNAL(avSetChangeCommand(QByteArray)),event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)));
//        connect(this,SIGNAL(newAnalogValue(int)),event_manager::get_instance(),SIGNAL(newAnalogValue(int)));

//        //connect(event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType, int)), this, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType,int)));
//        connect(event_manager::get_instance(),SIGNAL(error(QString)), this, SLOT(logError(QString)));
//        connect(event_manager::get_instance(),SIGNAL(set_value_changed(int)), this, SLOT(avSetChangeProxy(int)));

//        connect(this,SIGNAL(change_act_value(int,double)),event_manager::get_instance(),SIGNAL(change_act_value(int,double)));
//        connect(this,SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_set_value(int,double)));

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

            analog.clear();
            actbuffer.clear();
            setbuffer.clear();
            if((desc.find("AV").value().canConvert<QVariantList>()))
            {
                for(int i=0; i < desc.find("AV").value().toList().length(); i++)
                {
                    QVariantMap tmp = desc.find("AV").value().toList()[i].toMap();
                    if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("set") && tmp.contains("id"))
                    {
                        int av = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
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
                        setbuffer.append(tmp.find("set").value().toDouble());
                        actbuffer.append(0);
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
                        int entity= analog[i];
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

    void qvmbackend::logError(QString errorString)
    {
        qDebug() << errorString << endl;
    }

    void qvmbackend::fcSetChangeProxy(int entity)
    {
        double value =entity_manager::get_instance()->get_analogSetValue_component(entity);
        int id = entity_manager::get_instance()->get_id_component(entity);
        if(id >= 0)
            fcSetChangeProxy(value, id);
    }

    void qvmbackend::fcSetChangeProxy(double value, int id)
    {
        //QByteArray command = "uibkav setById ";
        QByteArray command = "uibkav setById ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit fcSetChangeCommand(command);
    }
}
