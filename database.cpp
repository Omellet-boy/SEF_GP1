#include "database.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent) {}

bool Database::init() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("energy_data.db");
    }

    if (!db.isOpen() && !db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    // Ensure required tables exist
    query.exec("CREATE TABLE IF NOT EXISTS SolarLog (timestamp TEXT, energy REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS TempLog (timestamp TEXT, temperature REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS Users (username TEXT PRIMARY KEY, password TEXT, email TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS SimulationLog ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "sim_time_minutes REAL, "
               "timestamp TEXT, "
               "solar_output REAL, "
               "load REAL, "
               "net_power REAL, "
               "battery_level REAL, "
               "weather TEXT)");

    // Alter Users table to add 'email' column if it doesn't exist
    query.exec("PRAGMA table_info(Users)");
    bool emailExists = false;
    while (query.next()) {
        if (query.value(1).toString() == "email") {
            emailExists = true;
            break;
        }
    }
    if (!emailExists) {
        query.exec("ALTER TABLE Users ADD COLUMN email TEXT");
    }

    return true;
}

bool Database::checkLogin(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

QString Database::getEmail(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT email FROM Users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

bool Database::updateEmail(const QString &username, const QString &email) {
    QSqlQuery query;
    query.prepare("UPDATE Users SET email = ? WHERE username = ?");
    query.addBindValue(email);
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Failed to update email:" << query.lastError().text();
        return false;
    }
    return true;
}

void Database::logSolarData(double energy) {
    QSqlQuery query;
    query.prepare("INSERT INTO SolarLog (timestamp, energy) VALUES (?, ?)");
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    query.addBindValue(energy);
    query.exec();
}

void Database::logTemperature(double temp) {
    QSqlQuery query;
    query.prepare("INSERT INTO TempLog (timestamp, temperature) VALUES (?, ?)");
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    query.addBindValue(temp);
    query.exec();
}

void Database::logSimulationData(double time, double solar, double load, double net, double battery, const QString &weather) {
    QSqlQuery query;
    query.prepare("INSERT INTO SimulationLog (sim_time_minutes, timestamp, solar_output, load, net_power, battery_level, weather) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(time);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    query.addBindValue(solar);
    query.addBindValue(load);
    query.addBindValue(net);
    query.addBindValue(battery);
    query.addBindValue(weather);
    if (!query.exec()) {
        qDebug() << "Failed to insert simulation log:" << query.lastError().text();
    }
}
