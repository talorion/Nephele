#include "QTofDaqRec.hpp"

#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QThread>
#include <QEventLoop>

namespace talorion {

QTofDaqRec::QTofDaqRec(QObject *par) :
    QObject(par),
    m_tofdaqdll(Q_NULLPTR),
    m_tofdaqrecproc(Q_NULLPTR)
{

}

QTofDaqRec::~QTofDaqRec()
{
    cleanupProc();
}

bool QTofDaqRec::isRunning() const
{
    if(dllOk() == false){
        return false;
    }
    return m_tofdaqdll->tofDaqRunning();
}

void QTofDaqRec::init(QTofDaqDll *tofdaqdll)
{
    m_tofdaqdll = tofdaqdll;
}

int QTofDaqRec::startTofDaqRec()
{
    if(dllOk() == false){
        qWarning()<<"could not start TofdaqRec: dll null or not initialized";
        return -1;
    }

    QString exe_name = m_tofdaqdll->tofDaqRecName();
    int timeout = m_tofdaqdll->getTimeout();

    if(isRunning() == false){
        createProc();
        m_tofdaqrecproc->start(exe_name);
    }

    if(m_tofdaqrecproc != Q_NULLPTR){
        QEventLoop tmp_evt_loop(this);
        connect(m_tofdaqrecproc ,SIGNAL(started()), &tmp_evt_loop, SLOT(quit()));
        QTimer::singleShot(5000 ,&tmp_evt_loop,SLOT(quit()));
        tmp_evt_loop.exec();
        return 4; //TwSuccess
    }else{
        QTime timer;
        timer.start();
        forever{
            QThread::currentThread()->msleep(50);
            if(timer.elapsed() > timeout)
                return 8; //TwTimeout

            if(isRunning() == true)
                return 4; //TwSuccess
        }
    }
    return 0; //TwDaqRecNotRunning
}

int QTofDaqRec::closeTofDaqRec()
{
    if(dllOk() == false){
        qWarning()<<"could not stop TofdaqRec: dll null or not initialized";
        return -1;
    }

    volatile int res = 0; //TwDaqRecNotRunning
    if(isRunning() == true)
        res =  m_tofdaqdll->closeTofDaqRec();

    if(res != 4 )//TwSuccess
        return res;

    if(m_tofdaqrecproc != Q_NULLPTR){
        QEventLoop tmp_evt_loop(this);
        connect(m_tofdaqrecproc ,SIGNAL(finished(int)), &tmp_evt_loop, SLOT(quit()));
        QTimer::singleShot(5000 ,&tmp_evt_loop,SLOT(quit()));
        tmp_evt_loop.exec();
        return 4; //TwSuccess
    }else{
        int timeout = m_tofdaqdll->getTimeout();
        QTime timer;
        timer.start();
        res = 0; //TwDaqRecNotRunning
        forever{
            QThread::currentThread()->msleep(50);
            if(timer.elapsed() > timeout){
                res = 8; //TwTimeout
                break;
            }

            if(isRunning() == false){
                res = 4; //TwSuccess
                break;
            }
        }
    }
    cleanupProc();
    return res;
}

bool QTofDaqRec::dllOk() const
{
    return (m_tofdaqdll != Q_NULLPTR) && m_tofdaqdll->isInitialized();
}

void QTofDaqRec::createProc()
{
    m_tofdaqrecproc = new QProcess();

    // create a timer with p as parent, so it will be deleted properly
    //QTimer *killtimer = new QTimer(p);
    //killtimer->setTimeout(5000); // 5 seconds
    //killtimer->setSingleShot(true); // should happen only once per process start

    // make the timer's timeout to terminate the process
    //QObject::connect(killtimer, SIGNAL(timeout()), p, SLOT(terminate()));

    // make the timer start when process starts
    //QObject::connect(p, SIGNAL(started()), killtimer, SLOT(start()));

    // you also want to connect signals to slots, and at least do debug print
    // example:
    QObject::connect(m_tofdaqrecproc, SIGNAL(started()), this, SLOT(handleStarted()));
    QObject::connect(m_tofdaqrecproc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(handleFinished(int,QProcess::ExitStatus)));
    QObject::connect(m_tofdaqrecproc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));

}

void QTofDaqRec::cleanupProc()
{
    if(m_tofdaqrecproc != Q_NULLPTR){
        closeTofDaqRec();
        delete m_tofdaqrecproc;
    }
    m_tofdaqrecproc = Q_NULLPTR;
}

void QTofDaqRec::handleStarted()
{
    qInfo()<<"Started Tof Daq Recorder";
}

void QTofDaqRec::handleFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    qInfo()<<"Daq Recorder finished with code: "<< exitCode;
}

void QTofDaqRec::handleError(QProcess::ProcessError error)
{
    qWarning()<<"Daq Recorder gor error: "<< error;
}

} // namespace talorion
