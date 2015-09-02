#ifndef TALORION_TBS_CONFIG_WIDGET_HPP
#define TALORION_TBS_CONFIG_WIDGET_HPP

#include <QGridLayout>
#include <QLabel>
#include "core/abstract_configuration_widget.hpp"

namespace talorion {

    class tbs_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit tbs_config_widget(QWidget *par = 0);
        ~tbs_config_widget();
        Q_DISABLE_COPY(tbs_config_widget)

    signals:

    public slots:

    private:
        QLabel* m_label;
        QGridLayout* mainLayout;
    };

} // namespace talorion

#endif // TALORION_TBS_CONFIG_WIDGET_HPP
