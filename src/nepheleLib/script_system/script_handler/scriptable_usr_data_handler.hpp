#ifndef TALORION_SCRIPTABLE_USR_DATA_HANDLER_HPP
#define TALORION_SCRIPTABLE_USR_DATA_HANDLER_HPP

#include "core/abstract_scriptable_object.hpp"

namespace talorion {

  class scriptable_usr_data_handler : public abstract_scriptable_object
  {
    Q_OBJECT
  public:
    explicit scriptable_usr_data_handler(QObject *par = 0);

    Q_INVOKABLE void create_usr_val(QString name);

    Q_INVOKABLE void set_usr_val(QString name, double value);

  signals:
      void set_value_changed(int entity, double set_val);

  private slots:
      void slot_set_value_changed(int entity);

  };

} // namespace talorion

#endif // TALORION_SCRIPTABLE_USR_DATA_HANDLER_HPP
