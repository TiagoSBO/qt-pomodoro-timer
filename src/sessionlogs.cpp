#include "sessionlogs.h"
#include <QTableWidgetItem>
#include <QDebug>

Sessionlogs::Sessionlogs(QTableWidget *table)
    : m_table(table)  // Inicializa com o ponteiro da tabela
{}

Sessionlogs::~Sessionlogs() {}

void Sessionlogs::addSession(int sessionNumber, const QString &sessionDuration, const QString &endTimeOfSession)
{
    if (!m_table) {
        qDebug() << "Erro: QTableWidget não inicializado!";
        return;
    }

    int row = m_table->rowCount();  // Obtém o número de linhas
    m_table->insertRow(row);  // Insere uma nova linha
    if (row % 4 == 0 && row != 0) {
        m_table->insertRow(row);

        QTableWidgetItem *groupHeader = new QTableWidgetItem("🕒 Ciclo " + QString::number(row / 4 + 1));
        groupHeader->setTextAlignment(Qt::AlignCenter);
        groupHeader->setFlags(Qt::NoItemFlags);
        groupHeader->setFont(QFont("Arial", 10, QFont::Bold));

        m_table->setSpan(row, 0, 1, 3);  // Mescla as colunas para o cabeçalho
        m_table->setItem(row, 0, groupHeader);

        row++;
    }

    // Criando itens e alinhando ao centro
    QTableWidgetItem *itemSession = new QTableWidgetItem(QString::number(sessionNumber));
    itemSession->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *itemDuration = new QTableWidgetItem(sessionDuration);
    itemDuration->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *itemEndTime = new QTableWidgetItem(endTimeOfSession);
    itemEndTime->setTextAlignment(Qt::AlignCenter);

    // Adiciona os itens na tabela
    m_table->setItem(row, 0, itemSession);
    m_table->setItem(row, 1, itemDuration);
    m_table->setItem(row, 2, itemEndTime);


    qDebug() << "Sessão adicionada com sucesso na tabela!";
}
