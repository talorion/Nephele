#include "DataAquisitionWorker.hpp"

#include <QEventLoop>
#include <QThread>
#include <QTofDaqDll.hpp>
#include "DaqDllEntity/DaqDllEntity.hpp"
#include "DaqDllEntity/DaqDllEntityFactory.hpp"
#include <DataAquisitionSystem/RegisteredDataEntity/registereddataentityfactory.hpp>
#include <DataAquisitionSystem/RegisteredDataEntity/registeredsourceentity.hpp>

//#define CALL_DELAY qrand() % ((10 + 1) - 6) + 6
#define CALL_DELAY 5
#define SIMULATE_DLL_CALL_DELAY QThread::currentThread()->msleep(CALL_DELAY)
#define EMIT_DELAY 1

namespace talorion {

  DataAquisitionWorker::DataAquisitionWorker(QObject *par)
    : QObject(par),
      m_daqDll(Q_NULLPTR),
      m_daqDllEntity(Q_NULLPTR),
      m_daqDllPollTimer(Q_NULLPTR),
      m_machine(Q_NULLPTR),
      m_idleState(Q_NULLPTR),
      m_daqErrorState(Q_NULLPTR),
      m_checkAquisitionServerState(Q_NULLPTR),
      m_checkMeasurementState(Q_NULLPTR),
      m_collectDataState(Q_NULLPTR),
      m_readValuesState(Q_NULLPTR),
      m_processQueueState(Q_NULLPTR),
      m_states(),
      m_state(State::notInitialized),
      m_previos_state(State::notInitialized)
  {

    qRegisterMetaType<DataAquisitionWorker::State>("DataAquisitionWorker::State");

    m_daqDll =  new QTofDaqDll();
    connect(m_daqDll, SIGNAL(initialized()), this, SIGNAL(dllInitialized()));

    connect(&DaqDllEntityFactory::instance(), SIGNAL(dataAquisitionDllChanged(EntityManager::EntityID,QString)),
            this, SLOT(dataAquisitionDllChanged(EntityManager::EntityID,QString)));
    connect(&DaqDllEntityFactory::instance(), SIGNAL(timoutChanged(EntityManager::EntityID,int)),
            this,SLOT(timoutChanged(EntityManager::EntityID,int)));
    connect(&DaqDllEntityFactory::instance(), SIGNAL(updaterateChanged(EntityManager::EntityID,int)),
            this, SLOT(updaterateChanged(EntityManager::EntityID,int)));
    connect(&DaqDllEntityFactory::instance(), SIGNAL(userDataPathChanged(EntityManager::EntityID,QString)),
            this, SLOT(userDataPathChanged(EntityManager::EntityID,QString)));

    m_daqDllPollTimer = new QTimer();
    m_daqDllPollTimer->setSingleShot(false);

    m_machine = new QStateMachine(QState::ExclusiveStates);

    ///===== Create States
    m_idleState = new QState();
    //m_idleState->assignProperty(this, "state", idle);
    m_daqErrorState = new QState();
    //m_daqErrorState->assignProperty(this, "state", daqError);
    m_checkAquisitionServerState = new QState();
    //m_checkAquisitionServerState->assignProperty(this, "state", checkAquisitionServerReady);
    m_checkMeasurementState = new QState();
    //m_checkMeasurementState->assignProperty(this, "state", checkMeasurementRunning);
    m_collectDataState = new QState();
    //m_collectDataState->assignProperty(this, "state", collectData);
    m_readValuesState = new QState();
    // m_readValuesState->assignProperty(this, "state", readValues);
    m_processQueueState = new QState();
    //m_processQueueState->assignProperty(this, "state", processQueue);

    ///connect corresponding signals
    connect(m_machine, &QStateMachine::stopped, [this] () {
        setState(notInitialized);
        emit notInitializedStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_idleState, &QState::entered, [this] () {
        setState(idle);
        emit idleStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_daqErrorState, &QState::entered, [this] () {
        setState(daqError);
        emit daqErrorStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_checkAquisitionServerState, &QState::entered, [this] () {
        setState(checkAquisitionServerReady);
        emit checkAquisitionServerStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_checkMeasurementState, &QState::entered, [this] () {
        setState(checkMeasurementRunning);
        emit checkMeasurementStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_collectDataState, &QState::entered, [this] () {
        setState(collectData);
        emit collectDataStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_readValuesState, &QState::entered, [this] () {
        setState(readValues);
        emit readValuesStateEntered(getPreviosState(),QPrivateSignal());
      } );
    connect(m_processQueueState, &QState::entered, [this] () {
        setState(processQueue);
        emit processQueueStateEntered(getPreviosState(),QPrivateSignal());
      } );

    ///Doing Useful Work on State Entry
    connect(m_idleState,SIGNAL(entered()),this,SLOT(idleStateEntered()));
    connect(m_daqErrorState,SIGNAL(entered()),this,SLOT(daqErrorStateEntered()));
    connect(m_checkAquisitionServerState,SIGNAL(entered()),this,SLOT(checkAquisitionServerStateEntered()));
    connect(m_checkMeasurementState,SIGNAL(entered()),this,SLOT(checkMeasurementStateEntered()));
    connect(m_collectDataState,SIGNAL(entered()),this,SLOT(collectDataStateEntered()));
    connect(m_readValuesState,SIGNAL(entered()),this,SLOT(readValuesStateEntered()));
    connect(m_processQueueState,SIGNAL(entered()),this,SLOT(processQueueStateEntered()));

    ///===== Add State Transitions
    m_idleState->addTransition(m_daqDllPollTimer, SIGNAL(timeout()), m_checkAquisitionServerState);
    m_idleState->addTransition(this, SIGNAL(daqErrorOccured()), m_daqErrorState);
    m_checkAquisitionServerState->addTransition(this, SIGNAL(aquisitionServerReady()), m_checkMeasurementState);
    m_checkAquisitionServerState->addTransition(this, SIGNAL(daqErrorOccured()), m_daqErrorState);
    m_checkMeasurementState->addTransition(this, SIGNAL(measurementRunning()), m_collectDataState);
    m_checkMeasurementState->addTransition(this, SIGNAL(measurementNotRunning()), m_readValuesState);
    m_checkMeasurementState->addTransition(this, SIGNAL(daqErrorOccured()), m_daqErrorState);
    m_collectDataState->addTransition(this, SIGNAL(collectDataFinished()), m_idleState);
    m_collectDataState->addTransition(this, SIGNAL(daqErrorOccured()), m_daqErrorState);
    m_readValuesState->addTransition(this, SIGNAL(registerUserDataFinished()), m_processQueueState);
    m_readValuesState->addTransition(this, SIGNAL(daqErrorOccured()), m_daqErrorState);
    m_processQueueState->addTransition(this, SIGNAL(queueEmpty()), m_idleState);
    m_processQueueState->addTransition(this, SIGNAL(daqErrorOccured()), m_daqErrorState);
    m_daqErrorState->addTransition(this, SIGNAL(daqErrorCleared()), m_idleState);

    ///===== Add States to MAchine
    m_machine->addState(m_idleState);
    m_machine->addState(m_daqErrorState);
    m_machine->addState(m_checkAquisitionServerState);
    m_machine->addState(m_checkMeasurementState);
    m_machine->addState(m_collectDataState);
    m_machine->addState(m_readValuesState);
    m_machine->addState(m_processQueueState);
    m_machine->setInitialState(m_idleState);
    m_machine->setErrorState(m_daqErrorState);

    m_states.clear();
    m_states.append(m_idleState);
    m_states.append(m_daqErrorState);
    m_states.append(m_checkAquisitionServerState);
    m_states.append(m_checkMeasurementState);
    m_states.append(m_collectDataState);
    m_states.append(m_readValuesState);
    m_states.append(m_processQueueState);

    //connect(m_machine, SIGNAL(started()), this , SIGNAL(started()));

    connect(m_daqDll, SIGNAL(initialized()), m_machine,SLOT(start()));
    connect(m_daqDll, SIGNAL(disposed()), m_machine,SLOT(stop()));

  }

  DataAquisitionWorker::~DataAquisitionWorker()
  {
    disconnectToDaqDllEntity();

    Q_ASSERT(m_daqDllPollTimer != Q_NULLPTR);
    m_daqDllPollTimer->stop();

    Q_ASSERT(m_machine != Q_NULLPTR);
    m_machine->stop();

    delete m_daqDll;
    delete m_daqDllEntity;

    delete m_daqDllPollTimer;

    delete m_machine;
    m_states.clear();
    //delete m_idleState;
    //delete m_daqErrorState;
    //delete m_checkAquisitionServerState;
    //delete m_checkMeasurementState;
    //delete m_collectDataState;
    //delete m_readValuesState;
    //delete m_processQueueState;

    m_state = State::notInitialized;
    m_previos_state = State::notInitialized;
  }

  DataAquisitionWorker::State DataAquisitionWorker::getPreviosState() const
  {
    return m_previos_state;
  }

  bool DataAquisitionWorker::isRunning() const
  {
    Q_ASSERT(m_machine != Q_NULLPTR);
    return m_machine->isRunning();
  }

  int DataAquisitionWorker::pollInterval() const
  {
    Q_ASSERT(m_daqDllPollTimer!=Q_NULLPTR);
    return m_daqDllPollTimer->interval();
  }

  int DataAquisitionWorker::daqTimeout() const
  {
    Q_ASSERT(m_daqDllEntity!=Q_NULLPTR);
    return m_daqDllEntity->timeout();
  }

  int DataAquisitionWorker::numberOfStates() const
  {
    return m_states.size();
  }

  void DataAquisitionWorker::setDllPath(const QString &new_val)
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return;
    m_daqDllEntity->setDllPath(new_val);
  }

  void DataAquisitionWorker::connectToDaqDllEntity(EntityManager::EntityID dllid)
  {
    if(EntityManager::isValid(dllid) != true)
      return;

    if(disconnectToDaqDllEntity(dllid) != 0)
      return;

    Q_ASSERT(m_daqDllEntity == Q_NULLPTR);

    ///===== connect to DaqDllEntity
    m_daqDllEntity = new DaqDllEntity(dllid);

    auto dllPath = m_daqDllEntity->dllPath();
    auto timeout = m_daqDllEntity->timeout();
    dataAquisitionDllChanged(m_daqDllEntity->daqDllId(), dllPath);
    timoutChanged(m_daqDllEntity->daqDllId(), timeout);

    setPollInterval(m_daqDllEntity->updaterate());

    //    QEventLoop tmp_evt_loop;
    //    QTimer::singleShot(timeout, &tmp_evt_loop, SLOT(quit()));
    //    connect(this,SIGNAL(dllInitialized()),&tmp_evt_loop,SLOT(quit()));
    //    tmp_evt_loop.exec();
  }

  int DataAquisitionWorker::disconnectToDaqDllEntity()
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return 0;
    auto dllid = m_daqDllEntity->daqDllId();
    return disconnectToDaqDllEntity(dllid);
  }

  int DataAquisitionWorker::disconnectToDaqDllEntity(EntityManager::EntityID dllid)
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return 0 ;

    if(m_daqDllEntity->daqDllId() != dllid){
        qWarning()<<Q_FUNC_INFO<<"unmatching Daq Dll Entity detected";
        return -1;
      }

    Q_ASSERT(m_daqDll != Q_NULLPTR);
    m_daqDll->dispose();
    delete m_daqDllEntity;
    m_daqDllEntity= Q_NULLPTR;
    return 0 ;
  }

  void DataAquisitionWorker::dataAquisitionDllChanged(EntityManager::EntityID dllid, const QString &newval)
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return;

    if(m_daqDllEntity->daqDllId() != dllid){
        qWarning()<<Q_FUNC_INFO<<"unmatching Daq Dll Entity detected";
        return;
      }

    if(m_daqDllEntity->isValid() == false)
      return;

    auto timeout = m_daqDllEntity->timeout();
    m_daqDll->dispose();
    m_daqDll->init(newval, timeout);
  }

  void DataAquisitionWorker::timoutChanged(EntityManager::EntityID dllid, int newval)
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return;

    if(m_daqDllEntity->daqDllId() != dllid){
        qWarning()<<Q_FUNC_INFO<<"unmatching Daq Dll Entity detected";
      }
    m_daqDll->setTimeout(newval);
  }

  void DataAquisitionWorker::updaterateChanged(EntityManager::EntityID dllid, int newval)
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return;

    if(m_daqDllEntity->daqDllId() != dllid){
        qWarning()<<Q_FUNC_INFO<<"unmatching Daq Dll Entity detected";
      }
    setPollInterval(newval);
  }

