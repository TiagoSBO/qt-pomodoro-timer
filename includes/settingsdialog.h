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

    void setPomodorDuration(int duration);
    int getPomodoroDuration() const;

signals:
    void valueChanged(int newValue);

private slots:
    void emitTimeChanged();

private:
    Ui::Settings *ui;

    int pomodoroDuration;
};

#endif // SETTINGSDIALOG_H
