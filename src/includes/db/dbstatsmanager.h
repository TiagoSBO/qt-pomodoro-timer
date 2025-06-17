#ifndef DBSTATSMANAGER_H
#define DBSTATSMANAGER_H

#include "focussession.h"
#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QString>

class DbStatsManager
{
public:
    DbStatsManager();

    static DbStatsManager& instance();
    QSqlDatabase& database();

    bool initDatabase(const QString& path = "dbPomodoro_stats.db");
    bool insertSession(const FocusSession &session);
    bool saveFocusSession(const QDateTime &start, const QDateTime &end, int &durationMinutes);
    bool clearAllSessions();


private:
    ~DbStatsManager();
    DbStatsManager(const DbStatsManager&) = delete;
    DbStatsManager& operator=(const DbStatsManager&) = delete;

    QSqlDatabase m_db;
};

#endif // DBSTATSMANAGER_H
