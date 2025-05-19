
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProgressBar>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *solarLabel = new QLabel("Solar Energy (W):", this);
    QLabel *batteryLabel = new QLabel("Battery %:", this);
    QLabel *tempLabel = new QLabel("Temperature (°C):", this);
    QLabel *degradationLabel = new QLabel("Battery Health (%):", this);

    QLCDNumber *solarDisplay = new QLCDNumber(this);
    QProgressBar *batteryBar = new QProgressBar(this);
    QLCDNumber *tempDisplay = new QLCDNumber(this);
    QProgressBar *degBar = new QProgressBar(this);

    QLabel *timeTitleLabel = new QLabel("Simulated Time:", this);
    QLabel *timeLabel = new QLabel(this);
    QLabel *weatherTitleLabel = new QLabel("Weather:", this);
    QLabel *weatherLabel = new QLabel(this);

    // Styling time and weather labels
    timeLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    weatherLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    layout->addWidget(solarLabel);
    layout->addWidget(solarDisplay);
    layout->addWidget(batteryLabel);
    layout->addWidget(batteryBar);
    layout->addWidget(tempLabel);
    layout->addWidget(tempDisplay);
    layout->addWidget(degradationLabel);
    layout->addWidget(degBar);
    layout->addWidget(timeTitleLabel);
    layout->addWidget(timeLabel);
    layout->addWidget(weatherTitleLabel);
    layout->addWidget(weatherLabel);

    setCentralWidget(central);

    connect(&simulator, &Simulator::dataUpdated, this, [=]() {
        solarDisplay->display(simulator.solarEnergy());
        batteryBar->setValue(simulator.batteryLevel());
        tempDisplay->display(simulator.temperature());
        degBar->setValue(simulator.getDegradation());

        // Format simulated time as HH:MM
        double hour = simulator.getSimulatedTime();
        int hh = static_cast<int>(hour);
        int mm = static_cast<int>((hour - hh) * 60);
        QString timeStr = QString("%1:%2")
                              .arg(hh, 2, 10, QChar('0'))
                              .arg(mm, 2, 10, QChar('0'));
        timeLabel->setText(timeStr);

        // Add weather icons
        QString weather = simulator.getWeather();
        QString icon;
        if (weather == "Sunny") icon = "☀️";
        else if (weather == "Cloudy") icon = "🌥️";
        else if (weather == "Rainy") icon = "🌧️";
        else if (weather == "Stormy") icon = "🌩️";
        else if (weather == "Snowy") icon = "❄️";
        else icon = "🌡️";

        weatherLabel->setText(icon + " " + weather);
    });
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateUI() {
    // Already handled in lambda
}
