#include "statswindow.h"
#include "daterangedialog.h"
#include "ui_statswindow.h"

StatsWindow::StatsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatsWindow)
{
    ui->setupUi(this);

    connect(ui->comboBox_dateFilter, &QComboBox::currentTextChanged, this, &StatsWindow::dateFilterChanged);
}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::filterStatsByDateRange(const QDate &start, const QDate &end)
{

    // QFile file(":/data/pomodoros.json"); // ou caminho real no disco
    // if (!file.open(QIODevice::ReadOnly)) {
    //     qWarning() << "Não foi possível abrir o arquivo JSON.";
    //     return;
    // }

    // QByteArray jsonData = file.readAll();
    // QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    // file.close();

    // if (!doc.isArray()) {
    //     qWarning() << "Formato JSON inválido.";
    //     return;
    // }

    // QJsonArray array = doc.array();

    // int totalPomodoros = 0;
    // int totalMinutes = 0;
    // QSet<QDate> workDays;


    // Atualizar os labels (supondo que você tenha QLabel chamados label_pomodoros etc.)
    // ui->label_pomodoros_value->setText(QString::number(totalPomodoros));
    // ui->label_timeSpent_value->setText(QString("%1h %2m").arg(totalMinutes / 60).arg(totalMinutes % 60));
    // ui->label_workDays_value->setText(QString::number(workDays.size()));

    // if (!workDays.isEmpty())
    //     ui->label_avgFocus_value->setText(QString("%1h %2m")
    //                                           .arg((totalMinutes / workDays.size()) / 60)
    //                                           .arg((totalMinutes / workDays.size()) % 60));
    // else
    //     ui->label_avgFocus_value->setText("0h 0m");
    // }
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
        int dayOfWeek = today.dayOfWeek(); // 1 (Monday) - 7 (Sunday)
        start = today.addDays(-dayOfWeek + 1);
        end = today;
    }
    else if (text == "This month") {
        start = QDate(today.year(), today.month(), 1);
        end = today;
    }
    else if (text == "This year") {
        start = QDate(today.year(), 1, 1);
        end = today;
    }
    else if (text == "Custom") {
        DateRangeDialog dialog(this);

        if (dialog.exec() == QDialog::Accepted) {
            start = dialog.startDate();
            end = dialog.endDate();
        } else {
            ui->comboBox_dateFilter->setCurrentText("This week");
            return;
        }
    } else {
        qWarning() << "Filtro de data desconhecido:" << text;
        return;
    }

    // Aplica o filtro com o intervalo definido
    filterStatsByDateRange(start, end);
}


