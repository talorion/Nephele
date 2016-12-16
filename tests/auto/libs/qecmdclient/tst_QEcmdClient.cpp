#include <QString>
#include <QtTest>

#include <QEcmdClient.hpp>

using namespace talorion;

class tst_QEcmdClient : public QObject
{
  Q_OBJECT

public:
  tst_QEcmdClient();

private Q_SLOTS:

};

tst_QEcmdClient::tst_QEcmdClient()
{
}

QTEST_APPLESS_MAIN(tst_QEcmdClient)

#include "tst_QEcmdClient.moc"
