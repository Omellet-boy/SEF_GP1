#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QTimer>

class Simulator : public QObject
{
    Q_OBJECT

public:
    explicit Simulator(QObject *parent = nullptr);

    double solarEnergy();
    double batteryLevel();
    double applianceLoad();
    double getDegradation() const;
    double getSimulatedTime() const;
    QString getWeather() const;
    double getTemperature() const;


    void setSolarChargingEnabled(bool enabled);
    bool isSolarChargingEnabled() const;

    double m_temperature; // Made public for UI access (or use getter if preferred)

signals:
    void dataUpdated();

private:
    double m_batteryLevel;
    double m_degradation;
    double m_simulatedTime;
    QString m_weather;
    double m_rainDuration;
    double m_dryCooldown;
    double m_sunDuration;
    bool m_solarChargingEnabled;

    QTimer m_timer;

    void updateSimulatedTime();
    void updateWeather();
    void updateDegradation();
    double computeSolarOutput();
    double randomDouble(double min, double max) const;
};

#endif // SIMULATOR_H
