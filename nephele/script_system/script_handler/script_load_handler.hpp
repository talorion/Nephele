#ifndef SCRIPT_LOAD_HANDLER_HPP
#define SCRIPT_LOAD_HANDLER_HPP

#include "core/abstract_scriptable_object.hpp"
#include <QScriptContext>
#include <QScriptEngine>
#include <QTextStream>

namespace talorion {
  class script_load_handler : public abstract_scriptable_object
  {
    Q_OBJECT
  public:
    explicit script_load_handler(QScriptEngine* script_engine, QObject *par = 0);
    ~script_load_handler();
    Q_DISABLE_COPY(script_load_handler)

    Q_INVOKABLE void load(QString path);

  private:
    QScriptEngine* m_script_engine;

  };
}

#endif // SCRIPT_LOAD_HANDLER_HPP
