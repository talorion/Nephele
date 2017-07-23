#ifndef GETALLMETHODSRESPONSEPARSER_HPP
#define GETALLMETHODSRESPONSEPARSER_HPP

#include "abstractresponseparser.hpp"

namespace talorion {

  class GetAllMethodsResponseParser : public AbstractResponseParser
  {
    Q_OBJECT
  public:
    explicit GetAllMethodsResponseParser(QObject *par = Q_NULLPTR):AbstractResponseParser(par){}
    virtual ~GetAllMethodsResponseParser() = default;

  signals:
    void newJsonRpcMethod(const QString &methodname, const JrpcDatatypeList &paramTypes, const QStringList &paramNames);

    // AbstractResponseParser interface
  private:
    virtual void doConnectToBackend(abstract_backend* back) Q_DECL_OVERRIDE;
    virtual bool doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // GETALLMETHODSRESPONSEPARSER_HPP
