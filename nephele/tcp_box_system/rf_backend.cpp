#include "rf_backend.hpp"

#include <QDebug>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    rf_backend::rf_backend(QObject *par) :
        abstract_backend(par),
        flowcontroller(),
        actbuffer(),
        setbuffer(),
        block_next_read(false)
    {

    }

    rf_backend::~rf_backend()
    {

    }

    void rf_backend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)
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
            QVariantMap::const_iterator cit;
            for(cit = desc.constBegin(); cit != desc.constEnd(); cit++){
                if(QString::compare( cit.key(), "AIO") == 0){

                }
                if(QString::compare( cit.key(), "AO") == 0){
                    QVariant tmp =cit.value();
                    if(tmp.canConvert(QMetaType::QVariantList)){
                        QVariantList lst= tmp.toList();
                        parse_all_AO(lst, box_id);
                    }
                }
                if(QString::compare( cit.key(), "AI") == 0){
                    QVariant tmp =cit.value();
                    if(tmp.canConvert(QMetaType::QVariantList)){
                        QVariantList lst=  tmp.toList();
                        parse_all_AI(lst, box_id);
                    }
                }
                if(QString::compare( cit.key(), "DO") == 0){

                }
                if(QString::compare( cit.key(), "DI") == 0){

                }
                if(QString::compare( cit.key(), "AIO") == 0){

                }
                if(QString::compare( cit.key(), "DIO") == 0){

                }
                if(QString::compare( cit.key(), "uibk_v") == 0){

                }
            }

            //            if((desc.find("FC").value().canConvert<QVariantList>()))
            //            {
            //                for(int i=0; i < desc.find("FC").value().toList().length(); i++)
            //                {
            //                    QVariantMap tmp = desc.find("FC").value().toList()[i].toMap();
            //                    if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("set") && tmp.contains("id"))
            //                    {
            //                        int fc = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
            //                                                                                      tmp.find("units").value().toString(),
            //                                                                                      tmp.find("smin").value().toDouble(),
            //                                                                                      tmp.find("smax").value().toDouble(),
            //                                                                                      tmp.find("amin").value().toDouble(),
            //                                                                                      tmp.find("amax").value().toDouble(),
            //                                                                                      tmp.find("set").value().toDouble(),
            //                                                                                      tmp.find("id").value().toInt(),
            //                                                                                      box_id
            //                                                                                      );
            //                        flowcontroller.append(fc);
            //                        //setbuffer.append(tmp.find("set").value().toDouble());
            //                        int tmp_id = tmp.find("id").value().toInt();
            //                        double tmp_set = tmp.find("set").value().toDouble();
            //                        setbuffer.insert(tmp_id, tmp_set);
            //                        actbuffer.append(0);
            //                    }
            //                }
            //            }
        }
        default:
        {
                        if ( (desc.find("AI").value().canConvert<QVariantList>()) && (flowcontroller.length()>0))
                        {
                            for(int i=0; i < desc.find("AI").value().toList().length(); i++)
                            {
                                QVariantMap tmp = desc.find("AI").value().toList()[i].toMap();
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
//                                    if (tmp.contains("set")){
//                                        int tmp_id = tmp.find("id").value().toInt();
//                                        double val = tmp.find("set").value().toDouble();
//                                        if(setbuffer[tmp_id] != val){
//                                            //qDebug()<<"flowControllerBackend::processData"<<setbuffer[tmp_id]<<val;
//                                            setbuffer[tmp_id] =val;
//                                            emit change_set_value(entity,val);
//                                        }
//                                    }
                                }
                            }
                        }

        }
        }
    }

    void rf_backend::logError(QString errorString)
    {
        qDebug() << errorString << endl;
    }

    void rf_backend::fcSetChangeProxy(int entity)
    {
        double value =entity_manager::get_instance()->get_analogSetValue_component(entity);
        int id = entity_manager::get_instance()->get_id_component(entity);
        if(id >= 0 && flowcontroller.contains(entity)){
            setbuffer[id] =value;
            block_next_read = true;
            fcSetChangeProxy(value, id);
        }
    }

    void rf_backend::fcSetChangeProxy(double value, int id)
    {
        //QByteArray command = "uibkafc set ";
        //qDebug()<<"flowControllerBackend::fcSetChangeProxy"<<"set "<<value;
        QByteArray command = "uibk setByModule ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit fcSetChangeCommand(command);
        //setbuffer[id] =value;
    }

    void rf_backend::parse_all_AO(QVariantList desc, int box_id)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;
            QVariantMap tmp = var.toMap();
            if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("set") && tmp.contains("id"))
            {
                int fc = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
                                                                              tmp.find("units").value().toString(),
                                                                              tmp.find("smin").value().toDouble(),
                                                                              tmp.find("smax").value().toDouble(),
                                                                              0,
                                                                              0,
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

    void rf_backend::parse_all_AI(QVariantList desc, int box_id)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;
            QVariantMap tmp = var.toMap();
            if (tmp.contains("name") && tmp.contains("units") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("act") && tmp.contains("id"))
            {
                //QVariantMap tmp = var.toMap();
                if (tmp.contains("name") && tmp.contains("units")  && tmp.contains("amin") && tmp.contains("amax")  && tmp.contains("id"))
                {
                    int fc = entity_manager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
                                                                                  tmp.find("units").value().toString(),
                                                                                  0,
                                                                                  0,
                                                                                  tmp.find("amin").value().toDouble(),
                                                                                  tmp.find("amax").value().toDouble(),
                                                                                  0,
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
} // namespace talorion

