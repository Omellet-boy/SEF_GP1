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
    QString getDay() const;              // Returns current day name, e.g. "Monday"

signals:
    void dataUpdated();

private:
    double m_batteryLevel;
    double m_degradation;
    double m_temperature;
    double m_simulatedTime;   // 0 to 24
    int m_dayIndex;           // 0 = Monday, 1 = Tuesday, ..., 6 = Sunday
    QString m_weather;
    double m_rainDuration;
    double m_dryCooldown;
    double m_sunDuration;
    double m_sunCooldown;

    QTimer m_timer;

    void updateDegradation();
    void updateSimulatedTime();
    void updateWeather();
    double computeSolarOutput();  // Based on time + weather
    double computeApplienceLoad();

    double randomDouble(double min, double max) const;
    void advanceDay();
    QString dayNameFromIndex(int index) const;
};

#endif // SIMULATOR_H
