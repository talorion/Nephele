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
        virtual void logError(QString errorString, int source) Q_DECL_OVERRIDE;

    private slots:
        virtual void fcSetChangeProxy(int entity) Q_DECL_OVERRIDE;
        virtual void fcSetChangeProxy(double value, int id)Q_DECL_OVERRIDE ;

        virtual void dvSetChangeProxy(int entity)Q_DECL_OVERRIDE ;
        virtual void dvSetChangeProxy(int value, int id)Q_DECL_OVERRIDE ;

    private:
        void parse_alldata_AO(QVariantList& desc, int box_id);
        void parse_alldata_AI(QVariantList& desc, int box_id);
        void parse_alldata_AIO(QVariantList& desc, int box_id);
        void parse_alldata_DO(QVariantList& desc, int box_id);
        //       void parse_alldata_DI(QVariantList& desc, int box_id);
        //       void parse_alldata_DIO(QVariantList& desc, int box_id);

        void parse_actset_AO(QVariantList& desc);
        void parse_actset_AI(QVariantList& desc);
        void parse_actset_AIO(QVariantList& desc);
        void parse_actset_DO(QVariantList& desc);
        //       void parse_actset_DI(QVariantList& desc, int box_id);
        //       void parse_actset_DIO(QVariantList& desc, int box_id);

    private:
        //QList<analogValue*> flowcontroller;
        QMap<int, int> registered_values;
        //QList<int> registered_values;
        QMap<int, double> analog_actbuffer;
        //QList<double> actbuffer;
        QMap<int, double> analog_setbuffer;
        //QList<double> setbuffer;
        //const tcpDriver* drv;
        QMap<int, int> digital_actbuffer;
        QMap<int, int> digital_setbuffer;
        bool block_next_read;

        int m_box_id;
    };

} // namespace talorion

#endif // TALORION_RF_BACKEND_HPP
