#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <DataAquisitionSystem/DataAquisitionSystem.hpp>

#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntity.hpp>
#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntityFactory.hpp>

#include <DataAquisitionSystem/ConfigurationWidget/DataAquisitionConfigurationWidget.hpp>

using namespace talorion;

class tst_DataAquisitionConfigurationWidget : public QObject
{
    Q_OBJECT

public:
    tst_DataAquisitionConfigurationWidget();

private Q_SLOTS:
    void DataAquisitionConfigurationWidgetIsProvidedByTofwerkDaqServerSystem();
    void DataAquisitionConfigurationWidgetCanChangeDllPath();
    void DataAquisitionConfigurationWidgetCanChangeTimout();
    void DataAquisitionConfigurationWidgetCanChangeUpdaterate();
    void DataAquisitionConfigurationWidgetCanChangeUserDataPath();
};

tst_DataAquisitionConfigurationWidget::tst_DataAquisitionConfigurationWidget()
{
}

void tst_DataAquisitionConfigurationWidget::DataAquisitionConfigurationWidgetIsProvidedByTofwerkDaqServerSystem()
{
    DataAquisitionSystem newsys;
    auto cfg_wdgt = newsys.get_configuration_widget();
    QVERIFY(cfg_wdgt != Q_NULLPTR);
}

void tst_DataAquisitionConfigurationWidget::DataAquisitionConfigurationWidgetCanChangeDllPath()
{
    auto entity_id=DaqDllEntityFactory::createNewDaqDllEntity();
    DaqDllEntity helper(entity_id);
    DataAquisitionConfigurationWidget cfg_wdg;

    QString new_dllPath("C:\\Tofwerk\\TofDaq_1.98\\TofDaqDll.dll");
    auto lineEdit = cfg_wdg.dllNameField();
    QTest::keyPress(lineEdit, Qt::Key_A, Qt::ControlModifier);
    QTest::keyPress(lineEdit, Qt::Key_Backspace);
    QTest::keyClicks(lineEdit, new_dllPath);
    QTest::keyPress(lineEdit, Qt::Key_Return);
    auto ent_to = helper.dllPath();
    QCOMPARE(ent_to, new_dllPath);
}

void tst_DataAquisitionConfigurationWidget::DataAquisitionConfigurationWidgetCanChangeTimout()
{
    auto entity_id=DaqDllEntityFactory::createNewDaqDllEntity();
    DaqDllEntity helper(entity_id);
    DataAquisitionConfigurationWidget cfg_wdg;
    auto new_ent_to = helper.timeout()+1;

    QString tmp = QString::number(new_ent_to);
    auto to_filed = cfg_wdg.timeoutField();
    QTest::keyPress(to_filed, Qt::Key_A, Qt::ControlModifier);
    QTest::keyPress(to_filed, Qt::Key_Backspace);
    QTest::keyClicks(to_filed, tmp);
    QTest::keyPress(to_filed, Qt::Key_Return);

    auto ent_to = helper.timeout();
    QCOMPARE(new_ent_to, ent_to);
}

void tst_DataAquisitionConfigurationWidget::DataAquisitionConfigurationWidgetCanChangeUpdaterate()
{
    auto entity_id=DaqDllEntityFactory::createNewDaqDllEntity();
    DaqDllEntity helper(entity_id);
    DataAquisitionConfigurationWidget cfg_wdg;
    auto new_ent_to = helper.updaterate()+1;

    QString tmp = QString::number(new_ent_to);
    auto to_filed = cfg_wdg.userDataUpdaterateField();
    QTest::keyPress(to_filed, Qt::Key_A, Qt::ControlModifier);
    QTest::keyPress(to_filed, Qt::Key_Backspace);
    QTest::keyClicks(to_filed, tmp);
    QTest::keyPress(to_filed, Qt::Key_Return);

    auto ent_to = helper.updaterate();
    QCOMPARE(new_ent_to, ent_to);
}

void tst_DataAquisitionConfigurationWidget::DataAquisitionConfigurationWidgetCanChangeUserDataPath()
{
    auto entity_id=DaqDllEntityFactory::createNewDaqDllEntity();
    DaqDllEntity helper(entity_id);
    DataAquisitionConfigurationWidget cfg_wdg;

    QString new_dllPath ="hello world";
    auto lineEdit = cfg_wdg.userDataPathField();
    QTest::keyPress(lineEdit, Qt::Key_A, Qt::ControlModifier);
    QTest::keyPress(lineEdit, Qt::Key_Backspace);
    QTest::keyClicks(lineEdit, new_dllPath);
    QTest::keyPress(lineEdit, Qt::Key_Return);
    auto ent_to = helper.userDataPath();
    QCOMPARE(ent_to, new_dllPath);
}

QTEST_MAIN(tst_DataAquisitionConfigurationWidget)

#include "tst_DataAquisitionConfigurationWidget.moc"
