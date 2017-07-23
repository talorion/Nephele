#ifndef NEPHELEDOCKWIDGET_HPP
#define NEPHELEDOCKWIDGET_HPP

#include <QDockWidget>

namespace talorion {

  class NepheleDockWidget : public QDockWidget
  {
    Q_OBJECT
  public:
    explicit NepheleDockWidget(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    explicit NepheleDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

    virtual ~NepheleDockWidget() = default;

  public slots:
    void slotSetWindowTitle(const QString& title);



  };

} // namespace talorion

#endif // NEPHELEDOCKWIDGET_HPP
