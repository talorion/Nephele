#include "analogview.hpp"


namespace talorion {

    analogView::analogView(int entity, mode md, QWidget *par):
        QWidget(par),
        m_entity(entity),
        m_mode(md)
    {
    }

    analogView::~analogView()
    {

    }
}
