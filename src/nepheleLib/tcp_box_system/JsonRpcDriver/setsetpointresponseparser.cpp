#include "setsetpointresponseparser.hpp"

#include <QDebug>

#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

namespace talorion {

  void SetSetpointResponseParser::doConnectToBackend(abstract_backend *back)
  {
    connect(this, SIGNAL(updateSetValue(int,double, qint32)), back,SLOT(updateSetValue(int,double, qint32)));
  }

  bool SetSetpointResponseParser::doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response)
  {
    Q_UNUSED(request);
    auto var = response->result();

    //qDebug()<<request->toJson()<<" returned: "<< var;
    if(var.canConvert<QVariantList>() == false)
      return false;
    auto varList= var.value<QVariantList>();

    if(varList.size() != 2)
      return false;

    auto varId = varList.at(0);
    if(varId.canConvert<int>() == false)
      return false;
    auto id= varId.value<int>();

    auto valId = varList.at(1);
    if(valId.canConvert<double>() == false)
      return false;
    auto set= valId.value<double>();

    emit updateSetValue(id, set, request->requestId());

    return true;
  }

} // namespace talorion
