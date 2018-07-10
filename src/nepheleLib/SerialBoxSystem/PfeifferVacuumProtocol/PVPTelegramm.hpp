#ifndef PVPTELEGRAMM_HPP
#define PVPTELEGRAMM_HPP

#include "PVPFactory.hpp"

#include <QtCore>

namespace talorion {

enum class PVPAction:quint8{
    PVP_READ    = 0,
    PVP_WRITE   = 10
};

class PVPTelegramm:public QObject
{
    Q_OBJECT
    friend class PVPFactory;
public:
    PVPTelegramm(QObject *par = nullptr):QObject(par){}
    ~PVPTelegramm() = default;

public:
    quint8 adress()const{return m_adress;}
    PVPAction action()const{return m_action;}
    quint16 parameterNumber()const{return m_parameterNumber;}
    quint8 dataLength()const{return m_dataLength;}
    QVariant data()const{return m_data;}
    quint8 crc8()const{return m_crc8;}

    QByteArray toTelegramm() const{return m_msg;}

    QString id()const{return QString::number((adress()<<8)+parameterNumber());}

private:
    QByteArray m_msg;

    quint8 m_adress;
    PVPAction m_action;
    quint16 m_parameterNumber;
    quint8 m_dataLength;
    QVariant m_data;
    quint8 m_crc8;

};
} // namespace talorion

#endif // PVPTELEGRAMM_HPP
