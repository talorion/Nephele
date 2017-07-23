#ifndef DATAAQUISITIONTHREAD_HPP
#define DATAAQUISITIONTHREAD_HPP

#include <QMutex>
#include <QObject>
#include <QThread>

namespace talorion {

class DataAquisitionThread : public QThread
{
    Q_OBJECT
public:
    explicit DataAquisitionThread(QObject *par = 0);
    virtual ~DataAquisitionThread()=default;

private:
    Q_DISABLE_COPY(DataAquisitionThread)

public:
    Qt::HANDLE threadId();

private:
    Qt::HANDLE m_threadId;
    QMutex m_mutex;

    // QThread interface
protected:
    virtual void run() Q_DECL_OVERRIDE;
};

} // namespace talorion

#endif // DATAAQUISITIONTHREAD_HPP
