#include "simulator.h"
#include <QRandomGenerator>

Simulator::Simulator(QObject *parent)
    : QObject(parent),
    m_batteryLevel(100.0),
    m_degradation(100.0),
    m_temperature(25.0),
    m_simulatedTime(6.0), // Start at 6 AM
    m_weather("Clear")
{
    connect(&m_timer, &QTimer::timeout, this, [=]() {
        updateSimulatedTime();
        updateWeather();

        double solar = computeSolarOutput();
        double load = applianceLoad();

        double netEnergy = solar - load;
        m_batteryLevel += netEnergy * 0.0001;

        if (m_batteryLevel > 100.0) m_batteryLevel = 100.0;
        if (m_batteryLevel < 0.0) m_batteryLevel = 0.0;

        m_temperature += randomDouble(-0.3, 0.6);
        if (m_temperature < 20.0) m_temperature = 20.0;

        updateDegradation();
        emit dataUpdated();
    });

    m_timer.start(5000); // update every 5 seconds
}

double Simulator::solarEnergy() {
    return computeSolarOutput();
}

double Simulator::batteryLevel() {
    return m_batteryLevel;
}

double Simulator::temperature() {
    return m_temperature;
}

double Simulator::applianceLoad() {
    return randomDouble(100.0, 500.0);
}

void Simulator::updateDegradation() {
    m_degradation -= randomDouble(0.001, 0.01);
    if (m_degradation < 0.0) m_degradation = 0.0;
}

double Simulator::getDegradation() const {
    return m_degradation;
}

double Simulator::randomDouble(double min, double max) const {
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

void Simulator::updateSimulatedTime() {
    m_simulatedTime += 0.25; // Each tick = 15 minutes
    if (m_simulatedTime >= 24.0)
        m_simulatedTime = 0.0;
}

void Simulator::updateWeather() {
    double roll = randomDouble(0.0, 1.0);
    if (roll < 0.7)
        m_weather = "Clear";
    else if (roll < 0.9)
        m_weather = "Cloudy";
    else
        m_weather = "Rainy";
}

double Simulator::computeSolarOutput() {
    // Simulate solar based on time (day: 6-18)
    if (m_simulatedTime < 6.0 || m_simulatedTime > 18.0)
        return 0.0; // Night

    double base = randomDouble(300.0, 500.0); // midday range

    // Reduce output based on weather
    if (m_weather == "Cloudy") base *= 0.5;
    else if (m_weather == "Rainy") base *= 0.2;

    // Reduce based on hour from solar peak (12.0 noon)
    double hourDiff = std::abs(m_simulatedTime - 12.0);
    double efficiency = 1.0 - (hourDiff / 6.0); // peak at noon
    return base * efficiency;
}

double Simulator::getSimulatedTime() const {
    return m_simulatedTime;
}

QString Simulator::getWeather() const {
    return m_weather;
}
