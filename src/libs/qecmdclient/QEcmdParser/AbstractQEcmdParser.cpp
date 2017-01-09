#include "AbstractQEcmdParser.hpp"

namespace talorion {
AbstractQEcmdParser::AbstractQEcmdParser()
{

}

bool AbstractQEcmdParser::hasEnoughData() const
{
    return false;
}

bool AbstractQEcmdParser::noDataIsLeft() const
{
    return false;
}

bool AbstractQEcmdParser::gatherData(const QByteArray &array)
{
    Q_UNUSED(array);
    return hasEnoughData() || noDataIsLeft();
}

QVariant AbstractQEcmdParser::parse()
{
    return data();
}

QVariant AbstractQEcmdParser::data() const
{
    return QVariant();
}
}
