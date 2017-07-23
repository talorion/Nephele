#ifndef QTOFDAQREC_HPP
#define QTOFDAQREC_HPP

#include "QTofDaqDll.hpp"

#include <QObject>
#include <QProcess>

namespace talorion {

class QTofDaqRec : public QObject
{
    Q_OBJECT
public:
    explicit QTofDaqRec(QObject *par = 0);
    virtual ~QTofDaqRec();

private:
    Q_DISABLE_COPY(QTofDaqRec)

public:
    bool isRunning() const;

    void init(QTofDaqDll* tofdaqdll);

    int startTofDaqRec();
    int closeTofDaqRec();

    bool dllOk()const;

private:
    void createProc();
    void cleanupProc();

private slots:
    void handleStarted();
    void handleFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleError(QProcess::ProcessError error);

public:
    QTofDaqDll* m_tofdaqdll;
    QProcess* m_tofdaqrecproc;

};

} // namespace talorion

#endif // QTOFDAQREC_HPP
