#include "simulator.h"
#include <QRandomGenerator>
#include <cmath>  // for std::sin, std::max

Simulator::Simulator(QObject *parent)
    : QObject(parent),
    m_batteryLevel(75.0),
    m_degradation(100.0),
    m_temperature(25.0),
    m_simulatedTime(6.0), // Start at 6 AM
    m_weather("Clear"),
    m_rainDuration(0.0),
    m_dryCooldown(0.0)
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

    m_timer.start(1000); // update every 5 seconds
}

double Simulator::solarEnergy() {
    return computeSolarOutput();
}

double Simulator::batteryLevel() {
    return m_batteryLevel;
}

double Simulator::applianceLoad() {
    double hour = m_simulatedTime;

    // Very low base load at night (midnight to 6 AM)
    if (hour < 6.0) {
        return randomDouble(20.0, 50.0);
    }
    // Morning (6 AM to 9 AM) gentle increase
    else if (hour < 9.0) {
        return 50.0 + (hour - 6.0) * 30.0 + randomDouble(-10.0, 10.0);
    }
    // Daytime (9 AM to 17 PM) moderate steady load
    else if (hour < 17.0) {
        return 150.0 + randomDouble(-20.0, 20.0);
    }
    // Evening peak (17 PM to 22 PM)
    else if (hour < 22.0) {
        return 300.0 + (hour - 17.0) * 40.0 + randomDouble(-20.0, 20.0);
    }
    // Late night (22 PM to midnight) winding down
    else {
        return 100.0 + (24.0 - hour) * 30.0 + randomDouble(-10.0, 10.0);
    }
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
    m_simulatedTime += 0.1667; // Each tick = 15 minutes
    if (m_simulatedTime >= 24.0)
        m_simulatedTime = 0.0;
}

void Simulator::updateWeather() {
    if (m_rainDuration > 0) {
        m_rainDuration -= 0.0833 * 60;
        if (m_rainDuration <= 0) {
            m_rainDuration = 0;
            m_dryCooldown = randomDouble(30.0, 90.0);
        } else {
            m_weather = "Rainy";
            return;
        }
    }

    if (m_dryCooldown > 0) {
        m_dryCooldown -= 0.0833 * 60;
        m_weather = randomDouble(0.0, 1.0) < 0.9 ? "Clear" : "Cloudy";
        return;
    }

    double hour = m_simulatedTime;

    // Set rain chance based on time windows
    double rainChance = 0.0;
    if (hour >= 6.0 && hour < 11.0) {
        rainChance = 0.2;  // Moderate rain chance morning
    } else if (hour >= 11.0 && hour < 17.0) {
        rainChance = 0.05; // Low rain chance midday
    } else if (hour >= 17.0 && hour < 22.0) {
        rainChance = 0.3;  // Moderate rain chance evening
    } else if ((hour >= 0.0 && hour < 3.0)) {
        rainChance = 0.6;  // High rain chance at night
    } else {
        rainChance = 0.1;  // Low chance other times
    }

    // Allow sunny only from 12 to 17 (noon to 5pm)
    bool canBeSunny = (hour >= 12.0 && hour <= 17.0);

    double roll = randomDouble(0.0, 1.0);

    if (roll < rainChance) {
        m_weather = "Rainy";
        m_rainDuration = randomDouble(30.0, 120.0);
    } else if (canBeSunny && roll < rainChance + 0.15) { // 15% sunny chance during allowed time
        m_weather = "Sunny";
        m_sunDuration = randomDouble(60.0, 120.0);
    } else if (roll < 0.85) {
        m_weather = "Clear";
    } else {
        m_weather = "Cloudy";
    }
}


double Simulator::computeSolarOutput() {
    double hour = m_simulatedTime;

    if (hour < 6.0 || hour > 19.0)
        return 0.0; // Night

    double peakPower = 1000.0; // Max solar panel output in watts

    // Normalized hour between 0 and 1 for day time
    double normalized = (hour - 6.0) / (19.0 - 6.0);

    // Parabolic solar irradiance curve peaking at noon
    double solarPower = peakPower * 4 * normalized * (1 - normalized);

    // Weather impact
    if (m_weather == "Cloudy") solarPower *= 0.6;
    else if (m_weather == "Rainy") solarPower *= 0.2;
    else if (m_weather == "Sunny") solarPower *= 1.2;

    // Add small noise +/- 50 W
    solarPower += randomDouble(-50.0, 50.0);
    if (solarPower < 0) solarPower = 0;

    return solarPower;
}

double Simulator::getSimulatedTime() const {
    return m_simulatedTime;
}

QString Simulator::getWeather() const {
    return m_weather;
}
