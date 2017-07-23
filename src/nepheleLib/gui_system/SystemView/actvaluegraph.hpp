#ifndef ACTVALUEGRAPH_HPP
#define ACTVALUEGRAPH_HPP

#include <core/EntityManager.hpp>
#include <core/AbstractSystemView.hpp>

#include <QGridLayout>

#include <gui_system/CustomWidgets/AnalogPlot.hpp>

namespace talorion {

  class ActValueGraph : public AbstractSystemView
  {
    Q_OBJECT
  public:
    explicit ActValueGraph(QWidget *par = nullptr);
    virtual ~ActValueGraph() = default;

    EntityManager::EntityID entity() const;

  private:
    Q_DISABLE_COPY(ActValueGraph)

  private:
    bool belongsToMe(EntityManager::EntityID en)const;
    bool isPlotableEntity(EntityManager::EntityID en)const;
    void resetPlot(EntityManager::EntityID en);
    void updatePlot(EntityManager::EntityID en);
    void updateHistory(EntityManager::EntityID en);

    double getMinValue(EntityManager::EntityID en);
    double getMaxValue(EntityManager::EntityID en);
    QCPGraphData getValueToPlot(EntityManager::EntityID en);

    graphHistory_t getHistory(EntityManager::EntityID en);

  private slots:
    void changeSetValue(EntityManager::EntityID en);
    void changeActValue(EntityManager::EntityID en);
    void setEntity(const EntityManager::EntityID &en);

    //void graphTimerTimeout();

  private:
    QGridLayout* m_layout;
    EntityManager::EntityID m_entity;
    AnalogPlot* m_analogPlot;
    QMap<EntityManager::EntityID, graphHistory_t> m_histories;

  };

} // namespace talorion

#endif // ACTVALUEGRAPH_HPP
