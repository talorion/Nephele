#include "TimeControl.hpp"

namespace talorion {

TimeControl::TimeControl(QWidget *par) :
    QTimeEdit(par)
{
    setButtonSymbols(QTimeEdit::NoButtons);
    //setStyleSheet("QTimeEdit {  border-radius: 5px;}");
    setDisplayFormat("HH:mm:ss.zzz");

    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

}

} // namespace talorion
