#ifndef TIMER_H
#define TIMER_H

#include <QMainWindow>
#include <QTimer>
#include "settingsdialog.h"


// Estados do Timer
enum TimerState {
    IDLE, RUNNING, PAUSED, SKIPED, FINISHED, SHORT_BREAK, LONG_BREAK
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTimerDefaults(int pomodoroDuration, int shortBreak, int longBreak, int sessionsBeforeLong);

private slots:
    //Spinboxes
    void defaultTimerFocus();
    void btton_startResume_clicked();
    void btton_reset_clicked();
    void btton_settings_clicked();
    void btton_skip_clicked();

    //Spinboxes do Dialog
    void updatePomodoroDuration(int newTime);
    void updateShortBreakDuration(int newTime);
    void updateLongBreakDuration(int newTime);
    void updatePomodoroRounds(int newRounds);


private:
    Ui::MainWindow *ui;
    QTimer *timer;

    TimerState currentStatusTimer;
    Settings *settingsScreen;

    // Funções específicas para cada tipo de sessão
    void startPomodoroSession();
    void startShortBreak();
    void startLongBreak();
    void handleSessionCompletion();
    QString formatTime(int seconds);

    int currentPomodorSessions;

    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultRoundsSessions;

    int timeRemaining;
};

#endif // TIMER_H
