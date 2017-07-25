#ifndef ANALOGPLOT_HPP
#define ANALOGPLOT_HPP

#include "AnalogIndicator.hpp"
#include "TitleLabel.hpp"

#include <QObject>
#include <qcustomplot.h>

namespace talorion {

  using graphHistory_t = QSharedPointer< QCPGraphDataContainer >;

class AnalogPlot : public QWidget
{
    Q_OBJECT
public:
    explicit AnalogPlot(QWidget *par = nullptr);
    virtual ~AnalogPlot();

    int graphPersistanceTime() const;
    void setGraphPersistanceTime(int graphPersistanceTime);

private:
    Q_DISABLE_COPY(AnalogPlot)

public slots:
    void setRanges(double lowerRange, double upperRange);
    void updatePlot(double value);
    void updatePlot(QCPGraphData& val);
    void setTitle(const QString& title);
    void resetData(const graphHistory_t& history );

private slots:
    void newValueTimeout();

private:
    TitleLabel* m_title;
    AnalogIndicator* m_actValue;
    QGridLayout* m_layout;
    QCustomPlot* m_plot;
    QCPGraph* m_actGraph;
    int m_graphPersistanceTime;
    QTimer* m_graphTimer;
    double m_lastValue;
};

} // namespace talorion

#endif // ANALOGPLOT_HPP
