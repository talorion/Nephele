#ifndef TALORION_SCRIPT_CONFIG_WIDGET_HPP
#define TALORION_SCRIPT_CONFIG_WIDGET_HPP

#include <QGridLayout>
#include <QScriptValue>
#include <QTreeWidget>

#include "core/abstract_configuration_widget.hpp"

namespace talorion {

    class script_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit script_config_widget(QWidget *par = 0);
        ~script_config_widget();
        Q_DISABLE_COPY(script_config_widget)

    private:
        void update_script_tree();
        void init_engine(int entity);

        virtual void do_refresh_data() Q_DECL_OVERRIDE;

        QList<QTreeWidgetItem *> rec_get_values(QScriptValue object);

    signals:

    public slots:

    private:
        QGridLayout* mainLayout;
        QTreeWidget* script_object_tree;
        int my_engine;
        QList<QTreeWidgetItem*> treeitems;

    };

} // namespace talorion

#endif // TALORION_SCRIPT_CONFIG_WIDGET_HPP
