#ifndef TALORION_ABSTRACTQECMDCOMMAND_HPP
#define TALORION_ABSTRACTQECMDCOMMAND_HPP

#include <QString>
//#include "qecmdclient_global.hpp"


namespace talorion {

//class QECMDCLIENTSHARED_EXPORT AbstractQEcmdCommand
class AbstractQEcmdCommand
{
public:
    AbstractQEcmdCommand(QString _cmd =QString(), int _timeout =-1);
    AbstractQEcmdCommand & operator= ( const AbstractQEcmdCommand & lhs) = default;

    bool operator ==(const AbstractQEcmdCommand &b) const;
    bool operator !=(const AbstractQEcmdCommand &b) const;

    QString cmdStr() const;

    int cmdLen()const;

    int timeout() const;

    QByteArray toByteArray() const;

    QString toString() const;

private:
    void setCmdStr(const QString &cmdStr);

private:
    QString m_cmdStr;
    int m_timeout;
};

} // namespace talorion

#endif // TALORION_ABSTRACTQECMDCOMMAND_HPP
