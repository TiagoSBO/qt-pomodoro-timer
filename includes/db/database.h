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
    static bool saveSetting(int pomodoroDuration, int shortBreak, int longBreak, int sessionsBeforeLong);
    static bool loadSettings(int &pomodoroDuration, int &shortBreak, int &longBreak, int &sessionsBeforeLong);
};

#endif // DATABASE_H
