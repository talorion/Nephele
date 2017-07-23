#ifndef TALORION_TITLELABEL_HPP
#define TALORION_TITLELABEL_HPP

#include <QLabel>

namespace talorion {

  class TitleLabel : public QLabel
  {
    Q_OBJECT
  public:
    explicit TitleLabel(QWidget *par=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
    explicit TitleLabel(const QString &text, QWidget *par=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());

  private:
    void init();
  };

} // namespace talorion

#endif // TALORION_TITLELABEL_HPP
