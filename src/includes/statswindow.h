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

private slots:
    void dateFilterChanged(const QString &text);

private:
    Ui::StatsWindow *ui;
};

#endif // STATSWINDOW_H
