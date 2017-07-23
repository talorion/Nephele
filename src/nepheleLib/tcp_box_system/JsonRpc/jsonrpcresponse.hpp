#ifndef JSONRPCRESPONSE_HPP
#define JSONRPCRESPONSE_HPP

#include <QByteArray>
#include <QDebug>
#include <QVariantMap>

namespace talorion {

struct JsonRpcError{
    JsonRpcError():m_code(0), m_message(), m_data(){}
    JsonRpcError(qint32 code, QString message, QVariantMap data):m_code(code), m_message(message), m_data(data){}

    qint32 m_code;
    QString m_message;
    QVariantMap m_data;
};

inline bool operator==(const JsonRpcError& lhs, const JsonRpcError& rhs){
    return lhs.m_code ==  rhs.m_code &&
            lhs.m_message ==  rhs.m_message;
}

inline bool operator!=(const JsonRpcError& lhs, const JsonRpcError& rhs){ return !(lhs == rhs); }

class JsonRpcResponse
{
    friend class JsonRpcFactory;

public:
    ~JsonRpcResponse(){ if (m_error != Q_NULLPTR){delete m_error;m_error =Q_NULLPTR;}}

private:
    JsonRpcResponse(const QByteArray &rdta);
    JsonRpcResponse( const JsonRpcResponse& ) = delete;
    JsonRpcResponse& operator=( const JsonRpcResponse& ) = delete;

public:
    bool isValid()const{return result().isValid() && isError()==false;}
    bool isError()const{return m_error != Q_NULLPTR;}

    QByteArray rawData()const{return m_rawData;}
    QVariant result() const  {return m_result;}
    QString id() const  {return m_id;}
    JsonRpcError *error() const  {return m_error;}

private:
    void setResult(const QVariant &result)  {m_result = result;}
    void setId(const QString &id) {m_id = id;}
    void setError(JsonRpcError *error)  {m_error = error;}

private:
    const QByteArray m_rawData;
    QVariant m_result;
    QString m_id;
    JsonRpcError* m_error;
};

} // namespace talorion

Q_DECLARE_METATYPE(talorion::JsonRpcError)
Q_DECLARE_METATYPE(talorion::JsonRpcError*)

#endif // JSONRPCRESPONSE_HPP
