#include "statswindow.h"
#include <db/dbstatsmanager.h>
#include "daterangedialog.h"
#include <models/focussession.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>


StatsWindow::StatsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatsWindow)
{
    ui->setupUi(this);
    setupSessionTable();

    connect(ui->comboBox_dateFilter, &QComboBox::currentTextChanged, this, &StatsWindow::dateFilterChanged);

    dateFilterChanged(ui->comboBox_dateFilter->currentText());
    QIcon customIcon(":/icons/assets/icons/calendar.png");
    ui->comboBox_dateFilter->setItemIcon(5, customIcon);

    connect(ui->comboBox_dateFilter->view(), &QAbstractItemView::pressed, this, [=](const QModelIndex &index) {
        QString selectedText = ui->comboBox_dateFilter->itemText(index.row());
        if (selectedText == "Custom") {
            showCustomDateDialog();
        }
    });

    connect(ui->deleteStats, &QPushButton::clicked, this, &StatsWindow::clearAllData);

}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::setupSessionTable()
{
    ui->table_sessionData->setColumnCount(5);
    QStringList headers = {"Sessions", "Duration", "Date", "End time", "❌"};
    ui->table_sessionData->setHorizontalHeaderLabels(headers);

    ui->table_sessionData->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->table_sessionData->setColumnWidth(4, 50);

    QHeaderView *header = ui->table_sessionData->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    ui->table_sessionData->verticalHeader()->setVisible(false);
    ui->table_sessionData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_sessionData->horizontalHeader()->setSectionsClickable(false);
    ui->table_sessionData->horizontalHeader()->setStretchLastSection(true);
    ui->table_sessionData->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->table_sessionData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_sessionData->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_sessionData->setAlternatingRowColors(true);
    ui->table_sessionData->setShowGrid(false);

    ui->table_sessionData->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

void StatsWindow::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    dateFilterChanged(ui->comboBox_dateFilter->currentText());
}


void StatsWindow::filterStatsByDateRange(const QDate &start, const QDate &end)
{
    QSqlDatabase db = DbStatsManager::instance().database();

    if (!db.isOpen()) {
        qWarning() << "Banco de dados não está aberto!";
        return;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        SELECT id, start_time, duration_minutes, end_time
        FROM focus_sessions
        WHERE date(start_time) BETWEEN ? AND ?
        ORDER BY start_time ASC
    )");

    query.addBindValue(start.toString("yyyy-MM-dd"));
    query.addBindValue(end.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qWarning() << "Erro ao executar consulta:" << query.lastError().text();
        return;
    }

    int totalPomodoros = 0;
    int totalMinutes = 0;
    QSet<QString> workDays;

    struct SessionInfo {
        int id;
        QDate date;
        int duration;
        QString endTimeStr;
    };

    QList<SessionInfo> sessionData;

    while (query.next()) {
        QString dateStr = query.value("start_time").toString();
        QDateTime startDateTime = QDateTime::fromString(dateStr, Qt::ISODate);
        QDate sessionDate = startDateTime.date();

        QString endTimeStr;
        if (!query.value("end_time").isNull()) {
            QDateTime endDateTime = QDateTime::fromString(query.value("end_time").toString(), Qt::ISODate);
            endTimeStr = endDateTime.time().toString("HH:mm");
        } else {
            endTimeStr = "-";
        }

        int duration = query.value("duration_minutes").toInt();
        totalPomodoros++;
        totalMinutes += duration;
        workDays.insert(sessionDate.toString("yyyy-MM-dd"));
        int sessionId = query.value(0).toInt();
        sessionData.append({sessionId, sessionDate, duration, endTimeStr});
    }

    // Update general labels
    ui->labelValue_pomodoros->setText(QString::number(totalPomodoros));
    ui->labelValue_totalTime->setText(QString("%1h %2m").arg(totalMinutes / 60).arg(totalMinutes % 60));
    ui->labelValue_workDays->setText(QString::number(workDays.size()));

    if (!workDays.isEmpty()) {
        int avg = totalMinutes / workDays.size();
        ui->labelValue_avgFocus->setText(QString("%1h %2m").arg(avg / 60).arg(avg % 60));
    } else {
        ui->labelValue_avgFocus->setText("0h 0m");
    }

    // Label Focus
    double focusPercentage = calculateFocusPercentage(totalPomodoros, totalMinutes);
    ui->labelValue_focus->setText(QString("%1%").arg(static_cast<int>(focusPercentage)));

    // Update sessions table
    ui->table_sessionData->setRowCount(sessionData.size());

    for (int i = 0; i < sessionData.size(); ++i) {
        const SessionInfo &info = sessionData[i];

        ui->table_sessionData->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->table_sessionData->setItem(i, 1, new QTableWidgetItem(QString("%1 min").arg(info.duration)));
        ui->table_sessionData->setItem(i, 2, new QTableWidgetItem(info.date.toString("dd/MM/yyyy")));
        ui->table_sessionData->setItem(i, 3, new QTableWidgetItem(info.endTimeStr));

        // Widget de alinhamento
        QWidget *cellWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(cellWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignCenter);

        //Table row delete button
        QPushButton *actionBtn = new QPushButton();
        actionBtn->setObjectName("SessionActionButton");
        actionBtn->setProperty("sessionId", info.id);
        actionBtn->setProperty("rowIndex", i);
        actionBtn->setCursor(Qt::PointingHandCursor);
        actionBtn->setFlat(true);
        actionBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QIcon menuIcon(":/icons/assets/icons/bttn_more.png");
        actionBtn->setIcon(menuIcon);
        layout->addWidget(actionBtn);

        connect(actionBtn, &QPushButton::clicked, this, &StatsWindow::showSessionMenu);

        // ui->table_sessionData->setCellWidget(i, 4, actionBtn);
        ui->table_sessionData->setCellWidget(i, 4, cellWidget);

        for (int col = 0; col < 4; ++col) {
            QTableWidgetItem *item = ui->table_sessionData->item(i, col);
            if (item) item->setTextAlignment(Qt::AlignCenter);
        }
    }
    // Table height
    int maxVisibleRows = 5;

    if (ui->table_sessionData->rowCount() > maxVisibleRows) {
        ui->table_sessionData->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        int headerHeight = ui->table_sessionData->horizontalHeader()->height();
        int rowHeight = ui->table_sessionData->rowHeight(0);
        int maxHeight = headerHeight + (rowHeight * maxVisibleRows);

        ui->table_sessionData->setMaximumHeight(maxHeight);
        ui->table_sessionData->setMinimumHeight(maxHeight);
    } else {
        int totalHeight = ui->table_sessionData->horizontalHeader()->height();
        for (int row = 0; row < ui->table_sessionData->rowCount(); ++row)
            totalHeight += ui->table_sessionData->rowHeight(row);

        ui->table_sessionData->setFixedHeight(totalHeight + 2);
    }
    ui->table_sessionData->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void StatsWindow::dateFilterChanged(const QString &text)
{
    QDate today = QDate::currentDate();
    QDate start, end;


    if (text == "Today") {
        start = today;
        end = today;
    }
    else if (text == "Yesterday") {
        start = today.addDays(-1);
        end = start;
    }
    else if (text == "This week") {
        int dayOfWeek = today.dayOfWeek();
        start = today.addDays(-dayOfWeek + 1);
        end = start.addDays(6);
    }
    else if (text == "This month") {
        start = QDate(today.year(), today.month(), 1);
        end = start.addMonths(1).addDays(-1);
    }
    else if (text == "This year") {
        start = QDate(today.year(), 1, 1);
        end = QDate(today.year(), 12, 31);
    }
    else if (text == "Custom") {
        return;
    }
    filterStatsByDateRange(start, end);
}

void StatsWindow::showCustomDateDialog()
{
    DateRangeDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QDate start = dialog.startDate();
        QDate end = dialog.endDate();
        filterStatsByDateRange(start, end);
    }
}

