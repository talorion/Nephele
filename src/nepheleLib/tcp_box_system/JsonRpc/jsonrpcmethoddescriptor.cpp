#include "jsonrpcmethoddescriptor.hpp"


namespace talorion {

QString JsonRpcMethodDescriptor::methodName() const{
    return ent_mng().componentDataForEntity(NAME_COMPONENT, descId).toString();
}

JrpcDatatypeList JsonRpcMethodDescriptor::paramTypes() const{
    auto tmp =  ent_mng().componentDataForEntity(PARAMETER_TYPES_COMPONENT, descId);
    if(tmp.canConvert<JrpcDatatypeList>())
        return tmp.value<JrpcDatatypeList>();

    return JrpcDatatypeList();
}

qint16 JsonRpcMethodDescriptor::methodId() const{
    auto tmp =  ent_mng().componentDataForEntity(METHOD_ID_COMPONENT, descId);
    if(tmp.canConvert<qint16>())
        return tmp.value<qint16>();

    return -1;
}

QStringList JsonRpcMethodDescriptor::paramNames() const{

    QStringList m_paramNames;
    auto tmpVar =  ent_mng().componentDataForEntity(PARAMETER_NAMES_COMPONENT, descId);
    if(tmpVar.canConvert<QStringList>())
        m_paramNames= tmpVar.value<QStringList>();

    if(m_paramNames.isEmpty() == false)
        return m_paramNames;

    QStringList tmp;
    if(paramTypes().isEmpty())
        return tmp;

    for(int i = 0; i<paramTypes().size(); i++){
        tmp.append(QString("arg_%1").arg(i));
    }
    return tmp;
}

JsonRpcMethodDescriptor::JsonRpcMethodDescriptor(EntityManager::EntityID id):
    //m_methodName(),
    //m_paramTypes(),
    //m_methodId(-1)
    descId(id)
{
    //descId =

}


} // namespace talorion
