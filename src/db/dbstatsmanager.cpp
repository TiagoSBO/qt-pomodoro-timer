#include "dbstatsmanager.h"
#include <QSqlError>
#include <QDebug>
#include <qsqlquery.h>
#include <QDir>
#include <QFileInfo>

DbStatsManager::DbStatsManager() {}

DbStatsManager::~DbStatsManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

DbStatsManager& DbStatsManager::instance() {
    static DbStatsManager instance;
    return instance;
}

QSqlDatabase& DbStatsManager::database() {
    return m_db;
}

bool DbStatsManager::initDatabase(const QString& path) {

    QFileInfo dbFileInfo(path);
    QDir dbDir = dbFileInfo.absoluteDir();

    if (!dbDir.exists()) {
        if (!dbDir.mkpath(".")) {
            qWarning() << "Falha ao criar diretório do banco de dados:" << dbDir.absolutePath();
            return false;
        }
    }

    if (!QFile::exists(path)) {
        QString resourcePath = ":/initial_db/db/dbPomodoro_stats.db";
        if (!QFile::copy(resourcePath, path)) {
            qWarning() << "Falha ao copiar banco de dados inicial do recurso.";
            return false;
        }

        qDebug() << "Banco de dados inicial copiado para:" << path;
    }

    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(path);
    }

    qDebug() << "Tentando abrir o banco de dados em:" << m_db.databaseName();

    if (!m_db.open()) {
        qDebug() << "Erro ao abrir o banco:" << m_db.lastError().text();
        return false;
    }

    // New collum "end time"
    QSqlQuery checkColumnQuery(m_db);
    checkColumnQuery.exec("PRAGMA table_info(focus_sessions)");
    bool hasEndTime = false;

    while (checkColumnQuery.next()) {
        if (checkColumnQuery.value(1).toString() == "end_time") {
            hasEndTime = true;
            break;
        }
    }
    if (!hasEndTime) {
        QSqlQuery alterQuery(m_db);
        alterQuery.exec("ALTER TABLE focus_sessions ADD COLUMN end_time TEXT");
    }

    qDebug() << "Banco de dados aberto com sucesso.";
    return true;
}

bool DbStatsManager::insertSession(const FocusSession &session)
{
    if (!m_db.isOpen()) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO focus_sessions (start_time, end_time, duration_minutes, created_at)
        VALUES (?, ?, ?, ?)
    )");
    query.addBindValue(session.startTime().toString(Qt::ISODate));
    query.addBindValue(session.endTime().toString(Qt::ISODate));
    query.addBindValue(session.duration());
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));


    if (!query.exec()) {
        qDebug() << "Erro ao inserir sessão:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DbStatsManager::saveFocusSession(const QDateTime &start, const QDateTime &end, int &durationMinutes)
{
    FocusSession session(start, end, durationMinutes);
    return insertSession(session);
}

bool DbStatsManager::clearAllSessions() {
    QSqlQuery query(m_db);
    if (!query.exec("DELETE FROM focus_sessions")) {
        qWarning() << "Erro ao limpar sessões:" << query.lastError().text();
        return false;
    }
    return true;
}
