#ifndef ABSTRACTMETHODWIDGET_HPP
#define ABSTRACTMETHODWIDGET_HPP

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QVariant>

#include <core/EntityManager.hpp>

namespace talorion {

class AbstractMethodWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMethodWidget(EntityManager::EntityID box, EntityManager::EntityID method, QWidget *par = nullptr);

private:
    Q_DISABLE_COPY(AbstractMethodWidget)

public:
    EntityManager::EntityID box() const;
    EntityManager::EntityID method() const;

    void refreshData();

    QWidget *requestWidget(){return doRequestWidget();}
    QPushButton *sendButton(){return doSendButton();}
    QWidget *responseWidget(){return doResponseWidget();}

    QVariant getParams(){return doGetParams();}

    void setResult(const QVariant& result){return doSetResult(result);}

public slots:
    void receivedJsonRpcResponse(int bx, QString methodname, QVariant result , QString id );

private:
    virtual void doRefreshData() = 0;

    virtual QWidget *doRequestWidget() = 0;
    virtual QPushButton *doSendButton();
    virtual QWidget *doResponseWidget() = 0;

    virtual QVariant doGetParams() = 0;
    virtual void doSetResult(const QVariant& result) = 0;

private slots:
    void sendButtonClicked();

private:
    const EntityManager::EntityID m_box;
    const EntityManager::EntityID m_method;

private:
    QWidget* m_requestWidget;
    QPushButton* m_sendButton;
    QWidget* m_responseWidget;

    QGridLayout* m_mainLayout;
    qint32 m_id;

};

} // namespace talorion

#endif // ABSTRACTMETHODWIDGET_HPP
