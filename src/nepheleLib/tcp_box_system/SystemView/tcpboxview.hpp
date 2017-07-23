#ifndef TCPBOXVIEW_HPP
#define TCPBOXVIEW_HPP

#include "core/AbstractSystemView.hpp"
#include <core/EntityManager.hpp>
#include <QGridLayout>
#include <QLabel>

namespace talorion {

class AnalogControl;
class DigitalControl;
class AnalogIndicator;
class DigitalIndicator;

class TcpBoxView : public AbstractSystemView
{
    Q_OBJECT
public:
    explicit TcpBoxView(EntityManager::EntityID en, QWidget *par = nullptr);
    virtual ~TcpBoxView() = default;

private:
    Q_DISABLE_COPY(TcpBoxView)

public:
    bool isValid()const;

    EntityManager::EntityID entity() const;

private:
    bool belongsToMe(EntityManager::EntityID en)const;

    void addView(EntityManager::EntityID entity);
    QLabel* addNameLabel(EntityManager::EntityID en);
    AnalogControl* addAnalogControl(EntityManager::EntityID en);
    DigitalControl* addDigitalControl(EntityManager::EntityID en);
    AnalogIndicator* addAnalogIndicator(EntityManager::EntityID en);
    DigitalIndicator* addDigitalIndicator(EntityManager::EntityID en);

    void setValue(EntityManager::EntityID en);
    void setAnalogControl(EntityManager::EntityID en);
    void setDigitalControl(EntityManager::EntityID en);
    void setAnalogIndicator(EntityManager::EntityID en);
    void setDigitalIndicator(EntityManager::EntityID en);

    void compareAnalogActWithSet(EntityManager::EntityID en);

    void valueChanged(EntityManager::EntityID en);
    void valueChangedAnalog(EntityManager::EntityID en);
    void valueChangedDigital(EntityManager::EntityID en);

    QString buildToolTipFromMetatata(EntityManager::EntityID en);

private slots:
    void addAIV(EntityManager::EntityID en);
    void addAOV(EntityManager::EntityID en);
    void addAV(EntityManager::EntityID en);
    void addDIV(EntityManager::EntityID en);
    void addDOV(EntityManager::EntityID en);
    void addDIOV(EntityManager::EntityID en);

    void connectionStateChanged(int en);

    void changeSetValue(EntityManager::EntityID en);
    void changeActValue(EntityManager::EntityID en);

    void setValueChangedByGui(EntityManager::EntityID en);
    void elementEntered(EntityManager::EntityID en);
    void elementLeft(EntityManager::EntityID en);

    void boxNameChanged(EntityManager::EntityID en);

    void metadataChanged(EntityManager::EntityID en);

private:
    const EntityManager::EntityID m_entity;
    QGridLayout* m_layout;
    QSignalMapper *m_setValueSignalMapper;
    QSignalMapper *m_enteredSignalMapper;
    QSignalMapper *m_leftSignalMapper;

    QMap<EntityManager::EntityID, AnalogControl*> m_analogControls;
    QMap<EntityManager::EntityID, DigitalControl*> m_digitalControls;
    QMap<EntityManager::EntityID, AnalogIndicator*> m_analogIndicators;
    QMap<EntityManager::EntityID, DigitalIndicator*> m_digitalIndicators;
};

} // namespace talorion

#endif // TCPBOXVIEW_HPP
