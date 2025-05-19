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

signals:
    void dataUpdated();

private:
    double m_batteryLevel;
    double m_degradation;
    double m_temperature;
    QTimer m_timer;

    void updateDegradation();

    double randomDouble(double min, double max) const;

};

#endif // SIMULATOR_H
