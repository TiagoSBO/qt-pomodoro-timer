#ifndef TIMER_H
#define TIMER_H

#include <QMainWindow>
#include <QTimer>

enum TimerState {
    IDLE,       // Antes do Start
    RUNNING,    // Cronômetro rodando
    PAUSED,     // Cronômetro pausado
    FINISHED    // Pomodoro ou descanso concluído
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

    int initialSeconds;
    int customDuration;
    int timeRemaining;
};
#endif // TIMER_H
