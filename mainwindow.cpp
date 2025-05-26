#include "mainwindow.h"
#include <QtCharts/QCategoryAxis>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), simulator(new Simulator(this))
{
    centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Navigation Bar
    navWidget = new QWidget();
    navLayout = new QVBoxLayout(navWidget);
    dashboardButton = new QPushButton("Dashboard");
    settingsButton = new QPushButton("Settings");
    batteryMonitoringButton = new QPushButton("Battery Monitoring");
    solarMonitoringButton = new QPushButton("Solar Monitoring");
    dataManagementButton = new QPushButton("Data Management");

    navLayout->addWidget(dashboardButton);
    navLayout->addWidget(settingsButton);
    navLayout->addWidget(batteryMonitoringButton);
    navLayout->addWidget(solarMonitoringButton);
    navLayout->addWidget(dataManagementButton);
    navLayout->addStretch();
    navWidget->setFixedWidth(150);

    // Connect buttons to pages
    connect(dashboardButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(settingsButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(1);
    });
    connect(batteryMonitoringButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(solarMonitoringButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(dataManagementButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(4);
    });

    // Pages container
    stackedWidget = new QStackedWidget();

    // Page 0 - Dashboard
    dashboardPage = new QWidget();
    QVBoxLayout *dashboardLayout = new QVBoxLayout(dashboardPage);
    dashboardLayout->addWidget(createSolarPanelGroup());
    dashboardLayout->addWidget(createBatteryGroup());
    dashboardLayout->addWidget(createSimulationInfoGroup());
    setupPowerChart();
    dashboardLayout->addWidget(powerChartView);

    // Add dummy pages for now
    QWidget *settingPage = new QLabel("Setting");
    QWidget *batteryPage = new QLabel("Battery Monitoring Page");
    QWidget *solarPage = new QLabel("Solar Monitoring Page");
    QWidget *dataPage = new QLabel("Data Management Page");

    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(settingPage);
    stackedWidget->addWidget(batteryPage);
    stackedWidget->addWidget(solarPage);
    stackedWidget->addWidget(dataPage);

    mainLayout->addWidget(navWidget);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(centralWidget);
    resize(900, 600);
    setWindowTitle("Energy Simulator Dashboard");

    connect(simulator, &Simulator::dataUpdated, this, &MainWindow::updateDashboard);
}

MainWindow::~MainWindow() {}

void MainWindow::updateDashboard()
{
    double solar = simulator->solarEnergy();
    double load = simulator->applianceLoad();
    double net = solar - load;
    double battery = simulator->batteryLevel();

    solarPowerLabel->setText(QString("Solar Output: %1 W").arg(int(solar)));
    loadLabel->setText(QString("Load: %1 W").arg(int(load)));
    netLabel->setText(QString("Net: %1 W").arg(int(net)));

    batteryBar->setValue(int(battery));
    batteryPercentLabel->setText(QString("%1%").arg(int(battery)));

    int hours = int(simulator->getSimulatedTime());
    int minutes = int((simulator->getSimulatedTime() - hours) * 60);
    QString timeStr = QString("%1:%2")
                          .arg(hours, 2, 10, QChar('0'))
                          .arg(minutes, 2, 10, QChar('0'));
    timeLabel->setText(QString("Simulated Time: %1").arg(timeStr));
    weatherLabel->setText(QString("Weather: %1").arg(simulator->getWeather()));

    double simTimeMinutes = simulator->getSimulatedTime() * 60.0;
    powerSeries->append(simTimeMinutes, net);

    while (!powerSeries->points().isEmpty() && powerSeries->points().first().x() < simTimeMinutes - 1440) {
        powerSeries->removePoints(0, 1);
    }
}

QGroupBox* MainWindow::createSolarPanelGroup()
{
    QGroupBox *group = new QGroupBox("Solar Panel");
    QVBoxLayout *layout = new QVBoxLayout(group);
    solarPowerLabel = new QLabel("Solar Output: ");
    loadLabel = new QLabel("Load: ");
    netLabel = new QLabel("Net: ");
    layout->addWidget(solarPowerLabel);
    layout->addWidget(loadLabel);
    layout->addWidget(netLabel);
    return group;
}

QGroupBox* MainWindow::createBatteryGroup()
{
    QGroupBox *group = new QGroupBox("Battery");
    QVBoxLayout *layout = new QVBoxLayout(group);
    batteryBar = new QProgressBar();
    batteryBar->setRange(0, 100);
    batteryBar->setValue(100);
    batteryPercentLabel = new QLabel("100%");
    layout->addWidget(batteryBar);
    layout->addWidget(batteryPercentLabel);
    return group;
}

QGroupBox* MainWindow::createSimulationInfoGroup()
{
    QGroupBox *group = new QGroupBox("Simulation Info");
    QVBoxLayout *layout = new QVBoxLayout(group);
    timeLabel = new QLabel("Simulated Time: ");
    weatherLabel = new QLabel("Weather: ");
    layout->addWidget(timeLabel);
    layout->addWidget(weatherLabel);
    return group;
}

void MainWindow::setupPowerChart()
{
    powerSeries = new QLineSeries();
    powerChart = new QChart();
    powerChart->addSeries(powerSeries);
    powerChart->legend()->hide();
    powerChart->setTitle("Net Power Over Time");

    QCategoryAxis *axisX = new QCategoryAxis;
    for (int hour = 0; hour <= 24; ++hour) {
        int minutes = hour * 60;
        axisX->append(QString("%1:00").arg(hour, 2, 10, QChar('0')), minutes);
    }
    axisX->setRange(0, 1440);
    axisX->setTitleText("Simulated Time");
    powerChart->addAxis(axisX, Qt::AlignBottom);
    powerSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Net Power (W)");
    axisY->setRange(-500, 1000);
    axisY->setTickInterval(250);
    axisY->setLabelFormat("%d");
    powerChart->addAxis(axisY, Qt::AlignLeft);
    powerSeries->attachAxis(axisY);

    QLineSeries *zeroLine = new QLineSeries();
    zeroLine->append(0, 0);
    zeroLine->append(1440, 0);
    zeroLine->setColor(Qt::gray);
    zeroLine->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    powerChart->addSeries(zeroLine);
    zeroLine->attachAxis(axisX);
    zeroLine->attachAxis(axisY);

    powerChartView = new QChartView(powerChart);
    powerChartView->setRenderHint(QPainter::Antialiasing);
}
