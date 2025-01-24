#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

signals:
    void valueChanged(int timeValue);

private slots:
    void emitTimeChanged();

private:
    Ui::Settings *ui;
};

#endif // SETTINGSDIALOG_H
