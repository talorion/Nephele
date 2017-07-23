#include "simulated_fc_box.h"
#include <QDebug>

simulated_fc_box::simulated_fc_box(QObject *par)
    : abstract_simulated_box(par),
      all_fcs(),
      random_id(0)
{
    init_all_fcs();
    random_id=qrand();
}

simulated_fc_box::~simulated_fc_box()
{
    foreach (simulated_fc* var, all_fcs) {
        if(var)
            delete var;
    }
    all_fcs.clear();
}

void simulated_fc_box::set(int id, double val)
{
    foreach (simulated_fc* fc, all_fcs) {
        if(fc->get_Id()==id){
            fc->set_Set(val);
        }
    }

}

void simulated_fc_box::init_all_fcs()
{
    all_fcs.clear();
    QStringList names;
    names<<"FC_0"<<"FC_1"<<"FC_2"<<"FC_3"<<"FC_4"<<"FC_5"<<"FC_6"<<"FC_7";
    int i=0;
    foreach (QString nme, names) {
        simulated_fc* fc = new simulated_fc();
        fc->set_Id(i);
        fc->set_Smin(0);
        fc->set_Smax(200);
        fc->set_Amin(0);
        fc->set_Amax(200);
        fc->set_Set(i*10+10);
        fc->set_Name(nme);
        fc->set_Units("sccm");

        all_fcs<<fc;
        ++i;
    }

}

QString simulated_fc_box::getAll_json()
{
    QString jsn =  "{\"uibk_v\":1,\"FC\":[";
    int i=0;
    foreach (simulated_fc* fc, all_fcs) {
        if(i)
            jsn += ",";
        jsn +=QString("{\"id\":%1,\"smin\":%2,\"smax\":%3,\"amin\":%4,\"amax\":%5,\"set\":%6,\"act\":%7,\"name\":\"%8\",\"units\":\"%9\"}")
                .arg(QString::number(fc->get_Id()),
                     QString::number(fc->get_Smin()),
                     QString::number(fc->get_Smax()),
                     QString::number(fc->get_Amin()),
                     QString::number(fc->get_Amax()),
                     QString::number(fc->get_Set()),
                     QString::number(fc->get_Act()),
                     fc->get_Name(),
                     fc->get_Units());

        ++i;
    }
    jsn         += "]}";

    return jsn;

}

