#include "userdatasource.hpp"

namespace talorion {

  UserDataSource::UserDataSource(QString& location, QStringList& descriptions, QVector<double>& data):
    m_location(location),
    m_descriptions(descriptions),
    m_data(data)
  {

  }

  QString UserDataSource::location() const
  {
    return m_location;
  }

  QStringList UserDataSource::descriptions() const
  {
    return m_descriptions;
  }

  QVector<double> UserDataSource::data() const
  {
    return m_data;
  }

  UserDataSource UserDataSource::updateData(const UserDataSource &rhs)
  {
    if(m_location != rhs.location())
      return *this;

    int i = 0;
    int j = 0;
    double data;
    foreach (auto desc, rhs.descriptions()) {
        i = m_descriptions.indexOf(desc);
        data = rhs.data().at(j);
        if(i >= 0){
            m_data.replace(i, data);
          }
        j++;
      }

    return *this;

  }

  UserDataSource UserDataSource::merge(const UserDataSource &rhs)
  {
    if(m_location != rhs.location())
      return *this;

    int i = 0;
    int j = 0;
    double data;
    foreach (auto desc, rhs.descriptions()) {
        i = m_descriptions.indexOf(desc);
        data = rhs.data().at(j);
        if(i >= 0){
            m_data.replace(i, data);
          }else{
            m_descriptions.append(desc);
            m_data.append(data);


          }
        j++;
      }

    return *this;
  }

  UserDataSource &UserDataSource::operator+=(const UserDataSource &rhs)
  {
    foreach (auto desc, rhs.descriptions()) {
        m_descriptions.append(desc);
      }
    foreach (auto dta, rhs.data()) {
        m_data.append(dta);
      }

    return *this; // return the result by reference
  }

  UserDataSource::operator QString() const {
    QString  ret= location();
    ret +=" ";

    int i = 0;
    foreach (auto desc, m_descriptions) {
        ret +=desc;
        ret +=":";
        ret +=QString::number(m_data.at(i));
        ret +=" ";
        i++;
      }

    return ret;
  }

} // namespace talorion
