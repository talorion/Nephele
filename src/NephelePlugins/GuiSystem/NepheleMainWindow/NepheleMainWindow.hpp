#ifndef TALORION_NEPHELEMAINWINDOW_HPP
#define TALORION_NEPHELEMAINWINDOW_HPP

#include <QGridLayout>
#include <QMainWindow>
#include <QPointer>

#include "guisystem_global.hpp"

namespace talorion {

  class NepheleMainWindow : public QMainWindow
  {
    Q_OBJECT
  public:
    explicit NepheleMainWindow(QWidget *par = 0);

  private:
    QPointer<QGridLayout> m_layout;

  };

} // namespace talorion

#endif // TALORION_NEPHELEMAINWINDOW_HPP
