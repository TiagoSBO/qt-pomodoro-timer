#ifndef SESSIONLOGS_H
#define SESSIONLOGS_H

#include <QObject>
#include <QTableWidget>

class Sessionlogs
{
public:
    explicit Sessionlogs(QTableWidget *table);
    ~Sessionlogs();

    void addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession);
    int getTotalTimeFocus();

    void clearTableOnly();
    int getAccumulatedFocusSeconds() const;

    void clearTotalFocusOnly();


private:
    QTableWidget *m_table;

    int m_totalAccumulatedFocusSeconds = 0;
};

#endif
