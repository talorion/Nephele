#ifndef TALORION_TCPBOX_FACTORY_HPP
#define TALORION_TCPBOX_FACTORY_HPP

#include <QObject>

namespace talorion {

  class tcpbox_system;

  class tcpbox_factory : public QObject
  {
    Q_OBJECT
  private:
    explicit tcpbox_factory(QObject *par = nullptr);
  public:
    // delete copy and move constructors and assign operators
    tcpbox_factory(tcpbox_factory const&) = delete;             // Copy construct
    tcpbox_factory(tcpbox_factory&&) = delete;                  // Move construct
    tcpbox_factory& operator=(tcpbox_factory const&) = delete;  // Copy assign
    tcpbox_factory& operator=(tcpbox_factory &&) = delete;      // Move assign

  public:
    static tcpbox_factory& get_instance();

    void create_new_tcpbox(tcpbox_system &sys);

    //int createNewTcpBox(QString nameVal="New Box", QString ip="localhost", quint16 port=2701);

  signals:

  public slots:

  private:

  };


} // namespace talorion

#endif // TALORION_TCPBOX_FACTORY_HPP
