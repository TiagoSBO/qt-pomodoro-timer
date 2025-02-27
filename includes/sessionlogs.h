#ifndef SESSIONLOGS_H
#define SESSIONLOGS_H

#include <QObject>
#include <QTableWidget>

class Sessionlogs
{
public:
    explicit Sessionlogs(QTableWidget *table);  // Agora passa o ponteiro para QTableWidget diretamente
    ~Sessionlogs();

    void addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession);


private:
    QTableWidget *m_table;  // Ponteiro para a tabela
};

#endif // SESSIONLOGS_H
