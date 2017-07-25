#include "genericmethodwidget.hpp"


#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>

#include <tcp_box_system/JsonRpc/jsonrpcfactory.hpp>
#include <tcp_box_system/JsonRpc/jsonrpcmethoddescriptor.hpp>

#include <cstdint>
#include <QDebug>

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {


  GenericMethodWidget::GenericMethodWidget(EntityManager::EntityID box, EntityManager::EntityID meth, QWidget *par):
    AbstractMethodWidget(box, meth, par),
    m_requestWidget(Q_NULLPTR),
    m_responseWidget(Q_NULLPTR),
    m_paramWidgets()
  {
    auto desc = JsonRpcFactory::createMethodDescriptor(method());
    if(desc == Q_NULLPTR)
      return;

    auto names = desc->paramNames();
    auto types = desc->paramTypes();

    m_requestWidget = new QWidget();
    QGridLayout* requestLayout = new QGridLayout();

    requestLayout->setMargin(GuiConstantProvider::margin());
    requestLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    requestLayout->setVerticalSpacing(0);

    int i =0;
    foreach (auto name, names) {
        requestLayout->addWidget(new QLabel(name), ++i,0,1,1);
      }

    i =0;
    m_paramWidgets.clear();
    foreach (auto type, types) {
        auto tmp =getGenericControlForType(type);
        requestLayout->addWidget(tmp, ++i,1,1,1);
        m_paramWidgets.append(tmp);
      }

    m_requestWidget->setLayout(requestLayout);

    //    m_responseWidget = new QWidget();
    //    m_responseWidget->setLayout(new QGridLayout());
  }

  void GenericMethodWidget::doRefreshData()
  {

  }

  QWidget *GenericMethodWidget::doRequestWidget()
  {
    if(m_requestWidget == Q_NULLPTR){
        m_requestWidget = new QLabel("request");
      }
    return m_requestWidget;
  }

  QWidget *GenericMethodWidget::doResponseWidget()
  {
    if(m_responseWidget == Q_NULLPTR){
        m_responseWidget = new QTextEdit ("response");
      }
    return m_responseWidget;
  }

  void GenericMethodWidget::doSetResult(const QVariant &result)
  {
    //    auto lo = m_responseWidget->layout();

    //    if ( lo != NULL )
    //      {
    //        QLayoutItem* item;
    //        while ( ( item = lo->takeAt( 0 ) ) != NULL )
    //          {
    //            delete item->widget();
    //            delete item;
    //          }
    //        //delete lo;
    //      }

    //    if(result.canConvert<QVariantList>() == false)
    //      return;

    //    auto resultList = result.value<QVariantList>();
    //    foreach (auto res, resultList) {
    //        lo->addWidget(getGenericIndicatorForType(res), ++i,0);
    //      }

    //qDebug()<< "received result " <<result;
    QJsonDocument doc = QJsonDocument::fromVariant(result);

    m_responseWidget->setPlainText(doc.toJson());

  }

  QWidget *GenericMethodWidget::getGenericControlForType(JrpcDatatype type)
  {
    switch (type) {
      case JRPCT_VOID:{
          return new QLabel(QString("UNKNON TYPE: VOID"));
        }
      case JRPCT_INT8:{
          auto tmp = new QSpinBox();
          tmp->setRange(INT8_MIN, INT8_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_UINT8:{
          auto tmp = new QSpinBox();
          tmp->setRange(0, UINT8_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_INT16:{
          auto tmp = new QSpinBox();
          tmp->setRange(INT16_MIN, INT16_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_UINT16:{
          auto tmp = new QSpinBox();
          tmp->setRange(0, UINT16_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_INT32:{
          auto tmp = new QSpinBox();
          tmp->setRange(INT32_MIN, INT32_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_UINT32:{
          auto tmp = new QSpinBox();
          tmp->setRange(0, UINT32_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_INT64:{
          auto tmp = new QSpinBox();
          tmp->setRange(0, UINT32_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_UINT64:{
          auto tmp = new QSpinBox();
          tmp->setRange(0, UINT32_MAX);
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_REAL32:{
          auto tmp = new QDoubleSpinBox();
          tmp->setRange(0, qInf());
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_REAL64:{
          auto tmp = new QDoubleSpinBox();
          tmp->setRange(0, qInf());
          tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_STRING:{
          auto tmp = new QLineEdit();
          tmp->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_\\-\\+]+"), this ));
          //tmp->setRange(0, UINT64_MAX);
          //tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_WSTRING:{
          auto tmp = new QLineEdit();
          tmp->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_\\-\\+]+"), this ));
          //tmp->setRange(0, UINT64_MAX);
          //tmp->setButtonSymbols(QSpinBox::NoButtons);
          return tmp;
        }
      case JRPCT_REAL80:{
          return new QLabel(QString("UNKNON TYPE: JRPCT_REAL80"));
        }
      case JRPCT_BIT:{
          return new QLabel(QString("UNKNON TYPE: JRPCT_BIT"));
        }
      case JRPCT_BIGTYPE:{
          return new QLabel(QString("UNKNON TYPE: JRPCT_BIGTYPE"));
        }
      case JRPCT_MAXTYPES:{
          return new QLabel(QString("UNKNON TYPE: JRPCT_MAXTYPES"));
        }
      default:
        return new QLabel(QString("UNKNON TYPE: %1").arg(type));
      }

    new QLabel(QString("UNKNON TYPE: %1").arg(type));
  }

  QVariant GenericMethodWidget::doGetParams()
  {
    //auto lo = dynamic_cast<QGridLayout*>(m_responseWidget->layout());
    //if(lo == Q_NULLPTR )
    //  return QVariant();
    if(m_paramWidgets.isEmpty())
      return QVariant();

    QVariantList lst;
    foreach (auto wdg, m_paramWidgets) {
        lst.append(getVariantFromWidget(wdg));
      }

    return lst;

  }

  QVariant GenericMethodWidget::getVariantFromWidget(QWidget *wdg)
  {
    if(wdg == Q_NULLPTR)
      return QVariant();

    auto dblsb = dynamic_cast<QDoubleSpinBox*>(wdg);
    if(dblsb != Q_NULLPTR)
      return QVariant::fromValue(dblsb->value());

    auto sb = dynamic_cast<QSpinBox*>(wdg);
    if(sb != Q_NULLPTR)
      return QVariant::fromValue(sb->value());

    auto le = dynamic_cast<QLineEdit*>(wdg);
    if(le != Q_NULLPTR)
      return QVariant::fromValue(le->text());

    return QVariant();

  }

} // namespace talorion