  void DataAquisitionWorker::userDataPathChanged(EntityManager::EntityID dllid, const QString &newval)
  {
    if(m_daqDllEntity == Q_NULLPTR)
      return;

    if(m_daqDllEntity->daqDllId() != dllid){
        qWarning()<<Q_FUNC_INFO<<"unmatching Daq Dll Entity detected";
      }
    Q_UNUSED(newval);
  }

  void DataAquisitionWorker::setPollInterval(int ms)
  {
    Q_ASSERT( m_daqDllPollTimer != Q_NULLPTR);
    m_daqDllPollTimer->stop();
    int interval = qBound(MIN_POLL_INTERVAL, ms, MAX_POLL_INTERVAL);
    m_daqDllPollTimer->setInterval(interval);
    m_daqDllPollTimer->start();
  }

  void DataAquisitionWorker::idleStateEntered()
  {
    setState(idle);
    //QThread::currentThread()->msleep(200);
  }

  void DataAquisitionWorker::daqErrorStateEntered()
  {
    setState(daqError);

    //emit daqErrorCleared();
    QTimer::singleShot(EMIT_DELAY, this, SIGNAL(daqErrorCleared()));
  }

  void DataAquisitionWorker::checkAquisitionServerStateEntered()
  {
    // qDebug()<<Q_FUNC_INFO;
    setState(checkAquisitionServerReady);

    if(m_daqDll->tofDaqRunning() == true){
        emit aquisitionServerReady(QPrivateSignal());
      }else{
        emit daqErrorOccured(QPrivateSignal());
      }
  }

