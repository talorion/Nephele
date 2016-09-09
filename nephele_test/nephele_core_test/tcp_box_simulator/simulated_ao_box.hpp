#ifndef SIMULATED_AO_BOX_HPP
#define SIMULATED_AO_BOX_HPP

#include "abstract_simulated_box.hpp"

#include "simulated_ao.hpp"

class simulated_ao_box : public abstract_simulated_box
{
    Q_OBJECT
public:
    explicit simulated_ao_box(QObject *par = 0);
    ~simulated_ao_box();
    Q_DISABLE_COPY(simulated_ao_box)

    virtual void set(int id, double val) Q_DECL_OVERRIDE;

    void init_all_aos();

    virtual QString getAll_json() Q_DECL_OVERRIDE;

private:
    QList<simulated_ao*> all_aos;
    int random_id;
};

#endif // SIMULATED_AO_BOX_HPP
