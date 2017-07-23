#include <QtTest>
#include <QCoreApplication>

#include <tcp_box_system/tcp_box_system.hpp>

#include <tcp_box_system/tbs_config_widget/tbs_config_widget.hpp>

#include <QSize>

#include <core/EventManager.hpp>
#include <core/EntityManager.hpp>

using namespace talorion;

class tst_tbs_config_widget : public QObject
{
    Q_OBJECT

public:
    tst_tbs_config_widget();
    ~tst_tbs_config_widget();

private slots:
    //void initTestCase(); //will be called before the first testfunction is executed.
    //void cleanupTestCase(); //will be called after the last testfunction was executed.
    void init(); //will be called before each testfunction is executed.
    void cleanup(); //will be called before each testfunction is executed.

    void isProvidedByTcpBoxSystem();
    void boxListIsCreatedByConstructor();
    void boxListIsEmptyAfterCreation();
    void boxListIsNotEmptyWhenTcpBoxIsDefined();

    void addButtonCreatesATcpBox();
    void removeButtonDeletesSelectedTcpBox();

    void noBoxSelectedAfterCreation();
    void boxIsSelectedAfterClicked();

    void boxIsRemovedFromListWhenRemoved();
    void removeButtonDeletesSelectedTcpBoxFromList();
    void removeButtonDoesNothingIfNoBoxIsSelected();

    void removeButtonIsDisabledWhenNoBoxIsSelected();
    void nameFieldIsDisabledWhenNoBoxIsSelected();
    void hostFieldIsDisabledWhenNoBoxIsSelected();
    void portFieldIsDisabledWhenNoBoxIsSelected();
    void backendFieldIsDisabledWhenNoBoxIsSelected();
    void connectButtonFieldIsDisabledWhenNoBoxIsSelected();
    void disconnectButtonFieldIsDisabledWhenNoBoxIsSelected();

    void removeButtonIsEnabledWhenBoxIsSelected();
    void nameFieldIsEnabledWhenBoxIsSelected();
    void hostFieldIsEnabledWhenBoxIsSelected();
    void portFieldIsEnabledWhenBoxIsSelected();
    void backendFieldIsEnabledWhenBoxIsSelected();

    void configureButtonIsEnabledWhenBoxIsConnected();
};

tst_tbs_config_widget::tst_tbs_config_widget()
{

}

tst_tbs_config_widget::~tst_tbs_config_widget()
{

}

void tst_tbs_config_widget::init()
{
    auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    QVERIFY(all_boxes.isEmpty());
}

void tst_tbs_config_widget::cleanup()
{
    auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    foreach (auto box, all_boxes) {
        EntityManager::get_instance()->removeEntity(box);
    }
}


void tst_tbs_config_widget::isProvidedByTcpBoxSystem()
{
    tcp_box_system sys;
    auto cfg_wdg = sys.get_configuration_widget();
    QVERIFY(cfg_wdg != Q_NULLPTR);
}

void tst_tbs_config_widget::boxListIsCreatedByConstructor()
{
    tbs_config_widget cfg_wdg;
    auto boxLst = cfg_wdg.getBoxes_list();
    QVERIFY(boxLst != Q_NULLPTR);
}

void tst_tbs_config_widget::boxListIsEmptyAfterCreation()
{
    tbs_config_widget cfg_wdg;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto size = boxLst->count();
    QCOMPARE(size, 0);
}

void tst_tbs_config_widget::boxListIsNotEmptyWhenTcpBoxIsDefined()
{
    EntityManager::get_instance()->createNewTcpBox();

    tbs_config_widget cfg_wdg;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto size = boxLst->count();

    QVERIFY(size == 1);
}

void tst_tbs_config_widget::addButtonCreatesATcpBox(){
    auto evt_mng = EventManager::get_instance();
    QSignalSpy spy(evt_mng, SIGNAL(newTcpBox(int)));
    tbs_config_widget cfg_wdg;
    auto addBut = cfg_wdg.getAdd_button();

    QTest::mouseClick(addBut, Qt::LeftButton);

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_tbs_config_widget::removeButtonDeletesSelectedTcpBox(){
    EntityManager::get_instance()->createNewTcpBox();
    auto evt_mng = EventManager::get_instance();
    QSignalSpy spy(evt_mng, SIGNAL(tcpBoxRemoved(int)));
    tbs_config_widget cfg_wdg;
    auto remBut = cfg_wdg.getRemove_button();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QTest::mouseClick(remBut, Qt::LeftButton);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

void tst_tbs_config_widget::noBoxSelectedAfterCreation(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto sel_ent = cfg_wdg.getCurrent_entity();
    auto exp_id = EntityManager::invalid_id;

    QCOMPARE(sel_ent, exp_id);
}

void tst_tbs_config_widget::boxIsSelectedAfterClicked(){
    int sel_row = 0;
    auto box = EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    auto sel_ent = cfg_wdg.getCurrent_entity();
    QCOMPARE(sel_ent, box);
}

void tst_tbs_config_widget::boxIsRemovedFromListWhenRemoved(){
    auto box = EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->count(),  1);
    EntityManager::get_instance()->removeEntity(box);
    QCOMPARE(boxLst->count(),  0);
}

void tst_tbs_config_widget::removeButtonDeletesSelectedTcpBoxFromList(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto remBut = cfg_wdg.getRemove_button();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QTest::mouseClick(remBut, Qt::LeftButton);
    QCOMPARE(boxLst->count(),  0);
}

void tst_tbs_config_widget::removeButtonDoesNothingIfNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto remBut = cfg_wdg.getRemove_button();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(boxLst->count(),  1);
    QTest::mouseClick(remBut, Qt::LeftButton);
    QCOMPARE(boxLst->count(),  1);
}

