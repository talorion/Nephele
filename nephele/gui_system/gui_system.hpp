#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include <QObject>

#include <core/abstract_system.hpp>
namespace talorion{
    class gui_system : public QObject, public abstract_system
    {
        Q_OBJECT
    public:
        explicit gui_system(QObject *par = 0);

    signals:

    public slots:

        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
    };
}

#endif // GUI_SYSTEM_HPP
