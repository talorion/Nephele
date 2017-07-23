#ifndef JSONRPCMETHODDESCRIPTOR_HPP
#define JSONRPCMETHODDESCRIPTOR_HPP

#include <QList>
#include <QString>
#include <core/ManagerLocator.hpp>
#include "JrpcDatatype.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

  class JsonRpcMethodDescriptor
  {
    friend class JsonRpcFactory;
    friend class JsonRpcDriverUtils;

  public:
    QString methodName() const;
    JrpcDatatypeList paramTypes() const;
    qint16 methodId() const;
    QStringList paramNames() const;

  //private:
  //  void setMethodName(const QString &methodName){m_methodName = methodName;}
  //  void setParamTypes(const JrpcDatatypeList &paramTypes){m_paramTypes = paramTypes;}
  //  void setParamNames(const QStringList &paramNames){m_paramNames = paramNames;}
  //  void setMethodId(const qint16 &methodId){m_methodId = methodId;}

  private:
    JsonRpcMethodDescriptor(EntityManager::EntityID id);
    JsonRpcMethodDescriptor( const JsonRpcMethodDescriptor& ) = delete;
    JsonRpcMethodDescriptor operator=( const JsonRpcMethodDescriptor&& ) = delete;
    JsonRpcMethodDescriptor& operator=( const JsonRpcMethodDescriptor& ) = delete;
    ~JsonRpcMethodDescriptor() = default ;

    inline EntityManager& ent_mng()const{return ManagerLocator::entityManager();}

  private:
    //QString m_methodName;
    //JrpcDatatypeList m_paramTypes;
    //QStringList m_paramNames;
    //qint16 m_methodId;
    const EntityManager::EntityID descId;
  };

  inline bool operator==(const JsonRpcMethodDescriptor& lhs, const JsonRpcMethodDescriptor& rhs){
      return
              lhs.methodName() == rhs.methodName() &&
              lhs.paramTypes() == lhs.paramTypes() &&
              lhs.paramNames() == lhs.paramNames();
  }
  inline bool operator!=(const JsonRpcMethodDescriptor& lhs, const JsonRpcMethodDescriptor& rhs){ return !(lhs == rhs); }

} // namespace talorion

#endif // JSONRPCMETHODDESCRIPTOR_HPP
