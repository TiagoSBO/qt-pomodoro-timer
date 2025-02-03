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
    // void setShortBreakDuration(int duration);
    // void setLongBreakInterval(int interval);
    void setPomodoroRounds(int interval);

    int getPomodoroDuration() const;
    // int getLongBreakInterval() const;
    // int getShortBreakDuration() const;
    int getPomodoroRounds() const;

signals:
    void pomodoroDurationChanged(int newValue);
    // void longBreakIntervalChanged(int newValue);
    // void shortBreakDurationChanged(int newValue);
    void pomodoroRoundsChanged(int newValue);

private slots:
    void emitTimeValueChanged();

private:
    Ui::Settings *ui;

};

#endif // SETTINGSDIALOG_H
