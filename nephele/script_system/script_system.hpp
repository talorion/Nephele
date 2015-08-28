#ifndef SCRIPTING_THREAD_H
#define SCRIPTING_THREAD_H

#include <QThread>
#include <QScriptEngine>

#include <core/abstract_system.hpp>


namespace talorion {

    class script_system : public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        explicit script_system(QObject *par = 0);
        ~script_system();
        Q_DISABLE_COPY(script_system)

    signals:

    protected:
        virtual void run() Q_DECL_OVERRIDE;


        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
    };
}

#endif // SCRIPTING_THREAD_H
