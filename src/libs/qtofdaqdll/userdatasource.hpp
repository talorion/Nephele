#ifndef USERDATASOURCE_HPP
#define USERDATASOURCE_HPP

#include "qtofdaqdll_global.hpp"
#include <QString>
#include <QStringList>
#include <QVector>
#include "iostream"

namespace talorion {

  class QTOFDAQDLLSHARED_EXPORT UserDataSource
  {
  public:
    UserDataSource(QString &location, QStringList &descriptions, QVector<double> &data);
    ~UserDataSource() = default;

    QString location() const;

    QStringList descriptions() const;

    QVector<double> data() const;

    UserDataSource updateData(const UserDataSource& rhs);

    UserDataSource merge(const UserDataSource& rhs);

    UserDataSource& operator+=(const UserDataSource& rhs);

    operator QString() const;

    friend UserDataSource operator+(UserDataSource lhs,const UserDataSource& rhs) {lhs += rhs; return lhs; }

  private:
    QString m_location;
    QStringList m_descriptions;
    QVector<double> m_data;
  };

  inline bool operator==(const UserDataSource& lhs, const UserDataSource& rhs){
    return lhs.location() == rhs.location();
  }

  using UserDataSourceList = QList<UserDataSource>;

} // namespace talorion

#endif // USERDATASOURCE_HPP
