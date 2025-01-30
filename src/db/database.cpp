#include "database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <Qdir>

database::database() {}


bool database::initializeDatabase(const QString& dbName){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Adiciona driver do SQLite
    QString dbPath = QDir::currentPath() + "/pomodoroDb.sqlite"; // Caminho do banco

    db.setDatabaseName(dbPath);
    qDebug() << "Tentando abrir banco de dados em:" << dbPath;

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco de dados:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    // Criando tabela settings
    QString createSettingsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS settings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            pomodoro_duration INTEGER NOT NULL,
            short_break_duration INTEGER NOT NULL,
            long_break_duration INTEGER NOT NULL,
            sessions_before_long INTEGER NOT NULL
        )
    )";

    if (!query.exec(createSettingsTableSQL)) {
        qDebug() << "Erro ao criar a tabela settings:" << query.lastError().text();
        return false;
    }

    // Criando tabela sessions
    QString createSessionsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            start_time TEXT NOT NULL,
            end_time TEXT NOT NULL,
            type TEXT NOT NULL,
            duration INTEGER NOT NULL
        )
    )";

    if (!query.exec(createSessionsTableSQL)) {
        qDebug() << "Erro ao criar a tabela sessions:" << query.lastError().text();
        return false;
    }

    return true;
}

bool database::saveSetting(int pomodoroDuration, int shortBreak, int longBreak, int sessionsBeforeLong)
{
    QSqlQuery query;

    // Primeiro, verifica se já existe uma configuração
    query.exec("SELECT id FROM settings LIMIT 1");
    bool exists = query.next(); // Se houver resultado, já existe configuração

    if (exists) {
        query.prepare("UPDATE settings SET pomodoro_duration = ?, short_break_duration = ?, long_break_duration = ?, sessions_before_long = ? WHERE id = 1");
    } else {
        query.prepare("INSERT INTO settings (pomodoro_duration, short_break_duration, long_break_duration, sessions_before_long) VALUES (?, ?, ?, ?)");
    }

    query.addBindValue(pomodoroDuration);
    query.addBindValue(shortBreak);
    query.addBindValue(longBreak);
    query.addBindValue(sessionsBeforeLong);

    if (!query.exec()) {
        qDebug() << "Erro ao salvar configurações:" << query.lastError().text();
        return false;
    }

    return true;
}


bool database::loadSettings(int &pomodoroDuration, int &shortBreak, int &longBreak, int &sessionsBeforeLong)
{
    QSqlQuery query;
    query.prepare("SELECT pomodoro_duration, short_break_duration, long_break_duration, sessions_before_long FROM settings LIMIT 1");

    if (!query.exec()) {
        qDebug() << "Erro ao carregar configurações:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        pomodoroDuration = query.value(0).toInt();
        shortBreak = query.value(1).toInt();
        longBreak = query.value(2).toInt();
        sessionsBeforeLong = query.value(3).toInt();
        return true;
    }

    return false; // Nenhuma configuração encontrada
}


//C:/Users/Tiago/Documents/QtCreator_Projects/pomodoro_timer/db/pomodoro_sessions.db
