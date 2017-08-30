#ifndef SERIALBOXESTHREAD_HPP
#define SERIALBOXESTHREAD_HPP

#include <QMutex>
#include <QThread>

namespace talorion {

class SerialBoxesThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialBoxesThread(QObject *par = 0);
    virtual ~SerialBoxesThread()=default;

private:
    Q_DISABLE_COPY(SerialBoxesThread)


    // QThread interface
protected:
    virtual void run() Q_DECL_OVERRIDE;

private:
    QMutex m_mutex;
};

} // namespace talorion

#endif // SERIALBOXESTHREAD_HPP
