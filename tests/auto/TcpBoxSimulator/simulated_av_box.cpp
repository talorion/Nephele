#include "simulated_av_box.hpp"

#include <QStringList>

simulated_av_box::simulated_av_box(QObject *par) :
    abstract_simulated_box(par),
    all_avs(),
    random_id(0)
{
    init_all_avs();
    random_id=qrand();
}

simulated_av_box::~simulated_av_box()
{
    foreach (simulated_av* var, all_avs) {
        if(var)
            delete var;
    }
}

void simulated_av_box::set(int id, double val)
{
    foreach (simulated_av* av, all_avs) {
        if(av->get_Id()==id){
            av->set_Set(val);
        }
    }
    //all_avs.clear();

}

void simulated_av_box::init_all_avs()
{
    all_avs.clear();
    QStringList names;
    names<<"Uql"<<""<<"Udrift"<<""<<"Uhex"<<""<<"Uhin"<<""<<"Usd1"<<"Isd1"<<"Usd2"<<"Usd3"<<"Isd3"<<"Us"<<"Is";
    int i=0;
    foreach (QString nme, names) {
        simulated_av* av = new simulated_av();
        av->set_Id(i);
        av->set_Smin(0);
        av->set_Smax(200);
        av->set_Amin(0);
        av->set_Amax(200);
        av->set_Set(i*10+10);
        av->set_Name(nme);
        av->set_Units("V");

        all_avs<<av;
        ++i;
    }

}

//{"uibk_v":1,"AV":[
//{"id":0,"smin":0,"smax":200,"amin":0,"amax":200,"set":56,"act":55.58,"name":"Uql","units":"V"},
//{"id":1,"smin":0,"smax":500,"amin":0,"amax":500,"set":9.6,"act":341.6,"name":"","units":"V"},
//{"id":2,"smin":0,"smax":200,"amin":0,"amax":200,"set":44,"act":43.86,"name":"Udrift","units":"V"},
//{"id":3,"smin":0,"smax":500,"amin":0,"amax":500,"set":0,"act":332.4,"name":"","units":"V"},
//{"id":4,"smin":0,"smax":600,"amin":0,"amax":600,"set":0,"act":0.293,"name":"Uhex","units":"V"},
//{"id":5,"smin":0,"smax":500,"amin":0,"amax":500,"set":0,"act":332.1,"name":"","units":"V"},
//{"id":6,"smin":0,"smax":400,"amin":0,"amax":400,"set":0,"act":0.7814,"name":"Uhin","units":"V"},
//{"id":7,"smin":0,"smax":500,"amin":0,"amax":500,"set":5,"act":273.7,"name":"","units":"V"},
//{"id":8,"smin":0,"smax":2000,"amin":0,"amax":2000,"set":640,"act":638.8,"name":"Usd1","units":"V"},
//{"id":9,"smin":0,"smax":2,"amin":0,"amax":2,"set":0,"act":0,"name":"Isd1","units":"mA"},
//{"id":10,"smin":0,"smax":1000,"amin":0,"amax":1000,"set":220,"act":218.8,"name":"Usd2","units":"V"},
//{"id":11,"smin":0,"smax":500,"amin":7.006e-44,"amax":4,"set":0,"act":0.001954,"name":"Isd2","units":"mA"},
//{"id":12,"smin":0,"smax":1000,"amin":0,"amax":1000,"set":0,"act":0.9768,"name":"Usd3","units":"V"},
//{"id":13,"smin":0,"smax":4,"amin":0,"amax":4,"set":0,"act":0,"name":"Isd3","units":"mA"},
//{"id":14,"smin":0,"smax":4000,"amin":0,"amax":4000,"set":1120,"act":1119,"name":"Us","units":"V"},
//{"id":15,"smin":0,"smax":100,"amin":0,"amax":1000,"set":100,"act":65.93,"name":"Is","units":"uA"}
//]}

QString simulated_av_box::getAll_json()
{
    QString jsn =  "{\"uibk_v\":1,\"AV\":[";
    int i=0;
    foreach (simulated_av* fc, all_avs) {
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

