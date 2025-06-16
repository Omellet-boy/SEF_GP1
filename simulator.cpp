#include "simulator.h"
#include <QRandomGenerator>
#include <QProcess>
#include <QSettings>
#include "database.h"
#include <cmath>

Simulator::Simulator(QObject *parent)
    : QObject(parent),
    m_batteryLevel(75.0),
    m_degradation(100.0),
    m_temperature(25.0),
    m_simulatedTime(10.0),
    m_weather("Clear"),
    m_rainDuration(0.0),
    m_dryCooldown(0.0),
    m_sunDuration(0.0),
    m_solarChargingEnabled(true),
    m_emailSentOverheat(false),
    m_emailSentDegrade(false)
{
    connect(&m_timer, &QTimer::timeout, this, [=]() {
        updateSimulatedTime();
        updateWeather();

        double solar = computeSolarOutput();
        if (!m_solarChargingEnabled)
            solar = 0.0;

        double load = applianceLoad();
        double netEnergy = solar - load;
        m_batteryLevel += netEnergy * 0.0001;

        if (m_batteryLevel > 100.0) m_batteryLevel = 100.0;
        if (m_batteryLevel < 0.0) m_batteryLevel = 0.0;

        m_temperature += randomDouble(-0.3, 0.6);
        if (m_temperature < 20.0) m_temperature = 20.0;

        updateDegradation();
        emit dataUpdated();

        // ===== Email Notification Logic =====

        QSettings settings("MyCompany", "SEMS");

        if (settings.value("emailNotifications", false).toBool()) {
            QString user = settings.value("currentUsername", "admin").toString();
            QString email = Database().getEmail(user);

            if (!email.isEmpty()) {
                // Overheat Alert
                if (m_temperature > 45.0 && !m_emailSentOverheat) {
                    sendEmailNotification(email, "Overheating Alert",
                                          QString("Battery temperature has exceeded 45°C (current: %1°C)").arg(m_temperature));
                    m_emailSentOverheat = true;
                } else if (m_temperature < 40.0) {
                    m_emailSentOverheat = false;
                }

                // Degradation Alert
                if (m_degradation < 30.0 && !m_emailSentDegrade) {
                    sendEmailNotification(email, "Battery Health Alert",
                                          QString("Battery degradation dropped below 30%% (current: %1%%)").arg(m_degradation));
                    m_emailSentDegrade = true;
                } else if (m_degradation > 35.0) {
                    m_emailSentDegrade = false;
                }
            }
        }

    });

    m_timer.start(5000); // update every 5 seconds
}

void Simulator::sendEmailNotification(const QString &to, const QString &subject, const QString &message) {
    QProcess *process = new QProcess(this);
    QString script = "python3";
    QStringList args;
    args << "email_alert.py" << to << subject << message;
    process->startDetached(script, args);
}

double Simulator::solarEnergy() {
    return computeSolarOutput();
}

double Simulator::batteryLevel() {
    return m_batteryLevel;
}

double Simulator::applianceLoad() {
    double hour = m_simulatedTime;

    if (hour < 6.0) {
        return randomDouble(20.0, 50.0);
    } else if (hour < 9.0) {
        return 50.0 + (hour - 6.0) * 30.0 + randomDouble(-10.0, 10.0);
    } else if (hour < 17.0) {
        return 150.0 + randomDouble(-20.0, 20.0);
    } else if (hour < 22.0) {
        return 300.0 + (hour - 17.0) * 40.0 + randomDouble(-20.0, 20.0);
    } else {
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

double Simulator::getTemperature() const {
    return m_temperature;
}

double Simulator::randomDouble(double min, double max) const {
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

void Simulator::updateSimulatedTime() {
    m_simulatedTime += 0.1667; // 15 minutes per tick
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
    double rainChance = 0.0;

    if (hour >= 6.0 && hour < 11.0) {
        rainChance = 0.2;
    } else if (hour >= 11.0 && hour < 17.0) {
        rainChance = 0.05;
    } else if (hour >= 17.0 && hour < 22.0) {
        rainChance = 0.3;
    } else if ((hour >= 0.0 && hour < 3.0)) {
        rainChance = 0.6;
    } else {
        rainChance = 0.1;
    }

    bool canBeSunny = (hour >= 12.0 && hour <= 17.0);
    double roll = randomDouble(0.0, 1.0);

    if (roll < rainChance) {
        m_weather = "Rainy";
        m_rainDuration = randomDouble(30.0, 120.0);
    } else if (canBeSunny && roll < rainChance + 0.15) {
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
        return 0.0;

    double peakPower = 1000.0;
    double normalized = (hour - 6.0) / (19.0 - 6.0);
    double solarPower = peakPower * 4 * normalized * (1 - normalized);

    if (m_weather == "Cloudy") solarPower *= 0.6;
    else if (m_weather == "Rainy") solarPower *= 0.2;
    else if (m_weather == "Sunny") solarPower *= 1.2;

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

void Simulator::setSolarChargingEnabled(bool enabled) {
    m_solarChargingEnabled = enabled;
}

bool Simulator::isSolarChargingEnabled() const {
    return m_solarChargingEnabled;
}
