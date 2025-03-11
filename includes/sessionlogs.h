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

private:
    QTableWidget *m_table;  // Ponteiro para a tabela
};

#endif // SESSIONLOGS_H
