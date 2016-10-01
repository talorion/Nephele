#include "script_load_handler.hpp"

#include <QFile>


namespace talorion {
  script_load_handler::script_load_handler(QScriptEngine* script_engine, QObject *par)
    : abstract_scriptable_object("SCRIPT", par),
      m_script_engine(script_engine)
  {

  }

  script_load_handler::~script_load_handler()
  {

  }

  void script_load_handler::load(QString path)
  {
    QFile scriptFile(path);

    //check file is exited or not
    if(!scriptFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    //load file
    QTextStream stream(&scriptFile);
    QString s=stream.readAll();
    scriptFile.close();

    //set ScriptContext
    QScriptContext *context = m_script_engine->currentContext();
    QScriptContext *parent  = context->parentContext();
    if(parent!=0)
    {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }

    //exctue script
    QScriptValue result = m_script_engine->evaluate(s);

  }

}
