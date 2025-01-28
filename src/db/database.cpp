#include "database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <Qdir>

database::database() {}

bool database::initializeDatabase(const QString& dbName){

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/src/db/pomodoroDb");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database!" << db.lastError().text();
        return false;
    }
    QSqlQuery query;
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            currentDate TEXT NOT NULL,
            duration INTEGER NOT NULL,
            type TEXT NOT NULL
        )
    )";

    if (!query.exec(createTableSQL)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return false;
    }
    return true;
}

//C:/Users/Tiago/Documents/QtCreator_Projects/pomodoro_timer/db/pomodoro_sessions.db
