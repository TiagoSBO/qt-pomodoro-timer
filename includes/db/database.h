#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class database
{
public:
    database();

    static bool initializeDatabase(const QString& dbName);
};


#endif // POMODORO_TIMER_DB_H
