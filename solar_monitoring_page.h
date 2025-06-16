#ifndef SOLAR_MONITORING_PAGE_H
#define SOLAR_MONITORING_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QCategoryAxis>
#include "simulator.h"

class SolarMonitoringPage : public QWidget
{
    Q_OBJECT

public:
    explicit SolarMonitoringPage(Simulator* simulator, QWidget *parent = nullptr);
    ~SolarMonitoringPage();

private slots:
    void updateSolarData();
    void toggleSolarProduction();

private:
    Simulator* m_simulator;

    // UI Elements
    QLabel* m_solarOutputLabel;
    QLabel* m_efficiencyLabel;
    QLabel* m_weatherLabel;
    QLabel* m_peakOutputLabel;
    QProgressBar* m_solarOutputBar;
    QPushButton* m_toggleProductionButton;
    QChartView* m_solarChartView;
    QLineSeries* m_solarSeries;
    QChart* m_solarChart;
    QValueAxis* m_axisY;
    QCategoryAxis* m_axisX;

    double m_peakOutput = 0.0;
    bool m_productionEnabled = true;
    QList<QPointF> m_dataPoints;

    QGroupBox* createSolarOutputGroup();
    QGroupBox* createEfficiencyGroup();
    QGroupBox* createSolarChartGroup();
    void setupSolarChart();
    void clearOldData();
};

#endif // SOLAR_MONITORING_PAGE_H
