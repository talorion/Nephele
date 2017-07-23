#ifndef TALORION_TIMECONTROL_HPP
#define TALORION_TIMECONTROL_HPP

#include <QTimeEdit>

namespace talorion {

class TimeControl : public QTimeEdit
{
    Q_OBJECT
public:
    explicit TimeControl(QWidget *par = 0);

};

} // namespace talorion

#endif // TALORION_TIMECONTROL_HPP
