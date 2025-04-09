#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "soundmanager.h"
#include <QDialog>
#include <QLineEdit>
#include <QDebug>
#include <QSoundEffect>

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

private slots:
    void emitTimeValueChanged();
    void setAlarm_sound();
    void on_volume_slider_valueChanged(int value);
    void onSoundAlertToggled(bool checked);

signals:
    void pomodoroDurationChanged(int newValue);
    void shortBreakDurationChanged(int newValue);
    void longBreakDurationChanged(int newValue);
    void pomodoroRoundsChanged(int newValue);
    void notificationSoundChanged(int index);
    void volumeChanged(int volume);
    void soundEnabledChanged(bool enabled);

private:
    Ui::Settings *ui;
    MainWindow *mainwwindow;
    SoundManager *soundManager;
    QAudioOutput *audioOutput;

};

#endif // SETTINGSDIALOG_H
