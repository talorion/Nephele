#include "arduino_backend.hpp"

#include <QDebug>

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

    arduino_backend::arduino_backend(QObject *par) :
        abstract_backend(par),
        registered_values(),
        analog_actbuffer(),
        analog_setbuffer(),
        digital_actbuffer(),
        digital_setbuffer(),
        block_next_read(false),
        m_box_id(-1)
    {

    }

    arduino_backend::~arduino_backend()
    {

    }

    void arduino_backend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)
    {
        m_box_id=box_id;
        if(block_next_read){
            block_next_read = false;
            return;
        }

        switch (type)
        {
        case tcpDriverDataTypes::UIBK_ECMD_ALLDATA:{
            registered_values.clear();
            analog_actbuffer.clear();
            analog_setbuffer.clear();
            QVariantMap::const_iterator cit;
            for(cit = desc.constBegin(); cit != desc.constEnd(); cit++){
                QVariant tmp =cit.value();
                if(tmp.canConvert(QMetaType::QVariantList)){
                    QVariantList lst= tmp.toList();
                    if(QString::compare( cit.key(), "AO") == 0){parse_alldata_AO(lst, box_id);}
                    if(QString::compare( cit.key(), "AI") == 0){parse_alldata_AI(lst, box_id);}
                    if(QString::compare( cit.key(), "AIO") == 0){parse_alldata_AIO(lst, box_id);}
                    if(QString::compare( cit.key(), "FC") == 0){parse_alldata_AIO(lst, box_id);}
                    if(QString::compare( cit.key(), "AV") == 0){qDebug() << "AV pack received."; parse_alldata_AIO(lst, box_id);}
                    if(QString::compare( cit.key(), "DO") == 0){qDebug() << "DO pack received."; parse_alldata_DO(lst, box_id);}
                }
            }
        }
        default:{
            QVariantMap::const_iterator cit;
            for(cit = desc.constBegin(); cit != desc.constEnd(); cit++){
                QVariant tmp =cit.value();
                if(tmp.canConvert(QMetaType::QVariantList)){
                    QVariantList lst= tmp.toList();
                    if(QString::compare( cit.key(), "AO") == 0){parse_actset_AO(lst);}
                    if(QString::compare( cit.key(), "AI") == 0){parse_actset_AI(lst);}
                    if(QString::compare( cit.key(), "AIO") == 0){parse_actset_AIO(lst);}
                    if(QString::compare( cit.key(), "FC") == 0){parse_actset_AIO(lst);}
                    if(QString::compare( cit.key(), "AV") == 0){parse_actset_AIO(lst);}
                    if(QString::compare( cit.key(), "DO") == 0){parse_actset_DO(lst);}
                }
            }
            break;
        }
        }
    }

    void arduino_backend::logError(QString errorString, int source)
    {
        if(m_box_id==source)
        qDebug() << errorString << endl;
    }

    void arduino_backend::fcSetChangeProxy(int entity)
    {
        double value =EntityManager::get_instance()->get_analogSetValue_component(entity);
        int id = EntityManager::get_instance()->get_id_component(entity);
        if(id >= 0 && registered_values.contains(id)){
            if(registered_values[id]!=entity)
                return;
            analog_setbuffer[id] =value;
            block_next_read = true;
            fcSetChangeProxy(value, id);
        }
    }

    void arduino_backend::fcSetChangeProxy(double value, int id)
    {
        //QByteArray command = "uibkafc set ";
        //qDebug()<<"flowControllerBackend::fcSetChangeProxy"<<"set "<<value;
        QByteArray command = "setById ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit fcSetChangeCommand(command);
        //setbuffer[id] =value;
    }

    void arduino_backend::dvSetChangeProxy(int entity)
    {

        bool valueb =EntityManager::get_instance()->get_digitalSetValue_component(entity);
        int id = EntityManager::get_instance()->get_id_component(entity);
        if(id >= 0 && registered_values.contains(id)){
            if(registered_values[id]!=entity)
                return;

            int valuei = (valueb ? 1 : 0);
            digital_setbuffer[id] =valuei;
            block_next_read = true;
            fcSetChangeProxy(valuei, id);

        }
    }

    void arduino_backend::dvSetChangeProxy(int value, int id)
    {
        Q_UNUSED(value);
        Q_UNUSED(id);
    }

    void arduino_backend::parse_alldata_AO(QVariantList &desc, int box_id)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;
            QVariantMap tmp = var.toMap();
            if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("set") && tmp.contains("id"))
            {
                int fc = EntityManager::get_instance()->createNewAnalogOutputValue(tmp.find("name").value().toString(),
                                                                                    tmp.find("units").value().toString(),
                                                                                    tmp.find("smin").value().toDouble(),
                                                                                    tmp.find("smax").value().toDouble(),
                                                                                    tmp.find("set").value().toDouble(),
                                                                                    tmp.find("id").value().toInt(),
                                                                                    box_id
                                                                                    );
                //registered_values.append(fc);
                //setbuffer.append(tmp.find("set").value().toDouble());
                int tmp_id = tmp.find("id").value().toInt();
                double tmp_set = tmp.find("set").value().toDouble();
                registered_values.insert(tmp_id, fc);
                analog_setbuffer.insert(tmp_id, tmp_set);
                //actbuffer.append(0);
            }
        }
    }

    void arduino_backend::parse_alldata_AI(QVariantList &desc, int box_id)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;
            QVariantMap tmp = var.toMap();

            if (tmp.contains("name") && tmp.contains("units")  && tmp.contains("amin") && tmp.contains("amax")  && tmp.contains("id"))
            {
                int fc = EntityManager::get_instance()->createNewAnalogInputValue(tmp.find("name").value().toString(),
                                                                                   tmp.find("units").value().toString(),
                                                                                   tmp.find("amin").value().toDouble(),
                                                                                   tmp.find("amax").value().toDouble(),
                                                                                   tmp.find("id").value().toInt(),
                                                                                   box_id
                                                                                   );

                int tmp_id = tmp.find("id").value().toInt();
                registered_values.insert(tmp_id, fc);
                //double tmp_set = tmp.find("set").value().toDouble();
                //setbuffer.insert(tmp_id, tmp_set);
                analog_actbuffer.insert(tmp_id,0);
            }

        }
    }

    void arduino_backend::parse_alldata_AIO(QVariantList &desc, int box_id)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;
            QVariantMap tmp = var.toMap();
            if (tmp.contains("name") && tmp.contains("units") && tmp.contains("smin") && tmp.contains("smax") && tmp.contains("amin") && tmp.contains("amax") && tmp.contains("set") && tmp.contains("id"))
            {
                int fc = EntityManager::get_instance()->createNewAnalogValue(tmp.find("name").value().toString(),
                                                                              tmp.find("units").value().toString(),
                                                                              tmp.find("smin").value().toDouble(),
                                                                              tmp.find("smax").value().toDouble(),
                                                                              tmp.find("amin").value().toDouble(),
                                                                              tmp.find("amax").value().toDouble(),
                                                                              tmp.find("set").value().toDouble(),
                                                                              tmp.find("id").value().toInt(),
                                                                              box_id
                                                                              );
                //registered_values.append(fc);
                int tmp_id = tmp.find("id").value().toInt();
                double tmp_set = tmp.find("set").value().toDouble();
                registered_values.insert(tmp_id, fc);
                analog_setbuffer.insert(tmp_id, tmp_set);
                analog_actbuffer.insert(tmp_id,0);
            }

        }
    }

    void arduino_backend::parse_alldata_DO(QVariantList &desc, int box_id)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;
            QVariantMap tmp = var.toMap();
            if (tmp.contains("name") && tmp.contains("set") && tmp.contains("id"))
            {
                int fc = EntityManager::get_instance()->createNewDigitalOutputValue(tmp.find("name").value().toString(),
                                                                                     tmp.find("set").value().toDouble(),
                                                                                     tmp.find("id").value().toInt(),
                                                                                     box_id
                                                                                     );
                int tmp_id = tmp.find("id").value().toInt();
                int tmp_set = tmp.find("set").value().toInt();
                registered_values.insert(tmp_id, fc);
                digital_setbuffer.insert(tmp_id, tmp_set);
            }
        }
    }


    void arduino_backend::parse_actset_AO(QVariantList &desc)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;

            QVariantMap tmp = var.toMap();

            int tmp_id = tmp.find("id").value().toInt();
            int entity = registered_values[tmp_id];

            if (tmp.contains("set")){
                double val = tmp.find("set").value().toDouble();
                if(analog_setbuffer[tmp_id] != val){
                    analog_setbuffer[tmp_id] =val;
                    emit change_set_value(entity,val);
                }
            }
        }
    }

    void arduino_backend::parse_actset_AI(QVariantList &desc)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;

            QVariantMap tmp = var.toMap();

            int tmp_id = tmp.find("id").value().toInt();
            int entity= registered_values[tmp_id];

            if (tmp.contains("act")){
                double val = tmp.find("act").value().toDouble();
                if(analog_actbuffer[tmp_id] != val){
                    analog_actbuffer[tmp_id] =val;
                    emit change_act_value(entity,val);
                }
            }
        }
    }

    void arduino_backend::parse_actset_AIO(QVariantList &desc)
    {
        parse_actset_AO(desc);
        parse_actset_AI(desc);

    }

    void arduino_backend::parse_actset_DO(QVariantList &desc)
    {
        foreach (QVariant var, desc) {
            if(!var.canConvert(QMetaType::QVariantMap))
                continue;

            QVariantMap tmp = var.toMap();

            int tmp_id = tmp.find("id").value().toInt();
            int entity = registered_values[tmp_id];

            if (tmp.contains("set")){
                int val = tmp.find("set").value().toInt();
                if(digital_setbuffer[tmp_id] != val){
                    digital_setbuffer[tmp_id] =val;
                    emit change_digital_set_value(entity,val);
                }
            }
        }
    }
} // namespace talorion

