#include "timer.h"
#include "database.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Inicializa o banco de dados
    if (!database::initializeDatabase("pomodoroDb.sqlite")) {
        qDebug() << "Erro ao conectar ao banco de dados!";
        return -1;  // Sai do programa se o banco falhar
    }

    // Carregar configurações do banco
    int pomodoroDuration, shortBreak, longBreak, sessionsInterval;
    if (database::loadSettings(pomodoroDuration, shortBreak, longBreak, sessionsInterval)) {
        qDebug() << "Configurações carregadas do banco:";
        qDebug() << "Pomodoro:" << pomodoroDuration
                 << "Curta Pausa:" << shortBreak
                 << "Longa Pausa:" << longBreak
                 << "Sessões Antes da Longa Pausa:" << sessionsInterval;
    } else {
        qDebug() << "Usando valores padrão.";
        pomodoroDuration = 25;
        shortBreak = 5;
        longBreak = 15;
        sessionsInterval = 4;
    }

    MainWindow w;
    w.setTimerDefaults(pomodoroDuration, shortBreak, longBreak, sessionsBeforeLong);
    w.show();

    return a.exec();
}
