#ifndef TALORION_DIGITALCONTROL_HPP
#define TALORION_DIGITALCONTROL_HPP

#include "digitalcontrolbutton.hpp"
#include "digitalcontrollabel.hpp"

#include <QGridLayout>
#include <QPointer>
#include <QWidget>

namespace talorion {

  class DigitalControl : public QWidget
  {
    Q_OBJECT
  public:
    explicit DigitalControl(QString truestring=QString("HI"), QString falsestring=QString("LO"),QWidget *par = 0);

    bool value() const;

  signals:
    void valueChanged(bool newval);
    void entered();
    void left();

  public slots:
    void setValue(bool newval);

  private slots:
    void button_pressed(bool checked);

  private:
    QPointer<QGridLayout> m_layout;
    QPointer<DigitalControlButton> m_butt;
    QPointer<DigitalControlLabel> m_lbl;
    QString m_true_string;
    QString m_false_string;

    // QWidget interface
  protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_DIGITALCONTROL_HPP