  void DataAquisitionWorker::checkMeasurementStateEntered()
  {
    //qDebug()<<Q_FUNC_INFO;
    setState(checkMeasurementRunning);

    auto tmp_daqActive =m_daqDll->daqActive();
    if(tmp_daqActive == false){
        emit measurementNotRunning(QPrivateSignal());
      }else{
        emit measurementRunning(QPrivateSignal());
      }
  }

  void DataAquisitionWorker::collectDataStateEntered()
  {
    setState(collectData);
    updateUserData();
    emit collectDataFinished(QPrivateSignal());

    //emit daqErrorOccured();
  }

  void DataAquisitionWorker::readValuesStateEntered()
  {
    setState(readValues);
    registerUserData();
    emit registerUserDataFinished(QPrivateSignal());

    //emit daqErrorOccured(QPrivateSignal());
  }

  void DataAquisitionWorker::processQueueStateEntered()
  {
    setState(processQueue);
    SIMULATE_DLL_CALL_DELAY;

    //emit daqErrorOccured();
    //emit queueEmpty();
    QTimer::singleShot(EMIT_DELAY, this, SIGNAL(queueEmpty()));
  }

  void DataAquisitionWorker::updateUserData()
  {
    auto sourcesToRegister = DaqDllEntityFactory::getUserDataComponents(m_daqDllEntity->daqDllId());
    UserDataSourceList registeredSources;
    m_daqDll->readRegUserData(registeredSources);
//    foreach (auto src, registeredSources) {
//        if(src.location().contains(m_daqDllEntity->userDataPath()))
//          continue;

//        auto ent = RegisteredDataEntityFactory::createNewRegisteredSourceEntity(src);
//        RegisteredSourceEntity srcEnt(ent);
//        if(srcEnt.isValid() == false)
//          continue;

//        srcEnt.updateChildren(src);
//      }

    int index = 0;
    UserDataSourceList sourcesToUpdate;
    foreach (auto source, sourcesToRegister) {
        index = registeredSources.indexOf(source);
        if(index >= 0){
            auto upSource = registeredSources.at(index);
            upSource = upSource.updateData(source);
            sourcesToUpdate.append(upSource);
          }
      }

    int daqret = 4;
    foreach (auto upSource, sourcesToUpdate) {
        daqret = m_daqDll->updateUserData(upSource);
        if(daqret != 4)
          qDebug() << "Error updateing user data" << upSource;
      }
  }

