#include "solar_monitoring_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

SolarMonitoringPage::SolarMonitoringPage(Simulator* simulator, QWidget *parent)
    : QWidget(parent), m_simulator(simulator)
{
    // Initialize UI
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    mainLayout->addWidget(createSolarOutputGroup());
    mainLayout->addWidget(createSolarChartGroup());
    mainLayout->addWidget(createEfficiencyGroup());
    mainLayout->addStretch();

    // Connect to simulator updates
    connect(m_simulator, &Simulator::dataUpdated, this, &SolarMonitoringPage::updateSolarData);
}

SolarMonitoringPage::~SolarMonitoringPage()
{
    // Clean up
}

QGroupBox* SolarMonitoringPage::createSolarOutputGroup()
{
    QGroupBox* group = new QGroupBox("Solar Output Control");
    group->setStyleSheet(R"(
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

    QVBoxLayout* layout = new QVBoxLayout(group);

    m_solarOutputBar = new QProgressBar();
    m_solarOutputBar->setRange(0, 1000);
    m_solarOutputBar->setValue(0);
    m_solarOutputBar->setFormat("%v W");
    m_solarOutputBar->setStyleSheet(R"(
        QProgressBar {
            background-color: #2E2E2E;
            border: 1px solid #555;
            border-radius: 8px;
            text-align: center;
            color: white;
            height: 30px;
            font-family: 'Kode Mono';
        }
        QProgressBar::chunk {
            background-color: #FFA500;
            border-radius: 8px;
        }
    )");

    m_solarOutputLabel = new QLabel("Current Output: 0 W");
    m_solarOutputLabel->setStyleSheet("color: #FFD700; font-size: 16px; font-family: 'Kode Mono';");

    m_peakOutputLabel = new QLabel("Peak Today: 0 W");
    m_peakOutputLabel->setStyleSheet("color: #FFD700; font-size: 14px; font-family: 'Kode Mono';");

    m_weatherLabel = new QLabel("Weather Impact: None");
    m_weatherLabel->setStyleSheet("color: #FFD700; font-size: 14px; font-family: 'Kode Mono';");

    m_toggleProductionButton = new QPushButton("Stop Solar Production");
    m_toggleProductionButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FF5555;
            color: white;
            border-radius: 8px;
            padding: 10px;
            font-family: 'Kode Mono';
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #FF3333;
        }
    )");

    connect(m_toggleProductionButton, &QPushButton::clicked, this, &SolarMonitoringPage::toggleSolarProduction);

    layout->addWidget(m_solarOutputBar);
    layout->addWidget(m_solarOutputLabel);
    layout->addWidget(m_peakOutputLabel);
    layout->addWidget(m_weatherLabel);
    layout->addWidget(m_toggleProductionButton);

    return group;
}

