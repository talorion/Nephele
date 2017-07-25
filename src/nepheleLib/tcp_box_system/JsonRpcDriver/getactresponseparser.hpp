#ifndef GETACTRESPONSEPARSER_HPP
#define GETACTRESPONSEPARSER_HPP

#include "abstractresponseparser.hpp"

namespace talorion {
  class GetActResponseParser : public AbstractResponseParser
  {
    Q_OBJECT
  public:
    explicit GetActResponseParser(QObject *par = nullptr);
    virtual ~GetActResponseParser() = default;

  signals:
    void updateAO(int id, double set);
    void updateAI(int id,  double act);
    void updateAIO(int id, double set, double act);

    void updateFC(int id, double set, double act);
    void updateAV(int id, double set, double act);

    void updateDO(int id, bool set);
    void updateDI(int id, bool act);
    void updateDIO(int id, bool act, bool set);

    void finishedParsing(QList<int> allIds);

    // AbstractResponseParser interface
  private:
    virtual void doConnectToBackend(abstract_backend *back) Q_DECL_OVERRIDE;
    virtual bool doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response) Q_DECL_OVERRIDE;

    void parseAO(QVariant var);
    void parseAI(QVariant var);
    void parseAIO(QVariant var);

    void parseFC(QVariant var);
    void parseAV(QVariant var);

    void parseDO(QVariant var);
    void parseDI(QVariant var);
    void parseDIO(QVariant var);

    void clearAllIds();

  private:
    QList<int> allIds;
  };
}

#endif // GETACTRESPONSEPARSER_HPP
