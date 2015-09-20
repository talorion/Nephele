#ifndef TALORION_ABSTRACT_BACKEND_HPP
#define TALORION_ABSTRACT_BACKEND_HPP

#include <QObject>
#include <QMap>
#include <QVariantMap>

#include "tcpdriverdatatypes.hpp"

namespace talorion {

    class abstract_backend : public QObject
    {
        Q_OBJECT
    public:
        explicit abstract_backend(QObject *par = 0);
        virtual ~abstract_backend();
        Q_DISABLE_COPY(abstract_backend)

    signals:
        void fcSetChangeCommand(QByteArray command);
        void change_act_value(int, double);
        void change_set_value(int, double);

        void change_digital_act_value(int, bool);
        void change_digital_set_value(int, bool);

    public slots:
        virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)=0;
        virtual void logError(QString errorString)=0;

    private slots:
        virtual void fcSetChangeProxy(int entity)=0;
        virtual void fcSetChangeProxy(double value, int id)=0;

        virtual void dvSetChangeProxy(int entity)=0;
        virtual void dvSetChangeProxy(int value, int id)=0;

    };

} // namespace talorion

#endif // TALORION_ABSTRACT_BACKEND_HPP
