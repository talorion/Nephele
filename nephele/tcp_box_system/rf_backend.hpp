#ifndef TALORION_RF_BACKEND_HPP
#define TALORION_RF_BACKEND_HPP

#include "abstract_backend.hpp"

#include <QVariant>

namespace talorion {
    class tcpDriver;

    class rf_backend :
            public abstract_backend

    {
        Q_OBJECT
    public:
        explicit rf_backend(QObject *par = 0);
        ~rf_backend(void);                                  // not desctructable
        Q_DISABLE_COPY(rf_backend)

    public slots:
        virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id) Q_DECL_OVERRIDE;
        virtual void logError(QString errorString) Q_DECL_OVERRIDE;

    private slots:
        virtual void fcSetChangeProxy(int entity) Q_DECL_OVERRIDE;
        virtual void fcSetChangeProxy(double value, int id)Q_DECL_OVERRIDE ;

    private:
       void parse_all_AO(QVariantList desc, int box_id);
       void parse_all_AI(QVariantList desc, int box_id);

    private:
        //QList<analogValue*> flowcontroller;
        QList<int> flowcontroller;
        QList<double> actbuffer;
        QMap<int, double> setbuffer;
        //QList<double> setbuffer;
        //const tcpDriver* drv;
        bool block_next_read;
    };

} // namespace talorion

#endif // TALORION_RF_BACKEND_HPP
