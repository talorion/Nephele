#include <QtTest>
#include <QCoreApplication>

#include <tcp_box_system/tcp_box_system.hpp>

#include <tcp_box_system/tbs_config_widget/tbs_config_widget.hpp>

#include <QSize>

#include <core/EventManager.hpp>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>

//name
#define VAL_NAME "testvalue"
//unit
#define VAL_UNIT "%"
//act_min
#define A_MIN 0
//act_max
#define A_MAX 100
//set_min
#define S_MIN 0
//set_max
#define S_MAX 100
//setVal
//id
//boxid


using namespace talorion;

class tst_TcpBoxEntity : public QObject
{
  Q_OBJECT

public:
  tst_TcpBoxEntity();
  ~tst_TcpBoxEntity();

private slots:
  void init(); //will be called before each testfunction is executed.
  void cleanup(); //will be called before each testfunction is executed.

  void noBoxesExistAfterStartup();
  void tcpBoxEntityIsCreatedByAFactory();
  void tcpBoxEntityIsNotCreatedForSameHostAndPortComponent();
  void tcpBoxCreationEmitsNewTcpBoxEvent();
  void tcpBoxEntityHasName();
  void tcpBoxEntityHasBoxId();
  void tcpBoxEntityHasIpAddress();
  void tcpBoxEntityHasPort();
  void tcpBoxEntityHasConnectionState();
  void tcpBoxEntityHasBackend();
  void tcpBoxEntityIsDeletedByAFactory();
  void removeTcpBoxEmitsTcpBoxRemovedEvent();

  void noAnalogInputValuesExistAfterStartup();
  void AnalogInputValueIsCreatedByAFactory();
  void AnalogInputValueEmitsNewAnalogInputValueEvent();
  void AnalogInputValueHasName();
  void AnalogInputValueHasUnit();
  void AnalogInputValueHasActMin();
  void AnalogInputValueHasActMax();
  void AnalogInputValueHasAnalogActValue();
  void AnalogInputValueHasId();
  void AnalogInputValueHasBoxId();
  void AnalogInputValueHasUserData();
  void removeAnalogInputValueEmitsRemovedAnalogInputValueEvent();

  void noAnalogOutputValuesExistAfterStartup();
  void AnalogOutputValueIsCreatedByAFactory();
  void AnalogOutputtValueEmitsNewAnalogOutputValueEvent();
  void AnalogOutputValueHasName();
  void AnalogOutputValueHasUnit();
  void AnalogOutputValueHasSetMin();
  void AnalogOutputValueHasSetMax();
  void AnalogOutputValueHasAnalogSetValue();
  void AnalogOutputValueHasId();
  void AnalogOutputValueHasBoxId();
  void AnalogOutputValueHasUserData();
  void removeAnalogOutputValueEmitsRemovedAnalogOutputValueEvent();

  void noAnalogValueExistAfterStartup();
  void AnalogValueIsCreatedByAFactory();
  void AnalogValueEmitsNewAnalogValueEvent();
  void AnalogValueHasName();
  void AnalogValueHasUnit();
  void AnalogValueHasSetMin();
  void AnalogValueHasSetMax();
  void AnaloglueHasActMin();
  void AnalogValueHasActMax();
  void AnalogValueHasAnalogSetValue();
  void AnalogValueHasAnalogActValue();
  void AnalogValueHasId();
  void AnalogValueHasBoxId();
  void AnalogValueHasUserData();
  void removeAnalogValueEmitsRemovedAnalogValueEvent();

  void noDigitalInputValuesExistAfterStartup();
  void DigitalInputValueIsCreatedByAFactory();
  void DigitalInputValueEmitsNewDigitalInputValueEvent();
  void DigitalInputValueHasName();
  void DigitalInputValueHasUnit();
  void DigitalInputValueHasDigitalActValue();
  void DigitalInputValueHasId();
  void DigitalInputValueHasBoxId();
  void DigitalInputValueHasUserData();
  void removeDigitalInputValueEmitsRemovedDigitalInputValueEvent();

  void noDigitalOutputValuesExistAfterStartup();
  void DigitalOutputValueIsCreatedByAFactory();
  void DigitalOutputValueEmitsNewDigitalOutputValueEvent();
  void DigitalOutputValueHasName();
  void DigitalOutputValueHasDigitalSetValue();
  void DigitalOutputValueHasId();
  void DigitalOutputValueHasBoxId();
  void DigitalOutputValueHasUserData();
  void removeDigitalOutputValueEmitsRemovedDigitalOutputValueEvent();

