#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class Settings;
}

class MainWindow;

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    //SETS
    void setPomodorDuration(int duration);
    void setShortBreakDuration(int duration);
    void setLongBreakDuration(int duration);
    void setPomodoroRounds(int interval);
    void setSpinboxesEnabled(bool enabled);
    //GETS
    int getPomodoroDuration() const;
    int getShortBreakDuration() const;
    int getLongBreakDuration() const;
    int getPomodoroRounds() const;


public slots:
    // void resetTimerDefaults();

private slots:
    void emitTimeValueChanged();

signals:
    void pomodoroDurationChanged(int newValue);
    void shortBreakDurationChanged(int newValue);
    void longBreakDurationChanged(int newValue);
    void pomodoroRoundsChanged(int newValue);

private:
    Ui::Settings *ui;
    MainWindow *mainwwindow;

};

#endif // SETTINGSDIALOG_H
