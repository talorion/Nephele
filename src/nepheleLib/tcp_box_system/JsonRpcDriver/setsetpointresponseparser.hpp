#ifndef SETSETPOINTRESPONSEPARSER_HPP
#define SETSETPOINTRESPONSEPARSER_HPP

#include "abstractresponseparser.hpp"

namespace talorion {

  class SetSetpointResponseParser: public AbstractResponseParser
  {
    Q_OBJECT
  public:
    explicit SetSetpointResponseParser(QObject *par = Q_NULLPTR):AbstractResponseParser(par){}
    virtual ~SetSetpointResponseParser() = default;

  signals:
    void updateSetValue(int id, double set, qint32 requestId);

    // AbstractResponseParser interface
  private:
    virtual void doConnectToBackend(abstract_backend *back) Q_DECL_OVERRIDE;
    virtual bool doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // SETSETPOINTRESPONSEPARSER_HPP
