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
    bool checkLogin(const QString &username, const QString &password);
    QString getEmail(const QString &username);
    bool updateEmail(const QString &username, const QString &email);
    void logSolarData(double energy);
    void logTemperature(double temp);
    void logSimulationData(double time, double solar, double load, double net, double battery, const QString &weather);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
