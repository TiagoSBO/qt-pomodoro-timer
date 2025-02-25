#ifndef SESSIONLOGS_H
#define SESSIONLOGS_H

#include <QTableWidget>

class MainWindow; // Declaração antecipada para evitar dependência circular

class Sessionlogs
{
public:
    explicit Sessionlogs(MainWindow *mainWin);  // Construtor recebe um ponteiro para MainWindow
    ~Sessionlogs();

    void addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession);

private:
    MainWindow *m_mainWin; // Guarda o ponteiro da MainWindow
};

#endif // SESSIONLOGS_H
