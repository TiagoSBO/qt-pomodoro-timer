#ifndef TIMER_H
#define TIMER_H

#include <QMainWindow>
#include <QTimer>
#include "settingsdialog.h"


// Estados do Timer
enum TimerState {
    IDLE,        // Antes do Start
    RUNNING,     // Cronômetro rodando == "Start"
    PAUSED,      // Cronômetro pausado == "Pause"
    FINISHED,    // Sessão finalizada == Cronometro chegou ao tempo limite ou foi skipado
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
    //Spinboxes
    void defaultTimerFocus();
    void btton_startResume_clicked();
    void btton_reset_clicked();
    void btton_settings_clicked();
    // void btton_skip_clicked();

    //Timer
    void updatePomodoroDuration(int newTime);
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

    int currentPomodorSessions;
    int defaultPomodorSessions;
    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultSessionsLongBreak;

    int timeRemaining;
};

#endif // TIMER_H
