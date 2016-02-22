#ifndef TALORION_SYSTEM_MANAGER_HPP
#define TALORION_SYSTEM_MANAGER_HPP

#include <QObject>
#include <QSharedPointer>
#include "abstract_system.hpp"

namespace talorion {

  class abstract_system;

  class system_manager : public QObject
  {
    Q_OBJECT
  public:
    using abstract_system_t=QSharedPointer<abstract_system> ;
    using system_container_t=QList<abstract_system_t> ;

  public:
    explicit system_manager(QObject *par = nullptr);
    virtual ~system_manager()=default;

  private:
    Q_DISABLE_COPY(system_manager)

  signals:

  public slots:

  public:
    system_container_t get_all_systems()const;
    system_container_t::size_type size()const;

    template<typename T>
    abstract_system_t register_new_system();

  private:
    system_container_t m_all_systems;

  };

  template<typename T>
  system_manager::abstract_system_t system_manager::register_new_system()
  {
    static_assert(std::is_base_of<abstract_system,T>::value,"T must be derived from abstract_system!");
    abstract_system_t tmp (new T());
    m_all_systems.append(tmp);

    return tmp;
  }

} // namespace talorion

#endif // TALORION_SYSTEM_MANAGER_HPP
