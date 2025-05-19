
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
