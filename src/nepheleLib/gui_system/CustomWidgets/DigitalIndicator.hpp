#ifndef TALORION_DIGITALINDICATOR_HPP
#define TALORION_DIGITALINDICATOR_HPP

#include <QGridLayout>
#include <QLabel>
#include <QPointer>

namespace talorion {

  class DigitalIndicator : public QLabel
  {
    Q_OBJECT
  public:
    //explicit DigitalIndicator(QString truestring=QString("HI"), QString falsestring=QString("LO"),QWidget *par = 0);

    explicit DigitalIndicator(QString truestring=QString("HI"), QString falsestring=QString("LO"), QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    //explicit DigitalControlLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~DigitalIndicator()= default;

    void setPointSize(int sz);
    void setPixelSize(int sz);

  signals:
    void entered();
    void left();

  public slots:
    void setValue(bool newval);

  private:
//    QPointer<QGridLayout> m_layout;
//    QPointer<QLabel> m_lbl;
    QString m_true_string;
    QString m_false_string;

    // QWidget interface
  protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_DIGITALINDICATOR_HPP
