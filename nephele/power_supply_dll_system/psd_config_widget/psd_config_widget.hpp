#ifndef TALORION_PSD_CONFIG_WIDGET_HPP
#define TALORION_PSD_CONFIG_WIDGET_HPP

#include "core/abstract_configuration_widget.hpp"

namespace talorion {

    class psd_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit psd_config_widget(QWidget *par = 0);
        ~psd_config_widget();
        Q_DISABLE_COPY(psd_config_widget)

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;
    };

} // namespace talorion

#endif // TALORION_PSD_CONFIG_WIDGET_HPP
