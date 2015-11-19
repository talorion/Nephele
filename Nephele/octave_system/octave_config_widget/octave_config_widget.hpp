#ifndef OCTAVE_CONFIG_WIDGET_HPP
#define OCTAVE_CONFIG_WIDGET_HPP


#include "abstract_configuration_widget.hpp"

namespace talorion{

    class octave_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit octave_config_widget(QWidget *par = 0);
        ~octave_config_widget();
        Q_DISABLE_COPY(octave_config_widget)


        // abstract_configuration_widget interface
    private:
        virtual void do_refresh_data();
    };

}

#endif // OCTAVE_CONFIG_WIDGET_HPP
