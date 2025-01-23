#ifndef POMODORO_TIMER_DB_H
#define POMODORO_TIMER_DB_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>


class pomodoro_timer_db
{
public:
    pomodoro_timer_db();

    static bool initializeDatabase(const QString& dbName);

};

#endif // POMODORO_TIMER_DB_H
