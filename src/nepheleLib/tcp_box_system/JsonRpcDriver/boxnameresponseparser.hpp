#ifndef BOXNAMERESPONSEPARSER_HPP
#define BOXNAMERESPONSEPARSER_HPP

#include "abstractresponseparser.hpp"

namespace talorion {

  class BoxNameResponseParser : public AbstractResponseParser
  {
    Q_OBJECT
  public:
    explicit BoxNameResponseParser(QObject *parent = nullptr);
    virtual ~BoxNameResponseParser(){}

  signals:
    void updateName(QString set);

    // AbstractResponseParser interface
  private:
    virtual void doConnectToBackend(abstract_backend *back) Q_DECL_OVERRIDE;
    virtual bool doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // BOXNAMERESPONSEPARSER_HPP
