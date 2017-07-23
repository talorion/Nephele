#ifndef REGISTEREDDATAVIEW_HPP
#define REGISTEREDDATAVIEW_HPP

#include "core/AbstractSystemView.hpp"
#include <core/EntityManager.hpp>
#include <QGridLayout>

#include <gui_system/tcp_box_view/analog_view/flowcontrollerview.hpp>
#include <gui_system/tcp_box_view/digital_view/digital_view.hpp>

namespace talorion {

  class RegisteredDataView : public AbstractSystemView
  {
    Q_OBJECT
  public:
    explicit RegisteredDataView(EntityManager::EntityID entity, QWidget *par = nullptr);
    virtual ~RegisteredDataView() = default;

  private:
     Q_DISABLE_COPY(RegisteredDataView)

  public:
    bool isValid()const;

    EntityManager::EntityID entity() const;

  private:
    bool belongsToMe(EntityManager::EntityID en)const;
    void addAnalogView(EntityManager::EntityID  entity, analogView::mode md);
    void addDigitalView(EntityManager::EntityID  entity, digital_view::mode md);

  private slots:
      void addAIV(EntityManager::EntityID  entity);
      void addAOV(EntityManager::EntityID  entity);
      void addAV(EntityManager::EntityID  entity);
      void addDIV(EntityManager::EntityID  entity);
      void addDOV(EntityManager::EntityID  entity);
      void addDIOV(EntityManager::EntityID  entity);

      void removeAIV(EntityManager::EntityID  entity);
      void removeAOV(EntityManager::EntityID  entity);
      void removeAV(EntityManager::EntityID  entity);
      void removeDIV(EntityManager::EntityID  entity);
      void removeDOV(EntityManager::EntityID  entity);
      void removeDIOV(EntityManager::EntityID  entity);

  private:
    const EntityManager::EntityID m_entity;

    QGridLayout* m_layout;
    QMap<int, analogView* > ana_views;
    QMap<int, digital_view* > dig_views;

  };

} // namespace talorion

#endif // REGISTEREDDATAVIEW_HPP
