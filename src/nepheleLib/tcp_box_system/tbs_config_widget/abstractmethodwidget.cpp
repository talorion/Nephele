#include "abstractmethodwidget.hpp"


#include <tcp_box_system/JsonRpc/jsonrpcfactory.hpp>

#include <core/EventManager.hpp>

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

AbstractMethodWidget::AbstractMethodWidget(EntityManager::EntityID box, EntityManager::EntityID method, QWidget *par) :
    QWidget(par),
    m_box(box),
    m_method(method),
    m_requestWidget(Q_NULLPTR),
    m_sendButton(Q_NULLPTR),
    m_responseWidget(Q_NULLPTR),
    m_mainLayout(Q_NULLPTR),
    m_id(0)
{
    connect(EventManager::get_instance(),SIGNAL(receivedJsonRpcResponse(int,QString,QVariant,QString)),this,SLOT(receivedJsonRpcResponse(int,QString,QVariant,QString)));

}

void AbstractMethodWidget::refreshData(){

    m_mainLayout = new QGridLayout();
    m_mainLayout->setMargin(GuiConstantProvider::margin());
    m_mainLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_mainLayout->setVerticalSpacing(0);

    if(requestWidget() != Q_NULLPTR)
        m_mainLayout->addWidget(requestWidget());

    if(sendButton() != Q_NULLPTR)
        m_mainLayout->addWidget(sendButton());

    if(responseWidget() != Q_NULLPTR)
        m_mainLayout->addWidget(responseWidget());

    setLayout(m_mainLayout);

    if(sendButton() != Q_NULLPTR)
        connect(sendButton(), SIGNAL(clicked(bool)), this, SLOT(sendButtonClicked()));

    return doRefreshData();
}

void AbstractMethodWidget::receivedJsonRpcResponse(int bx, QString methodname, QVariant result, QString id)
{
    Q_UNUSED(methodname)
    Q_UNUSED(id)

    if(bx != box())
        return;

    setResult(result);
}

EntityManager::EntityID AbstractMethodWidget::box() const
{
    return m_box;
}

EntityManager::EntityID AbstractMethodWidget::method() const
{
    return m_method;
}

QPushButton *AbstractMethodWidget::doSendButton()
{
    if(m_sendButton == Q_NULLPTR){
        m_sendButton = new QPushButton("SEND");
    }
    return m_sendButton;
}

void AbstractMethodWidget::sendButtonClicked()
{
    //auto requ = JsonRpcFactory::createRequest(method(), getParams());
    QString methodname  = EntityManager::get_instance()->get_name_component(method());
    QVariant params     = getParams();
    qint32 id           = qrand() % 0xFFFF;

    emit EventManager::get_instance()->sendJsonRpcRequest(box(), methodname, params, id);

}

} // namespace talorion
