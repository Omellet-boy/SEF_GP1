#include "mainwindow.h"
#include <QtCharts/QCategoryAxis>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include "settingwindow.h"
#include "battery_monitoring_page.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), simulator(new Simulator(this)), database(new Database(this))
{
    database->init();

    centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color: #1F2022;");
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

    // Dummy Pages
    QWidget *settingPage = new SettingsWindow;
    BatteryMonitoringPage *batteryPage = new BatteryMonitoringPage(simulator);
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

    if (!powerSeries->points().isEmpty() && simTimeMinutes < powerSeries->points().last().x()) {
        powerSeries->clear();
    }

    powerSeries->append(simTimeMinutes, net);

    if (database) {
        database->logSimulationData(simTimeMinutes, solar, load, net, battery, simulator->getWeather());
    }
}

QGroupBox* MainWindow::createSolarPanelGroup()
{
    QGroupBox *group = new QGroupBox("Solar Panel");
    group->setStyleSheet(R"(
        QGroupBox {
            background-color: #1E1E1E;
            color: white;
            border: 2px solid #444;
            border-radius: 10px;
            margin-top: 10px;
            font-family: 'Kode Mono', monospace;
            font-size: 14px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(group);
    solarPowerLabel = new QLabel("Solar Output: ");
    loadLabel = new QLabel("Load: ");
    netLabel = new QLabel("Net: ");

    QString labelStyle = "color: #FFD700; font-size: 14px; font-family: 'Kode Mono';";
    solarPowerLabel->setStyleSheet(labelStyle);
    loadLabel->setStyleSheet(labelStyle);
    netLabel->setStyleSheet(labelStyle);

    layout->addWidget(solarPowerLabel);
    layout->addWidget(loadLabel);
    layout->addWidget(netLabel);
    return group;
}

QGroupBox* MainWindow::createBatteryGroup()
{
    QGroupBox *group = new QGroupBox("Battery");
    group->setStyleSheet(R"(
        QGroupBox {
            background-color: #1E1E1E;
            color: white;
            border: 2px solid #444;
            border-radius: 10px;
            margin-top: 10px;
            font-family: 'Kode Mono', monospace;
            font-size: 14px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(group);
    batteryBar = new QProgressBar();
    batteryBar->setRange(0, 100);
    batteryBar->setValue(100);
    batteryBar->setStyleSheet(R"(
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

    batteryPercentLabel = new QLabel("100%");
    batteryPercentLabel->setStyleSheet("color: #FFD700; font-size: 16px; font-family: 'Kode Mono';");

    layout->addWidget(batteryBar);
    layout->addWidget(batteryPercentLabel);
    return group;
}

QGroupBox* MainWindow::createSimulationInfoGroup()
{
    QGroupBox *group = new QGroupBox("Simulation Info");
    group->setStyleSheet(R"(
        QGroupBox {
            background-color: #1E1E1E;
            color: white;
            border: 2px solid #444;
            border-radius: 10px;
            margin-top: 10px;
            font-family: 'Kode Mono', monospace;
            font-size: 14px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(group);
    timeLabel = new QLabel("Simulated Time: ");
    weatherLabel = new QLabel("Weather: ");

    timeLabel->setStyleSheet("color: #FFD700; font-size: 14px; font-family: 'Kode Mono';");
    weatherLabel->setStyleSheet("color: #FFD700; font-size: 14px; font-family: 'Kode Mono';");

    layout->addWidget(timeLabel);
    layout->addWidget(weatherLabel);
    return group;
}

void MainWindow::setupPowerChart()
{
    powerSeries = new QLineSeries();
    QPen orangePen(QColor("#FFA500"));
    orangePen.setWidth(5);
    powerSeries->setPen(orangePen);

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
