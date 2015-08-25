#ifndef TALORION_TCP_BOX_COM_THREAD_HPP
#define TALORION_TCP_BOX_COM_THREAD_HPP

#include <QObject>
#include <QThread>

namespace talorion {

    class tcp_box_com_thread : public QThread
    {
    public:
        tcp_box_com_thread(QObject* par=0);

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_COM_THREAD_HPP
