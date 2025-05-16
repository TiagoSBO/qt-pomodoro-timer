#ifndef DATERANGEDIALOG_H
#define DATERANGEDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDate>


namespace Ui {
class DateRangeDialog;
}

class DateRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateRangeDialog(QWidget *parent = nullptr);
    ~DateRangeDialog();

    QDate startDate() const;
    QDate endDate() const;

private:
    Ui::DateRangeDialog *ui;
};

#endif // DATERANGEDIALOG_H
