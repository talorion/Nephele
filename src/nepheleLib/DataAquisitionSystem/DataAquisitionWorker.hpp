#ifndef DATAAQUISITIONWORKER_HPP
#define DATAAQUISITIONWORKER_HPP

#include <QObject>
#include <QStateMachine>
#include <QTimer>

#include "core/EntityManager.hpp"

#define MIN_POLL_INTERVAL 10
#define MAX_POLL_INTERVAL 1000

namespace talorion {

class QTofDaqDll;
class DaqDllEntity;

class DataAquisitionWorker : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(DataAquisitionWorker::State state READ getState WRITE setState MEMBER m_state NOTIFY stateChanged)
public:
    enum State { notInitialized, idle, daqError, checkAquisitionServerReady, checkMeasurementRunning, collectData, readValues, processQueue };
    Q_ENUM(State)

public:
    explicit DataAquisitionWorker(QObject *par = 0);
    virtual ~DataAquisitionWorker();

private:
    Q_DISABLE_COPY(DataAquisitionWorker)

public:

    State getPreviosState() const;
    State getState() const;
    void setState(State getState);

    bool isRunning()const;
    int pollInterval()const;
    int daqTimeout()const;
    int numberOfStates()const;

    void setDllPath(const QString &new_val);
    void setPollInterval(int ms);

    void connectToDaqDllEntity(EntityManager::EntityID dllid);
    int disconnectToDaqDllEntity();
    int disconnectToDaqDllEntity(EntityManager::EntityID dllid);

signals:
    void stateChanged(DataAquisitionWorker::State new_state);

    void notInitializedStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void idleStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void daqErrorStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void checkAquisitionServerStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void checkMeasurementStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void collectDataStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void readValuesStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);
    void processQueueStateEntered(DataAquisitionWorker::State prev_state, QPrivateSignal);

signals:
    void dllInitialized();
    void measurementStarted();

    void aquisitionServerReady(QPrivateSignal );
    void measurementRunning(QPrivateSignal );
    void collectDataFinished(QPrivateSignal );
    void measurementNotRunning(QPrivateSignal );
    void registerUserDataFinished(QPrivateSignal );
    void daqErrorOccured(QPrivateSignal );
    void daqErrorCleared(QPrivateSignal );
    void queueEmpty(QPrivateSignal );

private slots:
    void dataAquisitionDllChanged(EntityManager::EntityID dllid, const QString &newval);
    void timoutChanged(EntityManager::EntityID dllid, int newval);
    void updaterateChanged(EntityManager::EntityID dllid, int newval);
    void userDataPathChanged(EntityManager::EntityID dllid, const QString &newval);

    void idleStateEntered();
    void daqErrorStateEntered();
    void checkAquisitionServerStateEntered();
    void checkMeasurementStateEntered();
    void collectDataStateEntered();
    void readValuesStateEntered();
    void processQueueStateEntered();

private:
//    void checkAquisitionServerReady();
//    void readSharedMemoryDescriotor();
//    void checkMeasurementActive();
    void updateUserData();
//    void collectNewData();
//    void readValues();
    void registerUserData();
//    void processQueue();

private:
    QTofDaqDll* m_daqDll;
    DaqDllEntity* m_daqDllEntity;
    //EntityManager::EntityID m_daqDllEntityId;

    QTimer* m_daqDllPollTimer;

    QStateMachine* m_machine;

    QState *m_idleState;
    QState *m_daqErrorState;
    QState *m_checkAquisitionServerState;
    QState *m_checkMeasurementState;
    QState *m_collectDataState;
    QState *m_readValuesState;
    QState *m_processQueueState;

    QList<QAbstractState *> m_states;

    State m_state;
    State m_previos_state;

    //bool m_daqActive;

};

} // namespace talorion

#endif // DATAAQUISITIONWORKER_HPP
