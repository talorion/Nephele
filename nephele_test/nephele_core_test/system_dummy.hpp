#ifndef SYSTEM_DUMMY_HPP
#define SYSTEM_DUMMY_HPP

#include <QObject>

#include "system_manager/abstract_system.hpp"

namespace talorion{
class system_dummy : public abstract_system
{
    Q_OBJECT
public:
    explicit system_dummy(QObject *par = 0);
    ~system_dummy();
    Q_DISABLE_COPY(system_dummy)

signals:

public slots:
};
}

#endif // SYSTEM_DUMMY_HPP
