#ifndef TALORION_EMPTY_CONFIG_WIDGET_HPP
#define TALORION_EMPTY_CONFIG_WIDGET_HPP

#include "core/abstract_configuration_widget.hpp"

namespace talorion {

    class empty_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit empty_config_widget(QWidget *par = 0);
        ~empty_config_widget();
        Q_DISABLE_COPY(empty_config_widget)

    signals:

    public slots:

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;
    };

} // namespace talorion

#endif // TALORION_EMPTY_CONFIG_WIDGET_HPP
