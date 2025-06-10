#include "battery_monitoring_page.h"

BatteryMonitoringPage::BatteryMonitoringPage(Simulator* simulator, QWidget *parent)
    : QWidget(parent), m_simulator(simulator)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // --- Battery Statistics Boxes ---
    QHBoxLayout* statsLayout = new QHBoxLayout();

    healthLabel = createInfoBox("Battery Health", "-- %");
    temperatureLabel = createInfoBox("Temperature", "-- °C");
    currentLabel = createInfoBox("Current", "-- A");

    statsLayout->addWidget(healthLabel->parentWidget());
    statsLayout->addWidget(temperatureLabel->parentWidget());
    statsLayout->addWidget(currentLabel->parentWidget());

    mainLayout->addLayout(statsLayout);
    mainLayout->addWidget(createChargeControlGroup());
    mainLayout->addStretch();

    connect(m_simulator, &Simulator::dataUpdated, this, &BatteryMonitoringPage::updateBatteryData);
    updateBatteryData();
}

QLabel* BatteryMonitoringPage::createInfoBox(const QString& title, const QString& value)
{
    QGroupBox* box = new QGroupBox(title);
    box->setStyleSheet(R"(
        QGroupBox {
            background-color: #1E1E1E;
            color: #FFD700;
            border: 2px solid #444;
            border-radius: 10px;
            font-family: 'Kode Mono';
            font-size: 14px;
            padding: 10px;
        }
    )");

    QVBoxLayout* layout = new QVBoxLayout(box);
    QLabel* label = new QLabel(value);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #FFD700; font-size: 16px; font-family: 'Kode Mono';");
    layout->addWidget(label);
    layout->addStretch();

    return label; // Return the value label only
}

QGroupBox* BatteryMonitoringPage::createChargeControlGroup()
{
    QGroupBox* group = new QGroupBox("Charge Control");
    group->setStyleSheet(R"(
        QGroupBox {
            background-color: #1E1E1E;
            color: white;
            border: 2px solid #444;
            border-radius: 10px;
            font-family: 'Kode Mono';
            font-size: 14px;
            padding: 10px;
        }
    )");

    QVBoxLayout* layout = new QVBoxLayout(group);

    chargeStatusLabel = new QLabel("Charge Status: Receiving Solar Power");
    chargeStatusLabel->setStyleSheet("color: #FFD700; font-family: 'Kode Mono'; font-size: 14px;");

    chargeProgressBar = new QProgressBar();
    chargeProgressBar->setRange(0, 100);
    chargeProgressBar->setValue(0);
    chargeProgressBar->setStyleSheet(R"(
        QProgressBar {
            background-color: #2E2E2E;
            border: 1px solid #555;
            border-radius: 8px;
            text-align: center;
            color: white;
        }
        QProgressBar::chunk {
            background-color: #FFD700;
            border-radius: 8px;
        }
    )");

    stopChargeButton = new QPushButton("Stop Receiving Charge From Solar");
    stopChargeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FFD700;
            color: black;
            border-radius: 8px;
            padding: 10px;
            font-family: 'Kode Mono';
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #FFC300;
        }
    )");

    connect(stopChargeButton, &QPushButton::clicked, this, &BatteryMonitoringPage::toggleSolarCharge);

    layout->addWidget(chargeStatusLabel);
    layout->addWidget(chargeProgressBar);
    layout->addWidget(stopChargeButton);

    return group;
}

void BatteryMonitoringPage::updateBatteryData()
{
    double health = m_simulator->getDegradation();
    double temp = m_simulator->m_temperature; // or add getter
    double charge = m_simulator->batteryLevel();

    double current = (m_simulator->solarEnergy() - m_simulator->applianceLoad()) * 0.01;

    healthLabel->setText(QString("%1 %").arg(int(health)));
    temperatureLabel->setText(QString("%1 °C").arg(int(temp)));
    currentLabel->setText(QString("%1 A").arg(current, 0, 'f', 2));
    chargeProgressBar->setValue(int(charge));
    chargeStatusLabel->setText(solarChargeEnabled ? "Charge Status: Receiving Solar Power"
                                                  : "Charge Status: Stopped");
}

void BatteryMonitoringPage::toggleSolarCharge()
{
    solarChargeEnabled = !solarChargeEnabled;
    stopChargeButton->setText(solarChargeEnabled ? "Stop Receiving Charge From Solar"
                                                 : "Resume Receiving Charge From Solar");
    m_simulator->setSolarChargingEnabled(solarChargeEnabled);
    updateBatteryData();
}
