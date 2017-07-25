#ifndef TALORION_TCP_BOX_COM_THREAD_HPP
#define TALORION_TCP_BOX_COM_THREAD_HPP

#include <QObject>
#include <QThread>
#include <QList>
//#include <QMap>

#include <core/EntityManager.hpp>
#include <core/abstract_system.hpp>

namespace talorion {

    //class tcpDriver;
    class tbs_config_widget;

    class tcp_box_system : public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        tcp_box_system(QObject* par=0);
        ~tcp_box_system();
        Q_DISABLE_COPY(tcp_box_system)

        Qt::HANDLE threadId();

    signals:
      void newSystemView(int);

    protected:
        virtual void run() Q_DECL_OVERRIDE;

       virtual void doSetSystemViews(SystemViewsContainer views)Q_DECL_OVERRIDE{m_systemViews = views;}

    private:
        virtual int doInitialize() Q_DECL_OVERRIDE;
        virtual int doDispose() Q_DECL_OVERRIDE;
        virtual int do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "tcp_box_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

        virtual ActionsContainer doActions() const Q_DECL_OVERRIDE;
        virtual ToolbarsContainer doToolbars() const Q_DECL_OVERRIDE;
        virtual SystemViewsContainer doSystemViews()const Q_DECL_OVERRIDE;

    protected:
        virtual void doProvideEntityManager(EntityManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideEventManager(EventManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideSystemManager(SystemManager* service) Q_DECL_OVERRIDE;

        QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;

    private slots:
        void newTcpBox(EntityManager::EntityID entity);
        //void tcpBoxRemoved(EntityManager::EntityID entity);

    private:
        ActionsContainer m_actions;
        ToolbarsContainer m_toolbars;
        SystemViewsContainer m_systemViews;

        Qt::HANDLE m_thread_id;
        tbs_config_widget* config_wdg;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_COM_THREAD_HPP
