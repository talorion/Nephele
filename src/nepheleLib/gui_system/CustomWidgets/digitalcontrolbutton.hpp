#ifndef DIGITALCONTROLBUTTON_HPP
#define DIGITALCONTROLBUTTON_HPP

#include <QPushButton>

namespace talorion {

  class DigitalControlButton : public QPushButton
  {
    Q_OBJECT
  public:
    explicit DigitalControlButton(QWidget *parent = Q_NULLPTR);
    explicit DigitalControlButton(const QString &text, QWidget *parent = Q_NULLPTR);
    explicit DigitalControlButton(const QIcon &icon, const QString &text, QWidget *parent = Q_NULLPTR);
    virtual ~DigitalControlButton() = default;

  private:
    void setup();
  };

} // namespace talorion

#endif // DIGITALCONTROLBUTTON_HPP
