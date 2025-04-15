#ifndef TIMER_H
#define TIMER_H
#include <QMainWindow>
#include <QTimer>
#include "helpwindow.h"
#include "sessionlogs.h"
#include "settingsdialog.h"
#include "QMessageBox"
#include <QDateTime>
#include <QDir>
#include <QString>
#include <QChar>
#include <QAction>
#include <QMenu>
#include <QStringList>
#include <QMap>

// Estados do Timer
enum TimerState {
    FOCUS, SHORT_BREAK, LONG_BREAK
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

public slots:
    void setAlarmSound(int index);

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

    //Table button
    void button_configTable_clicked();
    //Help dialog window
    void openHelpDialog();

private:
    Ui::MainWindow *ui;

    Settings *settingsScreen;
    Sessionlogs *sessionLogs;
    HelpWindow *helpWindow;

    QTimer *timer;
    TimerState currentStatusTimer;
    SoundManager soundManager;

    void setSession(TimerState session);
    QString formatTime(int seconds);

    int timerStarted;

    // Funções específicas para cada tipo de sessão
    void pomodoroSession();
    void startShortBreak();
    void startLongBreak();
    void handleSessionCompletion();

    //Style
    void updateStyleBasedOnState();

    //Others
    int currentPomodorSessions;
    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultRoundsSessions;
    int timeRemaining;
    int running;
    int sessionsDoneCount; // Contador de sessões finalizadas

    int selectedISoundIndex = 0;
    void updateTotalFocusTime();
};

#endif // TIMER_H