  void noDigitalValueExistAfterStartup();
  void DigitalValueIsCreatedByAFactory();
  void DigitalValueEmitsNewDigitalValueEvent();
  void DigitalValueHasName();
  void DigitalValueHasDigitalSetValue();
  void DigitalValueHasDigitalActValue();
  void DigitalValueHasId();
  void DigitalValueHasBoxId();
  void DigitalValueHasUserData();
  void removeDigitalValueEmitsRemovedDigitalValueEvent();

  void AnalogInputValuesCanHaveTcpBoxAsParent();
  void AnalogOutputValuesCanHaveTcpBoxAsParent();
  void AnalogValuesCanHaveTcpBoxAsParent();
  void DigitalInputValuesCanHaveTcpBoxAsParent();
  void DigitalOutputValuesCanHaveTcpBoxAsParent();
  void DigitalValuesCanHaveTcpBoxAsParent();

  void removeTcpBoxRemovesAllItsValues();

  void configuredBoxNeedsNameHostPortBackend();

};

tst_TcpBoxEntity::tst_TcpBoxEntity(){

}

tst_TcpBoxEntity::~tst_TcpBoxEntity(){

}

void tst_TcpBoxEntity::init()
{
    auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    QVERIFY(all_boxes.isEmpty());
}

void tst_TcpBoxEntity::cleanup()
{
    auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    foreach (auto box, all_boxes) {
        EntityManager::get_instance()->removeEntity(box);
    }
    all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    QVERIFY(all_boxes.isEmpty());
}

void tst_TcpBoxEntity::noBoxesExistAfterStartup(){
  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  QVERIFY(all_boxes.isEmpty());
}

void tst_TcpBoxEntity::tcpBoxEntityIsCreatedByAFactory(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  QVERIFY(all_boxes.contains(newbox));
}

void tst_TcpBoxEntity::tcpBoxEntityIsNotCreatedForSameHostAndPortComponent()
{
    QString serverName ="192.168.0.6";
    quint16 serverPort = 2701;

    EntityManager::get_instance()->createNewTcpBox("Firstbox", serverName, serverPort);
    auto oldsize = EntityManager::get_instance()->get_all_tcpBoxes().size();
    QVERIFY(oldsize>0);
    EntityManager::get_instance()->createNewTcpBox("Secondbox", serverName, serverPort);
    auto newsize = EntityManager::get_instance()->get_all_tcpBoxes().size();
    QCOMPARE(oldsize, newsize);
}

