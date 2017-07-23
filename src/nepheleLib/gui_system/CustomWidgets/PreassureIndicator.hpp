#ifndef TALORION_PREASSUREINDICATOR_HPP
#define TALORION_PREASSUREINDICATOR_HPP

#include <QLabel>

namespace talorion {

  class PreassureIndicator : public QLabel
  {
    Q_OBJECT
  public:
    explicit PreassureIndicator(QWidget *par=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
    explicit PreassureIndicator(const QString &text, QWidget *par=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());

  private:
    void init();
  };

} // namespace talorion

#endif // TALORION_PREASSUREINDICATOR_HPP
