#ifndef TIMER_H
#define TIMER_H

#include <QMainWindow>
#include <QTimer>

// Estados do Timer
enum TimerState {
    IDLE,        // Antes do Start
    RUNNING,     // Cronômetro rodando
    PAUSED,      // Cronômetro pausado
    FINISHED,    // Sessão finalizada
    SHORT_BREAK, // Descanso curto
    LONG_BREAK   // Descanso longo
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
    void defaultTimerFocus();
    void btton_startResume_clicked();
    void btton_stopDone_clicked();
    void btton_settings_clicked();
    void updateTimeValue(int newTime);

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    TimerState currentStatusTimer;

    // Funções específicas para cada tipo de sessão
    void startPomodoroSession();
    void startShortBreak();
    void startLongBreak();

    void handleSessionCompletion();
    void resetTimer();

    int currentPomodorSessions;
    int defaultPomodorSessions;
    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultSessionsLongBreak;

    int timeRemaining;
};

#endif // TIMER_H
