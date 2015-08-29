#include "analogview.hpp"


namespace talorion {

    analogView::analogView(analogValue *ref_analogValue, int hash, QWidget *par):
        QWidget(par),
        data(ref_analogValue),
        hashValue(hash)
    {
        //connect(data,SIGNAL(actChanged(double)),this,SLOT(changeActValue(double)));
        //connect(data,SIGNAL(setChangedByConnection(double)),this, SLOT(changeSetValue(double)));
        //connect(this, SIGNAL(setValueChangedByGui(double)), data, SLOT(changeSetByGui(double)));
    }

    analogView::~analogView()
    {

    }
}
