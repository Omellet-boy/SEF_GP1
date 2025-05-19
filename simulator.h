#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QTimer>

class Simulator : public QObject {
    Q_OBJECT

public:
    explicit Simulator(QObject *parent = nullptr);

    double solarEnergy();
    double batteryLevel();
    double temperature();
    double applianceLoad();
    double getDegradation() const;

    double getSimulatedTime() const;     // Simulated hour (0–24)
    QString getWeather() const;          // Weather state

signals:
    void dataUpdated();

private:
    double m_batteryLevel;
    double m_degradation;
    double m_temperature;
    double m_simulatedTime;   // 0 to 24
    QString m_weather;

    QTimer m_timer;

    void updateDegradation();
    void updateSimulatedTime();
    void updateWeather();
    double computeSolarOutput();  // Based on time + weather

    double randomDouble(double min, double max) const;
};

#endif // SIMULATOR_H
