#include "mainwindow.h"
#include <QtCharts/QCategoryAxis>
#include <QTimer>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), simulator(new Simulator(this))
{
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(createSolarPanelGroup());
    mainLayout->addWidget(createBatteryGroup());
    mainLayout->addWidget(createSimulationInfoGroup());

    setupPowerChart();
    mainLayout->addWidget(powerChartView);

    setCentralWidget(centralWidget);
    resize(800, 600);
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

    // Use simulated time in minutes as X value
    double simTimeMinutes = simulator->getSimulatedTime() * 60.0;
    powerSeries->append(simTimeMinutes, net);

    // Remove old points beyond 24 hours (1440 minutes) window
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

    // Use QCategoryAxis for readable time labels
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
    axisY->setTickInterval(250);  // <- Force ticks every 250 units
    axisY->setLabelFormat("%d");  // <- Makes labels integers
    powerChart->addAxis(axisY, Qt::AlignLeft);
    powerSeries->attachAxis(axisY);


    // 🔽 Add zero line
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
