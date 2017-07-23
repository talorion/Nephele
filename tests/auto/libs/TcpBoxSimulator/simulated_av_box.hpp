#ifndef SIMULATED_AV_BOX_HPP
#define SIMULATED_AV_BOX_HPP

#include "abstract_simulated_box.hpp"
#include "tcpboxsimulator_global.hpp"
#include "simulated_av.hpp"

class TCPBOXSIMULATORSHARED_EXPORT simulated_av_box : public abstract_simulated_box
{
    Q_OBJECT
public:
    explicit simulated_av_box(QObject *par = 0);
    ~simulated_av_box();
    Q_DISABLE_COPY(simulated_av_box)

    virtual void set(int id, double val) Q_DECL_OVERRIDE;

    void init_all_avs();

    virtual QString getAll_json() Q_DECL_OVERRIDE;

private:
    QList<simulated_av*> all_avs;
    int random_id;
};

#endif // SIMULATED_AV_BOX_HPP
