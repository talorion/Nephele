#include "SerialBoxesWorker.hpp"
#include "SerialJsonRpcDriver.hpp"

#include "core/EntityManager.hpp"

namespace talorion {

SerialBoxesWorker::SerialBoxesWorker(QObject *parent) : QObject(parent)
{
    connect(EventManager::get_instance(),SIGNAL(connect_tcp_box(int)),this,SLOT(slotConnectSerialBox(int)), Qt::QueuedConnection);
    connect(EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this,SLOT(slotDisconnectSerialBox(int)), Qt::QueuedConnection);
    connect(EventManager::get_instance(),SIGNAL(tcpBoxRemoved(int)), this, SLOT(serialBoxRemoved(int)));

    connect(this,SIGNAL(serialBoxDisconnected(int)),EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)), Qt::QueuedConnection);

    foreach (int box, EntityManager::get_instance()->get_all_tcpBoxes()) {
        EntityManager::get_instance()->setComponentDataForEntity(AUTO_RECONNECT_COMPONENT, box, QVariant::fromValue(true));
        slotConnectSerialBox(box);
    }
}

void SerialBoxesWorker::connectToSerialBox(int entity) const
{
    int trans = EntityManager::get_instance()->getTransportComponent(entity);
    if(trans != 1 )
        return;

    auto ent_mng = EntityManager::get_instance();
    if(ent_mng->isValid(entity) == false)
        return;

    auto all_boxes = ent_mng->get_all_tcpBoxes();
    if(all_boxes.contains(entity) == false)
        return;

    auto evt_mng =  EventManager::get_instance();
    emit evt_mng->connect_tcp_box(entity);
}

void SerialBoxesWorker::slotConnectSerialBox(int entity)
{
    int trans = EntityManager::get_instance()->getTransportComponent(entity);
    if(trans != 1 )
        return;

    int mode = EntityManager::get_instance()->get_tcp_box_backend_component(entity);

    auto it = boxes.find(entity);
    if(it == boxes.end()){
        switch(mode){
        case 0:{break;}
        case 1:{break;}
        case 2:{break;}
        case 3:{connectToBreitiBox(entity);break;}
        }
    }
}

void SerialBoxesWorker::slotDisconnectSerialBox(int entity)
{
    auto it = boxes.find(entity);
    if(it == boxes.end())
        return;

    auto dev1 = it.value();
    if(!dev1)
        return;

    dev1->disconectDevice();
    boxes.remove(entity);
    delete dev1;
}

void SerialBoxesWorker::slotSerialBoxDisconnected(int entity)
{
  Q_UNUSED(entity);
}

void SerialBoxesWorker::reconnectAllBoxes()
{

}

void SerialBoxesWorker::serialBoxRemoved(int boxId)
{
    auto it = boxes.find(boxId);
    if(it == boxes.end())
        return;

    auto drv = it.value();
    boxes.remove(boxId);
    delete drv;
}

void SerialBoxesWorker::connectToBreitiBox(int box_id)
{
    AbstractSerialDriver* dev1;
    auto it = boxes.find(box_id);
    if(it == boxes.end())
        dev1 = new SerialJsonRpcDriver(box_id);
    else
        dev1 = it.value();

    dev1->connectDevice();
    boxes.insert(box_id,dev1);
}

} // namespace talorion
