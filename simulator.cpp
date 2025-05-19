#include "simulator.h"
#include <QRandomGenerator>

Simulator::Simulator(QObject *parent)
    : QObject(parent),
    m_batteryLevel(100.0),
    m_degradation(100.0),
    m_temperature(25.0)
{
    connect(&m_timer, &QTimer::timeout, this, [=]() {
        double solar = solarEnergy();
        double load = applianceLoad();

        // Net power effect: positive means charge, negative means drain
        double netEnergy = solar - load;

        // Apply a scaled battery level change (realistic and slow)
        m_batteryLevel += netEnergy * 0.0001;

        // Clamp battery level between 0 and 100
        if (m_batteryLevel > 100.0) m_batteryLevel = 100.0;
        if (m_batteryLevel < 0.0) m_batteryLevel = 0.0;

        // Smooth temperature variation
        m_temperature = m_temperature + randomDouble(-0.3, 0.6);
        if (m_temperature < 20.0) m_temperature = 20.0;

        updateDegradation();
        emit dataUpdated();
    });

    m_timer.start(5000); // update every 5 seconds
}

double Simulator::solarEnergy() {
    return randomDouble(100.0, 500.0);
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
}

double Simulator::getDegradation() const {
    return m_degradation;
}

double Simulator::randomDouble(double min, double max) const {
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

//https://chatgpt.com/share/682ac3b2-937c-8002-aa08-340d5d937d43
