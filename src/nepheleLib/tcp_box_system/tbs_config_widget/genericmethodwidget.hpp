#ifndef GENERICMETHODWIDGET_HPP
#define GENERICMETHODWIDGET_HPP

#include "abstractmethodwidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include "../JsonRpc/JrpcDatatype.hpp"

namespace talorion {

  class GenericMethodWidget : public AbstractMethodWidget
  {
    Q_OBJECT
  public:
    explicit GenericMethodWidget(EntityManager::EntityID box, EntityManager::EntityID meth, QWidget *par = nullptr);

  private:
      Q_DISABLE_COPY(GenericMethodWidget)

    // AbstractMethodWidget interface
  private:
    virtual void doRefreshData() Q_DECL_OVERRIDE;

    virtual QWidget *doRequestWidget() Q_DECL_OVERRIDE;

    virtual QWidget *doResponseWidget() Q_DECL_OVERRIDE;

    virtual void doSetResult(const QVariant &result) Q_DECL_OVERRIDE;

  private:
    QWidget* getGenericControlForType(JrpcDatatype type);
    QWidget* getGenericIndicatorForType(QVariant var);

    virtual QVariant doGetParams();

    QVariant getVariantFromWidget(QWidget*wdg);

  private:
    QWidget* m_requestWidget;
    QTextEdit * m_responseWidget;

    QList<QWidget*> m_paramWidgets;

  };

} // namespace talorion

#endif // GENERICMETHODWIDGET_HPP