double StatsWindow::calculateFocusPercentage(int totalPomodoros, int totalMinutesReal)
{
    QSettings settings("QtPomodoro-Timer", "QtPomodoroApp");
    int configuredDuration = settings.value("pomodoroDuration", 25).toInt();

    if (totalPomodoros == 0 || configuredDuration == 0)
        return 0.0;

    int expectedTotalTime = totalPomodoros * configuredDuration;

    double percentage = (static_cast<double>(totalMinutesReal) / expectedTotalTime) * 100.0;
    return qBound(0.0, percentage, 100.0);
}


void StatsWindow::clearAllData() {

    if (DbStatsManager::instance().clearAllSessions()) {
        QMessageBox::StandardButton clearAllSessions = QMessageBox::question(this, "Danger Zone",
                "Are you sure you want to DELETE ALL DATA?",
                QMessageBox::Yes | QMessageBox::No);
        if (clearAllSessions == QMessageBox::Yes) {
            dateFilterChanged(ui->comboBox_dateFilter->currentText());
            qDebug() << "Sessões removidas com sucesso.";
        }
    } else {
        qWarning() << "Erro ao remover sessões.";
    }
}

void StatsWindow::showSessionMenu()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    int sessionId = btn->property("sessionId").toInt();
    int rowIndex = btn->property("rowIndex").toInt();

    QMenu menu;
    QAction *deleteAction = menu.addAction("Delete session");

    QAction *selected = menu.exec(QCursor::pos());
    if (selected == deleteAction) {
        QMessageBox::StandardButton clearSelectedSession = QMessageBox::question(this, "Delete session?",
            "Are you sure you want to delete this session?",
             QMessageBox::Yes | QMessageBox::No);
        if (clearSelectedSession == QMessageBox::Yes) {
            deleteSessionById(sessionId, rowIndex);
            qDebug() << "Sessão removida com sucesso.";
        } else {
            qWarning() << "Erro ao remover sessão.";
        }
    }
}

void StatsWindow::deleteSessionById(int sessionId, int rowIndex)
{
    QSqlDatabase db = DbStatsManager::instance().database();

    if (!db.isOpen()) {
        qWarning() << "Banco não aberto.";
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM focus_sessions WHERE id = :id");
    query.bindValue(":id", sessionId);

    if (query.exec()) {
        ui->table_sessionData->removeRow(rowIndex);
    } else {
        qWarning() << "Erro ao deletar sessão:" << query.lastError().text();
    }

    adjustTableHeight(ui->table_sessionData->rowCount());
    dateFilterChanged(ui->comboBox_dateFilter->currentText());
}



void StatsWindow::adjustTableHeight(int rowCount)
{
    const int maxVisibleRows = 5;

    if (rowCount > maxVisibleRows) {
        ui->table_sessionData->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        int headerHeight = ui->table_sessionData->horizontalHeader()->height();
        int rowHeight = ui->table_sessionData->rowHeight(0);
        int maxHeight = headerHeight + (rowHeight * maxVisibleRows);

        ui->table_sessionData->setMaximumHeight(maxHeight);
        ui->table_sessionData->setMinimumHeight(maxHeight);
    } else {
        int totalHeight = ui->table_sessionData->horizontalHeader()->height();
        for (int row = 0; row < rowCount; ++row)
            totalHeight += ui->table_sessionData->rowHeight(row);

        ui->table_sessionData->setFixedHeight(totalHeight + 2);
    }

    ui->table_sessionData->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}



