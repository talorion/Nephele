#ifndef PVPPARAMETER_HPP
#define PVPPARAMETER_HPP

#include "PVPRequest.hpp"
#include "PVPResponse.hpp"

#include <QObject>
#include <core/EntityManager.hpp>
#include <tcp_box_system/abstract_backend.hpp>

namespace talorion {

enum class PVPParameterType{
    PVPT_AI,
    PVPT_AO,
    PVPT_AIO,
    PVPT_DI,
    PVPT_DO,
    PVPT_DIO,
    PVPT_DEV_NAME,
    PVPT_SW_VER
};

enum class PVPParameterDataType{
    PVPDT_VOID           = VT_EMPTY,
    PVPDT_REAL32         = VT_R4,
    PVPDT_STRING         = VT_BSTR,
    PVPDT_BOOL           = VT_BOOL,
    PVPDT_BOOL_ARRAY     = (VT_BOOL | VT_ARRAY)
};

class PVPParameter:public QObject
{
    Q_OBJECT
    friend class PVPFactory;
private:
    explicit PVPParameter(QObject *par = nullptr);

public:
    virtual ~PVPParameter() = default;

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

public:
  void connectToBackend(abstract_backend *back);
  bool parseResponse(QSharedPointer<PVPRequest> request, PVPResponse* response) ;

  quint16 actParameterNumber() const;

  quint16 setParameterNumber() const;

  quint8 getBoxAddress() const;

  EntityManager::EntityID getValue() const;

  QString getSetValueAsString()const;

private:
  bool isDigitalValue()const;
  bool isAnalogValue()const;

  bool parseDigitalValue(quint16 paranum, const QVariant& raw_data);
  bool parseAnalogValue(quint16 paranum, const QVariant& raw_data);
  bool parseMetaValue(quint16 paranum, const QVariant& raw_data);

  double getMaxValue()const;
  double getActValue()const;
  double getSetValue()const;

  bool getDigActValue()const;
  bool getDigSetValue()const;

private:
    //QString m_parameterName;
    quint8  m_boxAddress;
    quint16 m_actParameterNumber;
    quint16 m_setParameterNumber;
    //QString m_requestString;
    //double m_actMin;
    //double m_actMax;
    //double m_setMin;
    //double m_setMax;
    //QString m_name;
    EntityManager::EntityID m_box;
    EntityManager::EntityID m_value;
};

} // namespace talorion

#endif // PVPPARAMETER_HPP
