#include "analogview.hpp"


namespace talorion {

    analogView::analogView(int entity, QWidget *par):
        QWidget(par),
        m_entity(entity)
    {
    }

    analogView::~analogView()
    {

    }
}
