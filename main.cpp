#include "timer.h"
// #include "pomodoro_timer_db.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // QDir().mkpath(dataPath); // Cria o diretório, se necessário
    // QString dbPath = dataPath + "/pomodoro_sessions.db";


    // if (!pomodoro_timer_db::initializeDatabase("pomodoro_sessions.db")) {
    //     qDebug() << "Failed to initialize database.";
    //     return -1;
    // }

    MainWindow w;
    w.show();
    return a.exec();
}
