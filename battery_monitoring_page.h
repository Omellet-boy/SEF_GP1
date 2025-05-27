#ifndef BATTERY_MONITORING_PAGE_H
#define BATTERY_MONITORING_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include "simulator.h"

class BatteryMonitoringPage : public QWidget
{
    Q_OBJECT

public:
    explicit BatteryMonitoringPage(Simulator* simulator, QWidget *parent = nullptr);

private slots:
    void updateBatteryData();
    void toggleSolarCharge();

private:
    Simulator* m_simulator;

    QLabel* healthLabel;
    QLabel* temperatureLabel;
    QLabel* currentLabel;
    QLabel* chargeStatusLabel;
    QProgressBar* chargeProgressBar;
    QPushButton* stopChargeButton;

    bool solarChargeEnabled = true;
};

#endif // BATTERY_MONITORING_PAGE_H
