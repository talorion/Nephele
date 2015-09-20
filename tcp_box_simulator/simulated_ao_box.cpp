#include "simulated_ao_box.hpp"

#include <QStringList>

simulated_ao_box::simulated_ao_box(QObject *par) :
    abstract_simulated_box(par),
    all_aos(),
    random_id(0)
{
    init_all_aos();
    random_id=qrand();
}

simulated_ao_box::~simulated_ao_box()
{

}

void simulated_ao_box::set(int id, double val)
{
    foreach (simulated_ao* fc, all_aos) {
        if(fc->get_id()==id){
            fc->setSet(val);
            //qDebug()<<"simulated_fc_box::set"<<val;
        }
    }
}

void simulated_ao_box::init_all_aos()
{
    all_aos.clear();
    QStringList names;
    names<<"AO_0"<<"AO_1"<<"AO_2"<<"AO_3"<<"AO_4"<<"AO_5"<<"AO_6"<<"AO_7";
    int i=0;
    foreach (QString nme, names) {
        simulated_ao* fc = new simulated_ao();
        fc->setId(i);
        fc->setSet(i*10+10);
        fc->setName(nme);
        fc->setUnits("none");

        all_aos<<fc;
        ++i;
    }
}

QString simulated_ao_box::getAll_json()
{
    QString jsn =  "{\"uibk_v\":1,\"DO\":[";
    int i=0;
    foreach (simulated_ao* fc, all_aos) {
        if(i)
            jsn += ",";
        jsn +=QString("{\"id\":%1,\"set\":%2,\"name\":\"%3\",\"units\":\"%4\"}")
                .arg(QString::number(fc->get_id()),
                     QString::number(fc->get_set()),
                     //QString::number(fc->get_Act()),
                     fc->get_name(),
                     fc->get_units());

        ++i;
    }
    jsn         += "]}";

    return jsn;
}

