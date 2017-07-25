#ifndef TALORION_QUICK_SCRIPT_WIDGET_HPP
#define TALORION_QUICK_SCRIPT_WIDGET_HPP

#include <QGridLayout>
#include <QMap>
#include <QPushButton>
#include <QSignalMapper>
#include <QWidget>

namespace talorion {

  class quick_script_widget : public QWidget
  {
    Q_OBJECT
  public:
    explicit quick_script_widget(QWidget *par = 0);
    virtual ~quick_script_widget() = default;

  private:
    quick_script_widget( const quick_script_widget& other ) = delete; // non construction-copyable
    quick_script_widget& operator=( const quick_script_widget& ) = delete; // non copyable

  signals:
   void start_script_file(const QString& script);

  private slots:
    void clicked(const int &id);
    void add_button(int id);
    void remove_button(int id);

  private:
    QSignalMapper *signalMapper;
    QGridLayout *gridLayout;
    QMap<int, QPushButton *> buttons;


  };

} // namespace talorion

#endif // TALORION_QUICK_SCRIPT_WIDGET_HPP
