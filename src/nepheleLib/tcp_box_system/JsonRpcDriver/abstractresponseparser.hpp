#ifndef ABSTRACTRESPONSEPARSER_HPP
#define ABSTRACTRESPONSEPARSER_HPP

#include <QDebug>
#include <QString>
#include <tcp_box_system/JsonRpc/jsonrpcrequest.hpp>
#include <tcp_box_system/abstract_backend.hpp>

namespace talorion {

  class AbstractResponseParser: public QObject
  {
    Q_OBJECT
  public:
    explicit AbstractResponseParser(QObject *par = Q_NULLPTR);
    virtual ~AbstractResponseParser(){}

    void connectToBackend(abstract_backend* back){
      if(back == Q_NULLPTR)return;
      return doConnectToBackend(back);
    }
    bool parseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse* response){
      if(request.isNull())return false;
      if(response == Q_NULLPTR)return false;
      return doParseResponse(request, response);
    }

  private:
    virtual void doConnectToBackend(abstract_backend* back)=0;
    virtual bool doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse* response)=0;
  };

} // namespace talorion

#endif // ABSTRACTRESPONSEPARSER_HPP
