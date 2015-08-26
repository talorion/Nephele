#include "tcp_box_connection.h"

#include <QHostAddress>

#include "simulated_fc_box.h"

tcp_box_connection::tcp_box_connection(QObject *par) :
    QTcpSocket(par),
    buffer()
{

    connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

tcp_box_connection::~tcp_box_connection()
{

}

void tcp_box_connection::processReadyRead()
{

    buffer.clear();

    if (!isValid()) {
        abort();
        return;
    }

    do {
        if (!readCommand())
            return;

        if (!parseArguments())
            return;

        processData();
    } while (bytesAvailable() > 0);

}

void tcp_box_connection::fc_sendAll()
{
    QString response=fc_buildAll();

    write(response.toLocal8Bit());
}

void tcp_box_connection::fc_sendActSet()
{
    fc_sendAll();
}

int tcp_box_connection::readDataIntoBuffer(const char sep, int maxSize)
{
    if (maxSize > MaxBufferSize)
        return 0;

    int numBytesBeforeRead = buffer.size();
    if (numBytesBeforeRead == MaxBufferSize) {
        abort();
        return 0;
    }

    while (bytesAvailable() > 0 && buffer.size() < maxSize) {
        buffer.append(read(1));
        if (buffer.endsWith(sep))
            break;
    }
    return buffer.size() - numBytesBeforeRead;
}

bool tcp_box_connection::readCommand()
{
    if (readDataIntoBuffer() <= 0) {
        return false;
    }

    if(buffer == "uibkafc getAll\r\n"){
        fc_sendAll();
    } else if(buffer == "uibkafc getActSet\r\n"){
        fc_sendActSet();
    } else if(buffer == "uibkafc set "){
//    } else if (buffer == "uibkafc setById"){
//    } else if (buffer == "uibkafc setByModule"){
//    } else if (buffer == "uibkafc saveChannelInfos"){
//    } else if (buffer == "uibkafc setDefaultChannelInfos"){
//    } else if (buffer == "uibkafc setChannelName"){
//    } else if (buffer == "uibkafc setChannelUnits"){
//    } else if (buffer == "uibkafc setChannelSetmin"){
//    } else if (buffer == "uibkafc setChannelSetmax"){
//    } else if (buffer == "uibkafc setChannelActmin"){
//    } else if (buffer == "uibkafc setChannelActmax"){
//    } else if (buffer == "uibkafc setChannelSet"){
//    } else if (buffer == "mac"){
//    } else if (buffer == "netmask"){
//    } else if (buffer == "ip"){
//        write(localAddress().toString().toLocal8Bit());
//    } else if (buffer == "gw"){
    } else {
        qDebug()<<"command not supported or not implemented: "<<buffer;
        abort();
        return false;
    }

    buffer.clear();
    return true;

}

bool tcp_box_connection::parseArguments()
{
    //uibkafc set 2 2\r\n

    buffer.clear();
    return true;
}

void tcp_box_connection::processData()
{
    buffer.clear();
}

QString tcp_box_connection::fc_buildAll()
{
    simulated_fc_box box;
    box.init_all_fcs();

//    QString response= box.getAll_json();
    QString response=  QString(   "{\"uibk_v\":1,\"FC\":["
                                  "{\"id\":0,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_0\",\"units\":\"sccm\"},"
                                  "{\"id\":1,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_1\",\"units\":\"sccm\"},"
                                  "{\"id\":2,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_2\",\"units\":\"sccm\"},"
                                  "{\"id\":3,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_3\",\"units\":\"sccm\"},"
                                  "{\"id\":4,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_4\",\"units\":\"sccm\"},"
                                  "{\"id\":5,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_5\",\"units\":\"sccm\"},"
                                  "{\"id\":6,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_6\",\"units\":\"sccm\"},"
                                  "{\"id\":7,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0,\"name\":\"FC_7\",\"units\":\"sccm\"}"
                                  "]}");


    response += "\r\n\0";

    return response;
}

