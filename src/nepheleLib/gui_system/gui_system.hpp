#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include <QObject>
#include <core/abstract_system.hpp>



namespace talorion{

    class nephele_main_window;

    class gui_system_config_widget;

    class gui_system : public QObject, public abstract_system
    {
        Q_OBJECT
    public:
        explicit gui_system(QObject *par = 0);
        ~gui_system();
        Q_DISABLE_COPY(gui_system)

    signals:
        void dialog_finished();
        void dialog_finished(double val);
        void dialog_finished(QString val);

    private slots:
        void slot_open_numeric_dialog();
        void slot_open_string_dialog();
        void slot_open_file_dialog();
        void slot_open_info_dialog(const QString &msg);
        void slot_open_plot_dialog();

        void slot_box_disconnected(int boxId);

    private:
        virtual int doInitialize() Q_DECL_OVERRIDE;
        virtual int doDispose() Q_DECL_OVERRIDE;
        virtual int do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "gui_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

    private:
        virtual ActionsContainer doActions() const Q_DECL_OVERRIDE;
        virtual ToolbarsContainer doToolbars() const Q_DECL_OVERRIDE;
        virtual SystemViewsContainer doSystemViews()const Q_DECL_OVERRIDE;

    protected:
        virtual void doProvideEntityManager(EntityManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideEventManager(EventManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideSystemManager(SystemManager* service) Q_DECL_OVERRIDE;

        QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;

        virtual void doSetSystemViews(SystemViewsContainer views)Q_DECL_OVERRIDE{m_systemViews = views;}

    private:
        nephele_main_window* window;
        gui_system_config_widget* gui_system_cfg_wdgt;

        ActionsContainer m_actions;
        ToolbarsContainer m_toolbars;
        SystemViewsContainer m_systemViews;

    };
}

#endif // GUI_SYSTEM_HPP