void tst_tbs_config_widget::removeButtonIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto remBut = cfg_wdg.getRemove_button();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(remBut->isEnabled(),  false);
}

void tst_tbs_config_widget::nameFieldIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxnme = cfg_wdg.getBox_name_field();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(bxnme->isEnabled(),  false);
}

void tst_tbs_config_widget::hostFieldIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxhst = cfg_wdg.getIp_address_field();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(bxhst->isEnabled(),  false);
}

void tst_tbs_config_widget::portFieldIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxprt = cfg_wdg.getPortField();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(bxprt->isEnabled(),  false);
}

void tst_tbs_config_widget::backendFieldIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxbckend = cfg_wdg.getBackend_field();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(bxbckend->isEnabled(),  false);
}

void tst_tbs_config_widget::connectButtonFieldIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto conBtn = cfg_wdg.getConnect_button();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(conBtn->isEnabled(),  false);
}

void tst_tbs_config_widget::disconnectButtonFieldIsDisabledWhenNoBoxIsSelected(){
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto dconBtn = cfg_wdg.getDisconnect_button();
    auto boxLst = cfg_wdg.getBoxes_list();

    QCOMPARE(boxLst->currentRow (), -1 );
    QCOMPARE(dconBtn->isEnabled(),  false);
}

void tst_tbs_config_widget::removeButtonIsEnabledWhenBoxIsSelected()
{
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto remBut = cfg_wdg.getRemove_button();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QCOMPARE(remBut->isEnabled(),  true);
}

void tst_tbs_config_widget::nameFieldIsEnabledWhenBoxIsSelected()
{
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxnme = cfg_wdg.getBox_name_field();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QCOMPARE(bxnme->isEnabled(),  true);
}

void tst_tbs_config_widget::hostFieldIsEnabledWhenBoxIsSelected()
{
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxhst = cfg_wdg.getIp_address_field();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QCOMPARE(bxhst->isEnabled(),  true);
}

void tst_tbs_config_widget::portFieldIsEnabledWhenBoxIsSelected()
{
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxprt = cfg_wdg.getPortField();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QCOMPARE(bxprt->isEnabled(),  true);
}

void tst_tbs_config_widget::backendFieldIsEnabledWhenBoxIsSelected()
{
    EntityManager::get_instance()->createNewTcpBox();
    tbs_config_widget cfg_wdg;
    auto bxbckend = cfg_wdg.getBackend_field();

    int sel_row = 0;
    auto boxLst = cfg_wdg.getBoxes_list();
    auto item = boxLst->item ( sel_row );
    auto rect = boxLst->visualItemRect ( item );
    QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

    QCOMPARE(boxLst->count(),  1);
    QCOMPARE(boxLst->currentRow (), sel_row );
    QVERIFY(boxLst->currentItem () != Q_NULLPTR );
    QCOMPARE(bxbckend->isEnabled(),  true);
}

void tst_tbs_config_widget::configureButtonIsEnabledWhenBoxIsConnected()
{
  auto box = EntityManager::get_instance()->createNewTcpBox();
  tbs_config_widget cfg_wdg;

  EntityManager::get_instance()->slot_connection_state_component(box, true);

  int sel_row = 0;
  auto boxLst = cfg_wdg.getBoxes_list();
  auto item = boxLst->item ( sel_row );
  auto rect = boxLst->visualItemRect ( item );
  QTest::mouseClick ( boxLst->viewport(), Qt::LeftButton, 0, rect.center() );

  QCOMPARE(boxLst->count(),  1);
  QCOMPARE(boxLst->currentRow (), sel_row );
  QCOMPARE(cfg_wdg.getConfigure_button()->isEnabled(), true );
}

QTEST_MAIN(tst_tbs_config_widget)

#include "tst_tbs_config_widget.moc"
