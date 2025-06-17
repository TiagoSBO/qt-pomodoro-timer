#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>

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

private slots:
    void dateFilterChanged(const QString &text);
    void clearAllData();

private:
    Ui::StatsWindow *ui;

    void setupSessionTable();
    void showCustomDateDialog();
    double calculateFocusPercentage(int totalPomodoros, int totalMinutesReal);


protected:
    void showEvent(QShowEvent *event) override;

};

#endif // STATSWINDOW_H
