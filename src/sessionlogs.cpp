#include "sessionlogs.h"
#include <QTableWidgetItem>
#include <QDebug>

Sessionlogs::Sessionlogs(QTableWidget *table)
    : m_table(table)  // Inicializa com o ponteiro da tabela
{}

Sessionlogs::~Sessionlogs() {}

static int sessionCount = 0;

void Sessionlogs::addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession)
{
    if (!m_table) {
        qDebug() << "Erro: QTableWidget não inicializado!";
        return;
    }

    // 🔹 Se for a cada 4 sessões, adiciona um cabeçalho antes de inserir a nova sessão
    if (sessionCount % 4 == 0 && sessionCount != 0) {
        int rowHeader = m_table->rowCount();
        m_table->insertRow(rowHeader);

        QTableWidgetItem *groupHeader = new QTableWidgetItem("🕒 Ciclo " + QString::number(sessionCount / 4 + 1));
        groupHeader->setTextAlignment(Qt::AlignCenter);
        groupHeader->setFlags(Qt::NoItemFlags);
        groupHeader->setFont(QFont("Arial", 10, QFont::Bold));

        m_table->setSpan(rowHeader, 0, 1, 3);  // Mescla as colunas para criar a separação
        m_table->setItem(rowHeader, 0, groupHeader);
    }

    int row = m_table->rowCount();  // Obtém a nova posição correta
    m_table->insertRow(row);

    QTableWidgetItem *itemSession = new QTableWidgetItem(QString::number(sessionNumber));
    itemSession->setTextAlignment(Qt::AlignCenter);
    itemSession->setFlags(itemSession->flags() & ~Qt::ItemIsEditable); // ❌ Remove a capacidade de edição

    QTableWidgetItem *itemDuration = new QTableWidgetItem(sessionDuration);
    itemDuration->setTextAlignment(Qt::AlignCenter);
    itemDuration->setFlags(itemDuration->flags() & ~Qt::ItemIsEditable); // ❌ Remove a capacidade de edição

    QTableWidgetItem *itemEndTime = new QTableWidgetItem(endTimeOfSession);
    itemEndTime->setTextAlignment(Qt::AlignCenter);
    itemEndTime->setFlags(itemEndTime->flags() & ~Qt::ItemIsEditable); // ❌ Remove a capacidade de edição

    m_table->setItem(row, 0, itemSession);
    m_table->setItem(row, 1, itemDuration);
    m_table->setItem(row, 2, itemEndTime);

    sessionCount++;

    qDebug() << "Sessão adicionada com sucesso na tabela!";
}
