#ifndef TIMER_H
#define TIMER_H

#include <QMainWindow>
#include <QTimer>

enum TimerState {
    IDLE,       // Antes do Start
    RUNNING,    // Cronômetro rodando
    PAUSED,     // Cronômetro pausado
    FINISHED,
    SHORT_BREAK,
    LONG_BREAK    // Pomodoro ou descanso concluído
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

    void startPomodorSession();
    void handleSessionCompletion();
    void startBreak(TimerState breaktype);
    void resetTimer();

    int defaultPomodorSessions;
    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultSessionsLongBreak;

    int timeRemaining;
};
#endif // TIMER_H
