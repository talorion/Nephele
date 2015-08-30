#ifndef SIMULATED_FC_BOX_H
#define SIMULATED_FC_BOX_H

#include <QObject>
#include <QList>

#include "simulated_fc.h"

class simulated_fc_box : public QObject
{
    Q_OBJECT
public:
    explicit simulated_fc_box(QObject *par = 0);
    ~simulated_fc_box();
    Q_DISABLE_COPY(simulated_fc_box)

    void set(int id, double val);

    void init_all_fcs();

    QString getAll_json();

private:
    QList<simulated_fc*> all_fcs;

};

#endif // SIMULATED_FC_BOX_H
