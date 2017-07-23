#ifndef TALORION_DUMMYVIEW_HPP
#define TALORION_DUMMYVIEW_HPP

//#include <SystemManager/SystemView/AbstractSystemView.hpp>
#include <core/AbstractSystemView.hpp>

namespace talorion {

class DummyView : public AbstractSystemView
{
    Q_OBJECT
public:
    explicit DummyView(QWidget *par = 0);

};

} // namespace talorion

#endif // TALORION_DUMMYVIEW_HPP
