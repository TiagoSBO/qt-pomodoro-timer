#include "sessionlogs.h"
#include <QTableWidgetItem>
#include <QDebug>

Sessionlogs::Sessionlogs(QTableWidget *table)
    : m_table(table)
{}

Sessionlogs::~Sessionlogs() {}

static int sessionCount = 0;

void Sessionlogs::addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession)
{
    if (!m_table) return;

    // If it's every 4 sessions, add a header before inserting the new session
    if (sessionCount % 4 == 0 && sessionCount != 0) {
        int rowHeader = m_table->rowCount();
        m_table->insertRow(rowHeader);

        QTableWidgetItem *groupHeader = new QTableWidgetItem("🕒 Cycle: " + QString::number(sessionCount / 4 + 1));
        groupHeader->setTextAlignment(Qt::AlignCenter);
        groupHeader->setFlags(Qt::NoItemFlags);
        groupHeader->setFont(QFont("Arial", 10, QFont::Bold));

        m_table->setSpan(rowHeader, 0, 1, 3);
        m_table->setItem(rowHeader, 0, groupHeader);
    }

    int row = m_table->rowCount();
    m_table->insertRow(row);

    QTableWidgetItem *itemSession = new QTableWidgetItem(QString::number(sessionNumber));
    itemSession->setTextAlignment(Qt::AlignCenter);
    itemSession->setFlags(itemSession->flags() & ~Qt::ItemIsEditable);

    QTableWidgetItem *itemDuration = new QTableWidgetItem(sessionDuration);
    itemDuration->setTextAlignment(Qt::AlignCenter);
    itemDuration->setFlags(itemDuration->flags() & ~Qt::ItemIsEditable);

    QTableWidgetItem *itemEndTime = new QTableWidgetItem(endTimeOfSession);
    itemEndTime->setTextAlignment(Qt::AlignCenter);
    itemEndTime->setFlags(itemEndTime->flags() & ~Qt::ItemIsEditable);

    m_table->setItem(row, 0, itemSession);
    m_table->setItem(row, 1, itemDuration);
    m_table->setItem(row, 2, itemEndTime);

    // Updates accumulated time
    QStringList timeParts = sessionDuration.split(":");
    if (timeParts.size() == 2) {
        int minutes = timeParts[0].toInt();
        int seconds = timeParts[1].toInt();
        m_totalAccumulatedFocusSeconds += (minutes * 60) + seconds;
    }

    sessionCount++;

    qDebug() << "Session successfully added to table!";
}

int Sessionlogs::getTotalTimeFocus()
{
    if (!m_table) return 0;

    int totalSeconds = 0;

    for (int row = 0; row < m_table->rowCount(); ++row) {
        QTableWidgetItem *item = m_table->item(row, 1); // Collum "Focus Time"
        if (item) {
            QString timeStr = item->text(); // format "MM:SS"
            qDebug() << "Line " << row << " - Time extracted: " << timeStr;
            QStringList timeParts = timeStr.split(":");

            if (timeParts.size() == 2) {
                int minutes = timeParts[0].toInt();
                int seconds = timeParts[1].toInt();
                totalSeconds += (minutes * 60) + seconds;
                m_totalAccumulatedFocusSeconds = totalSeconds;
            }
        }
    }
    qDebug() << "Total accumulated focus time: " << totalSeconds << " seconds";

    return totalSeconds;
}

void Sessionlogs::clearTableOnly()
{
    if (!m_table) return;

    m_table->clearContents();
    m_table->setRowCount(0);

    qDebug() << "Table contents cleared (time preserved).";
}


int Sessionlogs::getAccumulatedFocusSeconds() const
{
    return m_totalAccumulatedFocusSeconds;
}

void Sessionlogs::clearTotalFocusOnly()
{
    if (!m_table) return;
    m_totalAccumulatedFocusSeconds = 0;
}


