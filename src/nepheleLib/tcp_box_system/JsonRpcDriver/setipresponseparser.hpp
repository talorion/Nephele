#ifndef SETIPRESPONSEPARSER_HPP
#define SETIPRESPONSEPARSER_HPP

#include "abstractresponseparser.hpp"

namespace talorion {

  class SetIPResponseParser : public AbstractResponseParser
  {
    Q_OBJECT
  public:
    explicit SetIPResponseParser(QObject *par = nullptr):AbstractResponseParser(par){}
    virtual ~SetIPResponseParser() = default;

  signals:
    void newIpAddress(QString ip);

    // AbstractResponseParser interface
  private:
    virtual void doConnectToBackend(abstract_backend *back) Q_DECL_OVERRIDE;
    virtual bool doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // SETIPRESPONSEPARSER_HPP
