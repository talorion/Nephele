#ifndef ABSTRACT_SYSTEM_HPP
#define ABSTRACT_SYSTEM_HPP

#include <QObject>

namespace talorion {
class abstract_system : public QObject
{
    Q_OBJECT
public:
    explicit abstract_system(QObject *par = 0);
    ~abstract_system();
    Q_DISABLE_COPY(abstract_system)

signals:

public slots:
};
}

#endif // ABSTRACT_SYSTEM_HPP
