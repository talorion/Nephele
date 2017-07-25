#ifndef TALORION_ANALOGCONTROL_HPP
#define TALORION_ANALOGCONTROL_HPP

#include <QDoubleSpinBox>

namespace talorion {

  class AnalogControl : public QDoubleSpinBox
  {
    Q_OBJECT
  public:
    explicit AnalogControl(QString unit = QString(), int decimals=2, QWidget *par = 0);
    explicit AnalogControl(int min, int max, QString unit = QString(), QWidget *par = 0);

  signals:
    void entered();
    void left();

  public slots:
    void setValue(double val);

  private:
    void setup(int min, int max, QString unit, int dec=2);


    // QWidget interface
  protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_ANALOGCONTROL_HPP
