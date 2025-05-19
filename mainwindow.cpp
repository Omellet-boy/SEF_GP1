
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

    layout->addWidget(solarLabel);
    layout->addWidget(solarDisplay);
    layout->addWidget(batteryLabel);
    layout->addWidget(batteryBar);
    layout->addWidget(tempLabel);
    layout->addWidget(tempDisplay);
    layout->addWidget(degradationLabel);
    layout->addWidget(degBar);

    setCentralWidget(central);

    connect(&simulator, &Simulator::dataUpdated, this, [=]() {
        solarDisplay->display(simulator.solarEnergy());
        batteryBar->setValue(simulator.batteryLevel());
        tempDisplay->display(simulator.temperature());
        degBar->setValue(simulator.getDegradation());
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateUI() {
    // Already handled in lambda
}
