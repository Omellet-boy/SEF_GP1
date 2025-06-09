#ifndef BATTERY_MONITORING_PAGE_H
#define BATTERY_MONITORING_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QGroupBox>
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
    QLabel* createInfoBox(const QString& title, const QString& value);

    Simulator* m_simulator;
    QLabel* healthLabel;
    QLabel* temperatureLabel;
    QLabel* currentLabel;
    QLabel* chargeStatusLabel;
    QProgressBar* chargeProgressBar;
    QPushButton* stopChargeButton;
    bool solarChargeEnabled = true;

    QGroupBox* createChargeControlGroup();

};

#endif // BATTERY_MONITORING_PAGE_H
