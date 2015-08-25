#ifndef TCP_BOX_SIMULATOR_HPP
#define TCP_BOX_SIMULATOR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>


class tcp_box_simulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit tcp_box_simulator(QWidget *par = 0);
    ~tcp_box_simulator();

private:
    QHBoxLayout m_layout;
    QLabel m_label;
};

#endif // TCP_BOX_SIMULATOR_HPP
