#include "getallmethodsresponseparser.hpp"

#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

namespace talorion {

  void GetAllMethodsResponseParser::doConnectToBackend(abstract_backend *back)
  {
    connect(this, SIGNAL(newJsonRpcMethod(QString,JrpcDatatypeList,QStringList)), back, SLOT(newJsonRpcMethod(QString,JrpcDatatypeList,QStringList)));
  }

  bool GetAllMethodsResponseParser::doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response)
  {
    Q_UNUSED(request);

    auto var = response->result();

    if(var.canConvert<QVariantList>() == false)
      return false;
    auto varList = var.value<QVariantList>();

    foreach (auto listitem, varList) {

      auto obj = JsonRpcFactory::createMethodDescriptorObject(listitem);
      if(obj == Q_NULLPTR)
        return false;

      auto methodname = obj->methodname;
      auto paramTypes = obj->paramTypes;
      auto paramNames = obj->paramNames;

      emit newJsonRpcMethod(methodname, paramTypes, paramNames);

      delete obj;
    }
    return true;
  }


} // namespace talorion
