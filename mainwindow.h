#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressBar>
#include "simulator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDashboard();

private:
    Simulator *simulator;

    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // Solar Panel Group
    QGroupBox *createSolarPanelGroup();
    QLabel *solarPowerLabel;
    QLabel *loadLabel;
    QLabel *netLabel;

    // Battery Group
    QGroupBox *createBatteryGroup();
    QProgressBar *batteryBar;
    QLabel *batteryPercentLabel;

    // Simulation Info Group
    QGroupBox *createSimulationInfoGroup();
    QLabel *timeLabel;
    QLabel *weatherLabel;

    // Chart Components
    QChartView *powerChartView;
    QLineSeries *powerSeries;
    QChart *powerChart;
    int chartX;

    void setupPowerChart();

    QString currentDay;
    void resetGraphForNewDay();
};

#endif // MAINWINDOW_H
