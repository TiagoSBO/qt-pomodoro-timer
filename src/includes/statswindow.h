#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QDateTime>
#include "ui_statswindow.h"
#include <QSqlDatabase>
#include <QSet>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
#include <QIcon>
#include <qmenu.h>

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatsWindow(QWidget *parent = nullptr);
    ~StatsWindow();
    void filterStatsByDateRange(const QDate &start, const QDate &end);

    void addStats(int pomodoros, const QString &totalTimeSpent, const QString &averageFocusPerDay, const QString &workDays);

    struct SessionInfo {
        int id;
        QDate date;
        int duration;
        QString endTimeStr;
    };

private slots:
    void dateFilterChanged(const QString &text);
    void clearAllData();
    void showSessionMenu();
    void deleteSessionById(int sessionId, int rowIndex);

private:
    Ui::StatsWindow *ui;

    void setupSessionTable();
    void showCustomDateDialog();
    double calculateFocusPercentage(int totalPomodoros, int totalMinutesReal);
    void adjustTableHeight(int rowCount);

protected:
    void showEvent(QShowEvent *event) override;

};

#endif // STATSWINDOW_H
