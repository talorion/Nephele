#ifndef SIMULATED_FC_BOX_H
#define SIMULATED_FC_BOX_H

#include "abstract_simulated_box.hpp"
#include <QList>
#include "tcpboxsimulator_global.hpp"
#include "simulated_fc.h"

class TCPBOXSIMULATORSHARED_EXPORT simulated_fc_box : public abstract_simulated_box
{
    Q_OBJECT
public:
    explicit simulated_fc_box(QObject *par = 0);
    ~simulated_fc_box();
    Q_DISABLE_COPY(simulated_fc_box)

    virtual void set(int id, double val) Q_DECL_OVERRIDE;

    void init_all_fcs();

    virtual QString getAll_json() Q_DECL_OVERRIDE;

private:
    QList<simulated_fc*> all_fcs;
    int random_id;

};

#endif // SIMULATED_FC_BOX_H
