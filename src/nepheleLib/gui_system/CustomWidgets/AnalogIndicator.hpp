#ifndef TALORION_ANALOGINDICATOR_HPP
#define TALORION_ANALOGINDICATOR_HPP

#include <QDoubleSpinBox>

namespace talorion {

  class AnalogIndicator : public QDoubleSpinBox
  {
    Q_OBJECT
  public:
    explicit AnalogIndicator(QString unit = QString(), int decimals=2, QWidget *par = 0);
    explicit AnalogIndicator(int min, int max, QString unit = QString(), QWidget *par = 0);

    void setPointSize(int sz);
    void setPixelSize(int sz);

  signals:
    void entered();
    void left();

  private:
    void setup(int min, int max, QString unit, int dec=2);

    // QWidget interface
  protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_ANALOGINDICATOR_HPP
