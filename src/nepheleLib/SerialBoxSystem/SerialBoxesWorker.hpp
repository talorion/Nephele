#ifndef SERIALBOXESWORKER_HPP
#define SERIALBOXESWORKER_HPP

#include "AbstractSerialDriver.hpp"

#include <QObject>

#include <tcp_box_system/abstract_backend.hpp>

namespace talorion {

class SerialBoxesWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialBoxesWorker(QObject *parent = nullptr);
    virtual ~SerialBoxesWorker()=default;
    Q_DISABLE_COPY(SerialBoxesWorker)

    void connectToSerialBox(int entity)const;

signals:
    void serialBoxDisconnected(int entity);
    void serialBoxConnected(int entity);

private slots:
    void slotConnectSerialBox(int entity);
    void slotDisconnectSerialBox(int entity);

    void slotSerialBoxDisconnected(int entity);
    void reconnectAllBoxes();

    void serialBoxRemoved(int boxId);

private:
    //int newBoxId();
    void connectToBreitiBox(int box_id);
    //void connectToPCUBox(int box_id);

private:
    int curr_box_id;
    QMap<int, AbstractSerialDriver*> boxes;
    //QList<abstract_backend*> bkends;
    QTimer* reconnectTimer;
};

} // namespace talorion

#endif // SERIALBOXESWORKER_HPP