void tst_TcpBoxEntity::tcpBoxCreationEmitsNewTcpBoxEvent(){
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newTcpBox(int)));
  EntityManager::get_instance()->createNewTcpBox();

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::tcpBoxEntityHasName(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::tcpBoxEntityHasBoxId(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::tcpBoxEntityHasIpAddress()
{
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, IP_ADDRESS_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::tcpBoxEntityHasPort(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, PORT_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::tcpBoxEntityHasConnectionState(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, CONNECTION_STATE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::tcpBoxEntityHasBackend(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, TCP_BOX_BACKEND_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueIsCreatedByAFactory(){
  auto newval = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto all_analog_values = EntityManager::get_instance()->get_all_Values();
  QVERIFY(all_analog_values.contains(newval));
}

void tst_TcpBoxEntity::AnalogInputValueEmitsNewAnalogInputValueEvent(){
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newAnalogInputValue(int)));
  EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::AnalogInputValueHasName()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasUnit()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, UNITS_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasActMin()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, ACT_MIN_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasActMax()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, ACT_MAX_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasAnalogActValue()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, ANALOG_ACT_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasId()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasBoxId()
{
  auto newbox = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newbox, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogInputValueHasUserData()
{
  auto newval = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, USER_DATA_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::removeAnalogInputValueEmitsRemovedAnalogInputValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  auto newval = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0);

  QSignalSpy spy(evt_mng, SIGNAL(removedAnalogInputValue(int)));
  EntityManager::get_instance()->removeEntity(newval);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_TcpBoxEntity::noAnalogOutputValuesExistAfterStartup()
{
  auto all_aos = EntityManager::get_instance()->get_all_AnalogOutputValues();
  QVERIFY(all_aos.isEmpty());
}

void tst_TcpBoxEntity::AnalogOutputValueIsCreatedByAFactory(){
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto all_analog_values = EntityManager::get_instance()->get_all_Values();
  QVERIFY(all_analog_values.contains(newval));
}

void tst_TcpBoxEntity::AnalogOutputtValueEmitsNewAnalogOutputValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newAnalogOutputValue(int)));
  EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::AnalogOutputValueHasName(){
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasUnit()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, UNITS_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasSetMin()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, SET_MIN_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasSetMax()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, SET_MAX_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasAnalogSetValue()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ANALOG_SET_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasId()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasBoxId()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogOutputValueHasUserData()
{
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, USER_DATA_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::removeAnalogOutputValueEmitsRemovedAnalogOutputValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0);

  QSignalSpy spy(evt_mng, SIGNAL(removedAnalogOutputValue(int)));
  EntityManager::get_instance()->removeEntity(newval);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_TcpBoxEntity::noAnalogValueExistAfterStartup()
{
  auto all_avs = EntityManager::get_instance()->get_all_AnalogValues();
  QVERIFY(all_avs.isEmpty());
}

void tst_TcpBoxEntity::AnalogValueIsCreatedByAFactory(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto all_analog_values = EntityManager::get_instance()->get_all_Values();
  QVERIFY(all_analog_values.contains(newval));
}

void tst_TcpBoxEntity::AnalogValueEmitsNewAnalogValueEvent(){
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newAnalogValue(int)));
  EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::AnalogValueHasName(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasUnit(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, UNITS_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasSetMin(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, SET_MIN_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasSetMax(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, SET_MAX_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnaloglueHasActMin(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ACT_MIN_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasActMax(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ACT_MAX_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasAnalogSetValue()
{
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ANALOG_SET_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasAnalogActValue(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ANALOG_ACT_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasId(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasBoxId(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::AnalogValueHasUserData(){
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, USER_DATA_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::removeAnalogValueEmitsRemovedAnalogValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN,S_MAX,A_MIN,A_MAX, 0, 0);

  QSignalSpy spy(evt_mng, SIGNAL(removedAnalogValue(int)));
  EntityManager::get_instance()->removeEntity(newval);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_TcpBoxEntity::noDigitalInputValuesExistAfterStartup()
{
  auto all_dis = EntityManager::get_instance()->get_all_DigitalInputValues();
  QVERIFY(all_dis.isEmpty());
}

void tst_TcpBoxEntity::DigitalInputValueIsCreatedByAFactory(){
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto all_analog_values = EntityManager::get_instance()->get_all_DValues();
  QVERIFY(all_analog_values.contains(newval));
}

void tst_TcpBoxEntity::DigitalInputValueEmitsNewDigitalInputValueEvent(){
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newDigitalInputValue(int)));
  EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::DigitalInputValueHasName()
{
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalInputValueHasUnit()
{
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, UNITS_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalInputValueHasDigitalActValue()
{
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, DIGITAL_ACT_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalInputValueHasId()
{
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalInputValueHasBoxId()
{
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalInputValueHasUserData()
{
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, USER_DATA_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::removeDigitalInputValueEmitsRemovedDigitalInputValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0);

  QSignalSpy spy(evt_mng, SIGNAL(removedDigitalInputValue(int)));
  EntityManager::get_instance()->removeEntity(newval);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_TcpBoxEntity::noDigitalOutputValuesExistAfterStartup()
{
  auto all_aos = EntityManager::get_instance()->get_all_DigitalOutputValues();
  QVERIFY(all_aos.isEmpty());
}

void tst_TcpBoxEntity::DigitalOutputValueIsCreatedByAFactory()
{
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  auto all_analog_values = EntityManager::get_instance()->get_all_DValues();
  QVERIFY(all_analog_values.contains(newval));
}

void tst_TcpBoxEntity::DigitalOutputValueEmitsNewDigitalOutputValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newDigitalOutputValue(int)));
  EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::DigitalOutputValueHasName()
{
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalOutputValueHasDigitalSetValue()
{
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, DIGITAL_SET_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalOutputValueHasId()
{
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalOutputValueHasBoxId(){
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalOutputValueHasUserData()
{
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, USER_DATA_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::removeDigitalOutputValueEmitsRemovedDigitalOutputValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0);

  QSignalSpy spy(evt_mng, SIGNAL(removedDigitalOutputValue(int)));
  EntityManager::get_instance()->removeEntity(newval);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_TcpBoxEntity::noDigitalValueExistAfterStartup(){
  auto all_dvs = EntityManager::get_instance()->get_all_DigitalValues();
  QVERIFY(all_dvs.isEmpty());
}

void tst_TcpBoxEntity::DigitalValueIsCreatedByAFactory()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto all_analog_values = EntityManager::get_instance()->get_all_DValues();
  QVERIFY(all_analog_values.contains(newval));
}

void tst_TcpBoxEntity::DigitalValueEmitsNewDigitalValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  QSignalSpy spy(evt_mng, SIGNAL(newDigitalValue(int)));
  EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::DigitalValueHasName()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, NAME_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalValueHasDigitalSetValue()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, DIGITAL_SET_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalValueHasDigitalActValue()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, DIGITAL_ACT_VALUE_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalValueHasId()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalValueHasBoxId()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, BOX_ID_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::DigitalValueHasUserData()
{
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);
  auto has_comp = EntityManager::get_instance()->entityHasComponent(newval, USER_DATA_COMPONENT);
  QCOMPARE(has_comp, true);
}

void tst_TcpBoxEntity::removeDigitalValueEmitsRemovedDigitalValueEvent()
{
  auto evt_mng = EventManager::get_instance();
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false , 0);

  QSignalSpy spy(evt_mng, SIGNAL(removedDigitalValue(int)));
  EntityManager::get_instance()->removeEntity(newval);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_TcpBoxEntity::AnalogInputValuesCanHaveTcpBoxAsParent()
{
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewAnalogInputValue(VAL_NAME, VAL_UNIT, A_MIN, A_MAX, 0, newbox);
  auto valPar = EntityManager::get_instance()->getParent(newval);
  QCOMPARE(valPar, newbox);
}

void tst_TcpBoxEntity::AnalogOutputValuesCanHaveTcpBoxAsParent()
{
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewAnalogOutputValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, 0, 0, newbox);
  auto valPar = EntityManager::get_instance()->getParent(newval);
  QCOMPARE(valPar, newbox);
}

void tst_TcpBoxEntity::AnalogValuesCanHaveTcpBoxAsParent()
{
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewAnalogValue(VAL_NAME, VAL_UNIT, S_MIN, S_MAX, A_MIN, A_MAX, 0 , 0 , newbox);
  auto valPar = EntityManager::get_instance()->getParent(newval);
  QCOMPARE(valPar, newbox);
}

void tst_TcpBoxEntity::DigitalInputValuesCanHaveTcpBoxAsParent()
{
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewDigitalInputValue(VAL_NAME, VAL_UNIT, 0, newbox);
  auto valPar = EntityManager::get_instance()->getParent(newval);
  QCOMPARE(valPar, newbox);
}

void tst_TcpBoxEntity::DigitalOutputValuesCanHaveTcpBoxAsParent()
{
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewDigitalOutputValue(VAL_NAME, false, 0, newbox);
  auto valPar = EntityManager::get_instance()->getParent(newval);
  QCOMPARE(valPar, newbox);
}

void tst_TcpBoxEntity::DigitalValuesCanHaveTcpBoxAsParent(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false, 0, newbox);
  auto valPar = EntityManager::get_instance()->getParent(newval);
  QCOMPARE(valPar, newbox);
}

void tst_TcpBoxEntity::removeTcpBoxRemovesAllItsValues(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  auto newval = EntityManager::get_instance()->createNewDigitalValue(VAL_NAME, false, 0, newbox);

  EntityManager::get_instance()->removeEntity(newbox);

  QCOMPARE(EntityManager::get_instance()->entityExists(newbox), false);
  QCOMPARE(EntityManager::get_instance()->entityExists(newval), false);
}

void tst_TcpBoxEntity::configuredBoxNeedsNameHostPortBackend()
{
    auto newbox = EntityManager::get_instance()->createNewTcpBox();
    auto isConfigured = TcpBoxEntityFacorty::isConfigured(newbox);
    QVERIFY(isConfigured);
}

void tst_TcpBoxEntity::removeTcpBoxEmitsTcpBoxRemovedEvent(){
  auto evt_mng = EventManager::get_instance();
  auto newbox = EntityManager::get_instance()->createNewTcpBox();

  QSignalSpy spy(evt_mng, SIGNAL(tcpBoxRemoved(int)));
  EntityManager::get_instance()->removeEntity(newbox);

  QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_TcpBoxEntity::tcpBoxEntityIsDeletedByAFactory(){
  auto newbox = EntityManager::get_instance()->createNewTcpBox();
  EntityManager::get_instance()->removeEntity(newbox);
  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  QCOMPARE(all_boxes.contains(newbox), false);
}

void tst_TcpBoxEntity::noAnalogInputValuesExistAfterStartup(){
  auto all_ais = EntityManager::get_instance()->get_all_AnalogInputValues();
  QVERIFY(all_ais.isEmpty());
}

QTEST_MAIN(tst_TcpBoxEntity)

#include "tst_TcpBoxEntity.moc"