QGroupBox* SolarMonitoringPage::createEfficiencyGroup()
{
    QGroupBox* group = new QGroupBox("System Efficiency");
    group->setStyleSheet(R"(
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

    QVBoxLayout* layout = new QVBoxLayout(group);

    m_efficiencyLabel = new QLabel("Current Efficiency: 0%");
    m_efficiencyLabel->setStyleSheet("color: #FFD700; font-size: 16px; font-family: 'Kode Mono';");

    QLabel* infoLabel = new QLabel("Efficiency affected by:\n- Weather conditions\n- Panel angle\n- Temperature");
    infoLabel->setStyleSheet("color: #AAAAAA; font-size: 12px; font-family: 'Kode Mono';");

    layout->addWidget(m_efficiencyLabel);
    layout->addWidget(infoLabel);

    return group;
}

QGroupBox* SolarMonitoringPage::createSolarChartGroup()
{
    QGroupBox* group = new QGroupBox("Solar Output History");
    group->setStyleSheet(R"(
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

    QVBoxLayout* layout = new QVBoxLayout(group);
    layout->setContentsMargins(0, 0, 0, 0);

    setupSolarChart();
    m_solarChartView->setMinimumSize(600, 300);
    layout->addWidget(m_solarChartView);

    return group;
}

void SolarMonitoringPage::setupSolarChart()
{
    m_solarSeries = new QLineSeries();
    QPen orangePen(QColor("#FFA500"));
    orangePen.setWidth(2);
    m_solarSeries->setPen(orangePen);

    m_solarChart = new QChart();
    m_solarChart->addSeries(m_solarSeries);
    m_solarChart->legend()->hide();
    m_solarChart->setTitle("Solar Output Over Time");
    m_solarChart->setBackgroundBrush(QBrush(QColor("#1E1E1E")));
    m_solarChart->setTitleBrush(QBrush(QColor("#FFD700")));

    // X Axis (Time)
    m_axisX = new QCategoryAxis();
    m_axisX->setTitleText("Time (hours)");
    m_axisX->setLabelsColor(QColor("#FFD700"));
    m_axisX->setTitleBrush(QBrush(QColor("#FFD700")));
    for (int hour = 0; hour <= 24; hour += 3) {
        m_axisX->append(QString("%1:00").arg(hour), hour * 60);
    }
    m_axisX->setRange(0, 1440);
    m_solarChart->addAxis(m_axisX, Qt::AlignBottom);
    m_solarSeries->attachAxis(m_axisX);

    // Y Axis (Power)
    m_axisY = new QValueAxis();
    m_axisY->setTitleText("Power (W)");
    m_axisY->setLabelsColor(QColor("#FFD700"));
    m_axisY->setTitleBrush(QBrush(QColor("#FFD700")));
    m_axisY->setRange(0, 1200);
    m_axisY->setTickCount(7);
    m_solarChart->addAxis(m_axisY, Qt::AlignLeft);
    m_solarSeries->attachAxis(m_axisY);

    m_solarChartView = new QChartView(m_solarChart);
    m_solarChartView->setRenderHint(QPainter::Antialiasing);
}

void SolarMonitoringPage::updateSolarData()
{
    // Get current solar data from simulator
    double solarOutput = m_productionEnabled ? m_simulator->solarEnergy() : 0.0;
    QString weather = m_simulator->getWeather();
    double simTimeMinutes = m_simulator->getSimulatedTime() * 60.0;

    // Debug output to verify data is coming in
    qDebug() << "Solar Output:" << solarOutput << "W at" << simTimeMinutes << "minutes";

    // Update peak output
    if (solarOutput > m_peakOutput) {
        m_peakOutput = solarOutput;
    }

    // Update UI elements
    m_solarOutputBar->setValue(static_cast<int>(solarOutput));
    m_solarOutputLabel->setText(QString("Current Output: %1 W").arg(solarOutput, 0, 'f', 1));
    m_peakOutputLabel->setText(QString("Peak Today: %1 W").arg(m_peakOutput, 0, 'f', 1));

    // Calculate efficiency
    double efficiency = qMin((solarOutput / 1000) * 100, 100.0);
    m_efficiencyLabel->setText(QString("Current Efficiency: %1%").arg(efficiency, 0, 'f', 1));

    // Update weather impact
    QString weatherImpact;
    if (weather == "Sunny") weatherImpact = "High (+20%)";
    else if (weather == "Clear") weatherImpact = "Normal";
    else if (weather == "Cloudy") weatherImpact = "Low (-40%)";
    else weatherImpact = "Very Low (-80%)"; // Rainy or other
    m_weatherLabel->setText(QString("Weather Impact: %1 (%2)").arg(weather, weatherImpact));

    // Update chart data
    m_solarSeries->append(simTimeMinutes, solarOutput);

    // Clear old data points beyond 24 hours
    while (!m_solarSeries->points().isEmpty() &&
           m_solarSeries->points().first().x() < (simTimeMinutes - 1440)) {
        m_solarSeries->remove(0);
    }
}

void SolarMonitoringPage::toggleSolarProduction()
{
    m_productionEnabled = !m_productionEnabled;

    if (m_productionEnabled) {
        m_toggleProductionButton->setText("Stop Solar Production");
        m_toggleProductionButton->setStyleSheet(R"(
            background-color: #FF5555;
            color: white;
            border-radius: 8px;
            padding: 10px;
            font-family: 'Kode Mono';
            font-size: 14px;
        )");
    } else {
        m_toggleProductionButton->setText("Resume Solar Production");
        m_toggleProductionButton->setStyleSheet(R"(
            background-color: #55FF55;
            color: black;
            border-radius: 8px;
            padding: 10px;
            font-family: 'Kode Mono';
            font-size: 14px;
        )");
    }

    // Force immediate update
    updateSolarData();
}
