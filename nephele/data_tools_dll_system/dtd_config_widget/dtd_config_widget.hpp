#ifndef DTD_CONFIG_WIDGET_HPP
#define DTD_CONFIG_WIDGET_HPP

#include "core/abstract_configuration_widget.hpp"

namespace talorion {
    class dtd_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit dtd_config_widget(QWidget *par = 0);
        ~dtd_config_widget();
        Q_DISABLE_COPY(dtd_config_widget)

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;

    };
}

#endif // DTD_CONFIG_WIDGET_HPP
