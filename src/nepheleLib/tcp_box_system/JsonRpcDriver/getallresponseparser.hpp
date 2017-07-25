#ifndef GETALLRESPONSEPARSER_HPP
#define GETALLRESPONSEPARSER_HPP

#include "abstractresponseparser.hpp"

namespace talorion {

  class GetAllResponseParser : public AbstractResponseParser
  {
    Q_OBJECT
  public:
    explicit GetAllResponseParser(QObject *par = Q_NULLPTR);
    virtual ~GetAllResponseParser() = default;

  signals:
    void newAO(int id, QString name, QString units, double set, double smin, double smax, QVariant metadata);
    void newAI(int id, QString name, QString units, double act, double amin, double amax, QVariant metadata);
    void newAIO(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, QVariant metadata);

    void newFC(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, double T, int read_counter, int error_counter, QVariant metadata);
    void newAV(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, QVariant metadata);

    void newDO(int id, QString name, bool set, QVariant metadata);
    void newDI(int id, QString name, bool act, QVariant metadata);
    void newDIO(int id, QString name, bool act, bool set, QVariant metadata);

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

    QList<int> aoIds;
    QList<int> aiIds;
    QList<int> aioIds;

    QList<int> fcIds;
    QList<int> avIds;

    QList<int> doIds;
    QList<int> diIds;
    QList<int> dioIds;

  };

} // namespace talorion

#endif // GETALLRESPONSEPARSER_HPP
