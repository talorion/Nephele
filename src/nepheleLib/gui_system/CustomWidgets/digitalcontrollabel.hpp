#ifndef DIGITALCONTROLLABEL_HPP
#define DIGITALCONTROLLABEL_HPP

#include <QLabel>

namespace talorion {

  class DigitalControlLabel : public QLabel
  {
    Q_OBJECT
  public:
    explicit DigitalControlLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    explicit DigitalControlLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~DigitalControlLabel()= default;

  private:
    void setup();

  };

} // namespace talorion

#endif // DIGITALCONTROLLABEL_HPP
