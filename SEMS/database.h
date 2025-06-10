#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

class Database : public QObject {
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    bool init();
    void logSolarData(double energy);
    void logTemperature(double temp);
    bool checkLogin(const QString &username, const QString &password);
    void logSimulationData(double time, double solar, double load, double net, double battery, const QString &weather);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
