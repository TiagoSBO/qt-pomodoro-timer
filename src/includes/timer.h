#ifndef TIMER_H
#define TIMER_H
#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include <QEvent>
#include "helpwindow.h"
#include "sessionlogs.h"
#include "settingsdialog.h"
#include "systemtrayiconhandler.h"
#include "floatingtimerwindow.h"
#include <QCloseEvent>
#include "QMessageBox"
#include <QDateTime>
#include <QDir>
#include <QString>
#include <QChar>
#include <QAction>
#include <QMenu>
#include <QStringList>
#include <QMap>

// Timer States
enum TimerState {
    FOCUS, SHORT_BREAK, LONG_BREAK
};

enum MenuTableAction {
    ClearTableContent,
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

signals:
    void timerUpdated(const QString &timeString);


private:
    Ui::MainWindow *ui;

    Settings *settingsScreen;
    Sessionlogs *sessionLogs;
    HelpWindow *helpWindow;

    SystemTrayiconHandler *systemTrayIcon;
    void restoreFromTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void closeEvent(QCloseEvent *event) override;
    bool isExiting = false;

    SoundManager soundManager;
    QString formatTime(int seconds);

    //Timer
    QTimer *timer;
    TimerState currentStatusTimer;
    void pomodoroSession();
    void startShortBreak();
    void startLongBreak();
    void handleSessionCompletion();
    void setSession(TimerState session);

    FloatingTimerWindow *floatingTimerWindow;
    void toggleFloatingWindow();
    void showFromFloating();

    //Style
    void updateStyleBasedOnState();

    //
    int timerStarted;
    int currentPomodorSessions;
    int defaultPomodoroDuration;
    int defaultShortBreakDuration;
    int defaultLongBreakDuration;
    int defaultRoundsSessions;
    int timeRemaining;
    int running;
    int sessionsDoneCount;

    int selectedISoundIndex = 0;
    void updateTotalFocusTime();
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif
