#ifndef TIMER_H
#define TIMER_H

#include <QMainWindow>
#include <QTimer>
#include "settingsdialog.h"


// Estados do Timer
enum TimerState {
    IDLE, FOCUS, SHORT_BREAK, LONG_BREAK, PAUSED
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTimerDefaults(int pomodoroDuration, int shortBreak, int longBreak, int sessionsBeforeLong);

    void start();
    void pause();

signals:
    void sessionChanged(TimerState newSession);

private slots:
    //Buttons
    void btton_startResume_clicked();
    void btton_reset_clicked();
    void btton_settings_clicked();
    void btton_skip_clicked();

    //Update Configs
    void updatePomodoroDuration(int newTime);
    void updateShortBreakDuration(int newTime);
    void updateLongBreakDuration(int newTime);
    void updatePomodoroRounds(int newRounds);

    //Slot Timer
    void onTimerOut();

private:
    Ui::MainWindow *ui;
    Settings *settingsScreen;

    QTimer *timer;

    TimerState currentStatusTimer;
    TimerState previousStatusTimer;
    void setSession(TimerState session);
    QString formatTime(int seconds);

    // Funções específicas para cada tipo de sessão
    void pomodoroSession();
    void startShortBreak();
    void startLongBreak();
    void handleSessionCompletion();

    int currentPomodorSessions;
    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultRoundsSessions;
    int timeRemaining;
    int running;
};

#endif // TIMER_H
