#include "flowcontrollerbackend.h"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

//#include "tcpdriver.hpp"

namespace talorion {
    flowControllerBackend::flowControllerBackend(abstract_backend *par) :
        abstract_backend(par),
        flowcontroller(),
        actbuffer(),
        setbuffer(),
        block_next_read(false)
    {
    }

    flowControllerBackend::~flowControllerBackend()
    {

    }

    int flowControllerBackend::count()
    {
        return flowcontroller.count();
    }

    void flowControllerBackend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)
    {
        if(block_next_read){
            block_next_read = false;
            return;
        }

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
                        //setbuffer.append(tmp.find("set").value().toDouble());
                        int tmp_id = tmp.find("id").value().toInt();
                        double tmp_set = tmp.find("set").value().toDouble();
                        setbuffer.insert(tmp_id, tmp_set);
                        actbuffer.append(0);
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
                    //int tmp_id = tmp.find("id").value().toInt();
                    if ( i<flowcontroller.length())
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
                            int tmp_id = tmp.find("id").value().toInt();
                            double val = tmp.find("set").value().toDouble();
                            if(setbuffer[tmp_id] != val){
                                //qDebug()<<"flowControllerBackend::processData"<<setbuffer[tmp_id]<<val;
                                setbuffer[tmp_id] =val;
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
        double value =entity_manager::get_instance()->get_analogSetValue_component(entity);
        int id = entity_manager::get_instance()->get_id_component(entity);
        if(id >= 0 && flowcontroller.contains(entity)){
            setbuffer[id] =value;
            block_next_read = true;
            fcSetChangeProxy(value, id);
        }
    }

    void flowControllerBackend::fcSetChangeProxy(double value, int id)
    {
        //QByteArray command = "uibkafc set ";
        //qDebug()<<"flowControllerBackend::fcSetChangeProxy"<<"set "<<value;
        QByteArray command = "uibkafc setByModule ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit fcSetChangeCommand(command);
        //setbuffer[id] =value;
    }
}
