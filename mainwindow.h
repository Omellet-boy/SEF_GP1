// mainwindow.h
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
#include <QPushButton>
#include <QStackedWidget>
#include "simulator.h"
#include "database.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setDatabase(Database *db);

private slots:
    void updateDashboard();

private:
    Simulator *simulator;

    //database
    Database *database = nullptr;

    // Nav bar
    QWidget *navWidget;
    QVBoxLayout *navLayout;
    QPushButton *dashboardButton;
    QPushButton *settingsButton;
    QPushButton *batteryMonitoringButton;
    QPushButton *solarMonitoringButton;
    QPushButton *dataManagementButton;

    // Pages
    QWidget *dashboardPage;
    QStackedWidget *stackedWidget;

    // Central widget
    QWidget *centralWidget;

    // Dashboard content
    QGroupBox *createSolarPanelGroup();
    QLabel *solarPowerLabel;
    QLabel *loadLabel;
    QLabel *netLabel;

    QGroupBox *createBatteryGroup();
    QProgressBar *batteryBar;
    QLabel *batteryPercentLabel;

    QGroupBox *createSimulationInfoGroup();
    QLabel *timeLabel;
    QLabel *weatherLabel;

    QChartView *powerChartView;
    QLineSeries *powerSeries;
    QChart *powerChart;
    int chartX;

    void setupPowerChart();
    QString currentDay;
    void resetGraphForNewDay();
};

#endif // MAINWINDOW_H
