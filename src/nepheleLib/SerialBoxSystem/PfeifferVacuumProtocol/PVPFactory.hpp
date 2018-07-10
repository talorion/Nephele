#ifndef PVPFACTORY_HPP
#define PVPFACTORY_HPP

#include <QByteArray>
#include <QList>

#include <core/EntityManager.hpp>

namespace talorion {

class PVPResponse;
class PVPRequest;
class PVPParameter;
enum class PVPAction:quint8;

class PVPFactory
{
private:
    PVPFactory() = delete;
    ~PVPFactory() = delete;

public:
    static QList<PVPResponse *> createResponses(const QByteArray& buffer);
    static QSharedPointer<PVPRequest> createRequest(quint8 adress, PVPAction action , quint16 parameterNumber, QVariant data, quint8 dataLength=0);
    static QSharedPointer<PVPRequest> createReadRequest(QSharedPointer<PVPParameter> parameter);
    static QSharedPointer<PVPRequest> createSetSetpointRequest(QSharedPointer<PVPParameter> parameter); 
    static QSharedPointer<PVPRequest> createReadSetPointRequest(QSharedPointer<PVPParameter> parameter);

    static QSharedPointer<PVPParameter> createAnalogPCUParameter(EntityManager::EntityID box, const QString& parameterName, QString unit,  double mx, quint8 adress, quint16 actParameterNumber, quint16 setParameterNumber = 0xFFFF);
    static QSharedPointer<PVPParameter> createDigitalPCUParameter(EntityManager::EntityID box, const QString& parameterName, quint8 adress, quint16 actParameterNumber, quint16 setParameterNumber = 0xFFFF);
    static QSharedPointer<PVPParameter> createMetadataPCUParameter(EntityManager::EntityID box, const QString& parameterName, quint8 adress, quint16 actParameterNumber, quint16 setParameterNumber = 0xFFFF);


private:
    static int readIntValue(const QByteArray &telegram, bool *ok, int idx, int len);

private:
    static int s_nextParameterId;
    static quint32 s_nextRequestId;
};

} // namespace talorion

#endif // PVPFACTORY_HPP
