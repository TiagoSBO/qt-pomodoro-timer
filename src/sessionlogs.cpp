#include "sessionlogs.h"
#include "timer.h"
#include <QDebug>

Sessionlogs::Sessionlogs(MainWindow *mainWin) {
    m_mainWin = mainWin; // Armazena o ponteiro da MainWindow
}

Sessionlogs::~Sessionlogs() {}

void Sessionlogs::addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession)
{
    if (!m_mainWin) {
        qDebug() << "Erro: MainWindow não inicializada!";
        return;
    }


    // Chama a função da MainWindow que adiciona os dados na tabela
    m_mainWin->addSessionToTable(sessionNumber, sessionDuration, endTimeOfSession);
}
