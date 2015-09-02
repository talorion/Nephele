#include "tbs_config_widget.hpp"

namespace talorion {

    tbs_config_widget::tbs_config_widget(QWidget *par) :
        abstract_configuration_widget(par),
        m_label(NULL),
        mainLayout(NULL)
    {

    }

    tbs_config_widget::~tbs_config_widget()
    {

    }

} // namespace talorion

