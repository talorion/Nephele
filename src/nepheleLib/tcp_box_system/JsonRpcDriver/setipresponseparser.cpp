#include "setipresponseparser.hpp"


#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>
#include <tcp_box_system/JsonRpc/jsonrpcrequest.hpp>

#include <QDebug>

namespace talorion {

  void SetIPResponseParser::doConnectToBackend(abstract_backend *back)
  {
    connect(this, SIGNAL(newIpAddress(QString)),back,SLOT(newIpAddress(QString)));
  }

  bool SetIPResponseParser::doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response)
  {
    Q_UNUSED(request);
    auto var = response->result();

    if(var.canConvert<QVariantList>() == false)
      return false;
    auto varLst = var.value<QVariantList>();

    if(varLst.length() != 4)
      return false;

    QString newip;
    int i = 0;
    foreach (auto elem, varLst) {
        i++;
        if(elem.canConvert<quint8>() == false)
          return false;
        auto elemInt = elem.value<quint8>();

        newip += QString::number((quint8)elemInt);
        if(i != 4)
         newip += ".";
      }

    //qDebug()<<var;
    //qDebug()<<newip;

    emit newIpAddress(newip);

    return true;
  }


} // namespace talorion
