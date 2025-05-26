
#include "database.h"
#include <QSqlError>
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent) {}

bool Database::init() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("energy_data.db");

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS SolarLog (timestamp TEXT, energy REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS TempLog (timestamp TEXT, temperature REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS Users (username TEXT PRIMARY KEY, password TEXT)");

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


