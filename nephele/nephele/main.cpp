#include <QApplication>

int main(int argc, char *argv[]){

    QString v = "0.1.0.0";

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Nephele");
    QCoreApplication::setOrganizationDomain("github.com/talorion");
    QCoreApplication::setApplicationName("Nephele");
    QCoreApplication::setApplicationVersion(v);

    return a.exec();
}
