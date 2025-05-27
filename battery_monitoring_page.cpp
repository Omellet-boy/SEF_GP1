#include "battery_monitoring_page.h"

BatteryMonitoringPage::BatteryMonitoringPage(Simulator* simulator, QWidget *parent)
    : QWidget(parent), m_simulator(simulator)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    healthLabel = new QLabel("Battery Health: -- %");
    temperatureLabel = new QLabel("Battery Temperature: -- °C");
    currentLabel = new QLabel("Battery Current: -- A");
    chargeStatusLabel = new QLabel("Charge Status: Receiving Solar Power");
    chargeProgressBar = new QProgressBar();
    chargeProgressBar->setRange(0, 100);
    chargeProgressBar->setValue(0);

    stopChargeButton = new QPushButton("Stop Receiving Charge From Solar");
    connect(stopChargeButton, &QPushButton::clicked, this, &BatteryMonitoringPage::toggleSolarCharge);

    layout->addWidget(healthLabel);
    layout->addWidget(temperatureLabel);
    layout->addWidget(currentLabel);
    layout->addWidget(chargeStatusLabel);
    layout->addWidget(chargeProgressBar);
    layout->addWidget(stopChargeButton);

    // Connect simulator updates to UI refresh
    connect(m_simulator, &Simulator::dataUpdated, this, &BatteryMonitoringPage::updateBatteryData);

    updateBatteryData(); // initial update
}

void BatteryMonitoringPage::updateBatteryData()
{
    double health = m_simulator->getDegradation();
    double temp = m_simulator->m_temperature;          // Make m_temperature public or add getter
    double charge = m_simulator->batteryLevel();

    // Example current approximation: net energy change * 10 (you can refine)
    double current = (m_simulator->solarEnergy() - m_simulator->applianceLoad()) * 0.01;

    healthLabel->setText(QString("Battery Health: %1 %").arg(int(health)));
    temperatureLabel->setText(QString("Battery Temperature: %1 °C").arg(int(temp)));
    currentLabel->setText(QString("Battery Current: %1 A").arg(current, 0, 'f', 2));
    chargeProgressBar->setValue(int(charge));
    chargeStatusLabel->setText(solarChargeEnabled ? "Charge Status: Receiving Solar Power"
                                                  : "Charge Status: Stopped");
}

void BatteryMonitoringPage::toggleSolarCharge()
{
    solarChargeEnabled = !solarChargeEnabled;
    stopChargeButton->setText(solarChargeEnabled ? "Stop Receiving Charge From Solar"
                                                 : "Resume Receiving Charge From Solar");

    // Control the battery charging logic here:
    // A simple approach is to block solar energy input in the simulator

    if (solarChargeEnabled) {
        // Re-enable solar charging by connecting or setting a flag in simulator
        // You need to add a method or flag to Simulator to respect this
        m_simulator->setSolarChargingEnabled(true);
    } else {
        m_simulator->setSolarChargingEnabled(false);
    }

    updateBatteryData();
}
