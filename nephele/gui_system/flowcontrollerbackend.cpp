#include "flowcontrollerbackend.h"

namespace talorion {

    flowControllerBackend::flowControllerBackend(QObject *par) :
        QObject(par),
        flowcontroller()
    {
    }

    flowControllerBackend::~flowControllerBackend()
    {

    }

    int flowControllerBackend::count()
    {
        return flowcontroller.count();
    }

    void flowControllerBackend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type)
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
                        analogValue* fc = new analogValue(tmp.find("name").value().toString(),
                                                          tmp.find("units").value().toString(),
                                                          tmp.find("smin").value().toDouble(),
                                                          tmp.find("smax").value().toDouble(),
                                                          tmp.find("amin").value().toDouble(),
                                                          tmp.find("amax").value().toDouble(),
                                                          tmp.find("set").value().toDouble(),
                                                          tmp.find("id").value().toInt()
                                                          );
                        flowcontroller.append(fc);
                        emit newFlowcontroller(fc);
                        connect(fc,SIGNAL(setChangedByGui(double,int)), this, SLOT(fcSetChangeProxy(double,int)));
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
                        if (tmp.contains("act"))
                            flowcontroller[i]->updateActByConnection(tmp.find("act").value().toDouble());
                        if (tmp.contains("set"))
                            flowcontroller[i]->updateSetByConnection(tmp.find("set").value().toDouble());
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

    void flowControllerBackend::fcSetChangeProxy(double value, int id)
    {
        QByteArray command = "uibkafc set ";
        command.append(QByteArray::number(id));
        command.append(" ");
        command.append(QByteArray::number(value));
        emit fcSetChangeCommand(command);
    }
}
