#ifndef SCRIPTING_THREAD_H
#define SCRIPTING_THREAD_H

#include <QThread>
#include <QScriptEngine>


namespace talorion {

    class scripting_thread : public QThread
    {
        Q_OBJECT
    public:
        explicit scripting_thread(QObject *par = 0);
        ~scripting_thread();
        Q_DISABLE_COPY(scripting_thread)

    signals:

    protected:
        virtual void run() Q_DECL_OVERRIDE;

    };
}

#endif // SCRIPTING_THREAD_H
