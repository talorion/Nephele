#include "boxnameresponseparser.hpp"

#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

namespace talorion {

  BoxNameResponseParser::BoxNameResponseParser(QObject *parent) : AbstractResponseParser(parent)
  {

  }

  void BoxNameResponseParser::doConnectToBackend(abstract_backend *back)
  {
   connect(this,SIGNAL(updateName(QString)), back, SLOT(updateName(QString)));
  }

  bool BoxNameResponseParser::doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response)
  {
    Q_UNUSED(request);

    auto var = response->result();

    QVariantList nameAsArray;
    if(var.canConvert<QVariantList>() == true){
        nameAsArray= var.value<QVariantList>();
      }

    QString name;
    if(nameAsArray.isEmpty() == false){
        auto nameVar = nameAsArray.at(0);
        if(nameVar.canConvert<QString>() ){
            name= nameVar.value<QString>();
          }
      }else if(var.canConvert<QString>() == true){
        name= var.value<QString>();
      }else{
        return false;
      }

    if(name.isNull() || name.isEmpty())
      return false;

    emit updateName(name);
    return true;


  }

} // namespace talorion