  void DataAquisitionWorker::registerUserData()
  {
    int daqret = 4;
    auto sourcesToRegister = DaqDllEntityFactory::getUserDataComponents(m_daqDllEntity->daqDllId());
    UserDataSourceList registeredSources;
    m_daqDll->readRegUserData(registeredSources);
//    foreach (auto src, registeredSources) {
//        if(src.location().contains(m_daqDllEntity->userDataPath()))
//          continue;

//        auto ent = RegisteredDataEntityFactory::createNewRegisteredSourceEntity(src);
//        RegisteredSourceEntity srcEnt(ent);
//        if(srcEnt.isValid() == false)
//          continue;

//        srcEnt.updateChildren(src);
//      }

    int index = 0;
    UserDataSourceList sourcesToUpdate;
    foreach (auto source, sourcesToRegister) {
        index = registeredSources.indexOf(source);
        if(index >= 0){
            auto upSource = registeredSources.at(index);
            upSource = upSource.merge(source);
            sourcesToUpdate.append(upSource);
          }else{
            daqret = m_daqDll->registerUserDataBuf(source);
            if(daqret != 4)
              qDebug() << "Error registering user data" << source;
            else
              sourcesToUpdate.append(source);
          }
      }


    foreach (auto upSource, sourcesToUpdate) {
        daqret = m_daqDll->updateUserData(upSource);
        if(daqret != 4)
          qDebug() << "Error updateing user data" << upSource;
      }

  }

  DataAquisitionWorker::State DataAquisitionWorker::getState() const
  {
    return m_state;
  }

  void DataAquisitionWorker::setState(State state)
  {
    if(getState() != state){
        m_previos_state = getState();
        m_state = state;
        emit stateChanged( m_state);
      }
  }


} // namespace talorion
