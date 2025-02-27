#include "sessionlogs.h"
#include <QTableWidgetItem>
#include <QDebug>

Sessionlogs::Sessionlogs(QTableWidget *table)
    : m_table(table)  // Inicializa com o ponteiro da tabela
{
}

Sessionlogs::~Sessionlogs() {}

void Sessionlogs::addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession)
{
    if (!m_table) {
        qDebug() << "Erro: QTableWidget não inicializado!";
        return;
    }

    int row = m_table->rowCount();  // Obtém o número de linhas
    m_table->insertRow(row);  // Insere uma nova linha

    // Adiciona as informações nas colunas da tabela
    m_table->setItem(row, 0, new QTableWidgetItem(QString::number(sessionNumber)));
    m_table->setItem(row, 1, new QTableWidgetItem(sessionDuration));
    m_table->setItem(row, 2, new QTableWidgetItem(endTimeOfSession));

    qDebug() << "Sessão adicionada com sucesso na tabela!";
}

