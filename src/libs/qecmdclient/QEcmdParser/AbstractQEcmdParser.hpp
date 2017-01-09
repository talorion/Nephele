#ifndef ABSTRACTQECMDPARSER_HPP
#define ABSTRACTQECMDPARSER_HPP

#include <QAbstractSocket>

#include "qecmdclient_global.hpp"

namespace talorion {
class QECMDCLIENTSHARED_EXPORT AbstractQEcmdParser
{
public:
    AbstractQEcmdParser();
    bool hasEnoughData() const;
    bool noDataIsLeft() const;

    bool gatherData(const QByteArray &array);
    //AbstractQEcmdParser &	operator<<(const QAbstractSocket &array);
    QVariant parse();

    QVariant data()const;
};
}

#endif // ABSTRACTQECMDPARSER_HPP
