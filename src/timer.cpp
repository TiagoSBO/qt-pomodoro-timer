#include "timer.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)

, currentStatusTimer(FOCUS)
, sessionsDoneCount(0)
, currentPomodorSessions(0)
, defaultPomodoroDuration(25)
, defaultShortBreakDuration(5)
, defaultLongBreakDuration(15)
, defaultRoundsSessions(4) //Long break interval
, timeRemaining(defaultPomodoroDuration * 60)
, timerStarted(false)
{
    ui->setupUi(this);
    ui->labelTimer->setText(formatTime(timeRemaining));

    //Table View
    sessionLogs = new Sessionlogs(ui->tableSessionLogs); // Passa o ponteiro da MainWindow
    ui->tableSessionLogs->verticalHeader()->setVisible(false);  // Isso vai esconder os números de linha
    ui->tableSessionLogs->setColumnCount(3);  // Definindo o número de colunas
    ui->tableSessionLogs->setHorizontalHeaderLabels({"Pomodoro done", "Focus time", "End time"});  // Definindo os rótulos das colunas

    //Connect Settings buttons
    settingsScreen = new Settings(this);
    connect(settingsScreen, &Settings::pomodoroDurationChanged, this, &MainWindow::updatePomodoroDuration);
    connect(settingsScreen, &Settings::shortBreakDurationChanged, this, &MainWindow::updateShortBreakDuration);
    connect(settingsScreen, &Settings::longBreakDurationChanged, this, &MainWindow::updateLongBreakDuration);
    connect(settingsScreen, &Settings::pomodoroRoundsChanged, this, &MainWindow::updatePomodoroRounds);

    //Timer creation
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerOut);

    //Connect MainWindow buttons
    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::btton_settings_clicked);
    connect(ui->button_resumePause, &QPushButton::clicked, this, &MainWindow::btton_startResume_clicked);
    connect(ui->button_reset, &QPushButton::clicked, this, &MainWindow::btton_reset_clicked);
    connect(ui->button_skip, &QPushButton::clicked, this, &MainWindow::btton_skip_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settingsScreen;
    delete sessionLogs;
}

void MainWindow::onTimerOut()
{
    // if (currentStatusTimer == PAUSED) return;
    if (timeRemaining > 0) {
            timeRemaining--;
            ui->labelTimer->setText(formatTime(timeRemaining));
        }else {
            timer->stop();
            //notification
            handleSessionCompletion();
    }
}

void MainWindow::btton_startResume_clicked()
{
    running = timer->isActive();
    if (running){
        timer->stop();
        ui->button_resumePause->setText("Resume");
        qDebug() << "Pause Clicked / CurrentState -> " << currentStatusTimer;
    }else{
        timer->start(1000);
        timerStarted = true;
        ui->button_resumePause->setText("Pause");
        qDebug() << "Start/Resume clicked / CurrentState -> " << currentStatusTimer;
    }
    running = !running;
}

void MainWindow::btton_reset_clicked()
{
    // Restaura o tempo com base na sessão atual
    qDebug() << "Botão Reset clicado: " << currentStatusTimer;
    timer->stop();

    switch (currentStatusTimer) {
    case FOCUS:
        timeRemaining = defaultPomodoroDuration * 60;
        currentStatusTimer = FOCUS;
        break;
    case SHORT_BREAK:
        timeRemaining = defaultShortBreakDuration * 60;
        currentStatusTimer = SHORT_BREAK;
        break;
    case LONG_BREAK:
        timeRemaining = defaultLongBreakDuration * 60;
        currentStatusTimer = LONG_BREAK;
        break;
    default:
        break;
    }

    ui->labelTimer->setText(formatTime(timeRemaining));
    ui->button_resumePause->setText("Start");
}

void MainWindow::btton_settings_clicked()
{
    if (timer->isActive()) {
        settingsScreen->setSpinboxesEnabled(false);
    } else {
        settingsScreen->setSpinboxesEnabled(true);
    }

    settingsScreen->setPomodorDuration(defaultPomodoroDuration);
    settingsScreen->setShortBreakDuration(defaultShortBreakDuration);
    settingsScreen->setLongBreakDuration(defaultLongBreakDuration);
    settingsScreen->setPomodoroRounds(defaultRoundsSessions);

    //notification
    settingsScreen->exec();
}

void MainWindow::setSession(TimerState session)
{
    currentStatusTimer = session;

    if (session == FOCUS){
        pomodoroSession();
    }
    else if (session == SHORT_BREAK){
        startShortBreak();
    }else if (session == LONG_BREAK){
        startLongBreak();
    }
}

void MainWindow::btton_skip_clicked()
{
    if (currentStatusTimer == FOCUS) {
        if (timerStarted || running == false) {
            int focusTimeSpent = (defaultPomodoroDuration * 60 - timeRemaining);
            QString formattedFocusTime = formatTime(focusTimeSpent);
            QString endTime = QDateTime::currentDateTime().toString("HH:mm:ss");

            // Adiciona a sessão na tabela
            sessionsDoneCount++;
            sessionLogs->addSession(sessionsDoneCount, formattedFocusTime, endTime);
            timerStarted = false;
            qDebug() << "Focus session skipped! Time spent: " << formattedFocusTime;
        }

        if (currentPomodorSessions >= defaultRoundsSessions) {
            qDebug() << "-->Focus Skipped. Starting LONG BREAK. Status ->" << currentStatusTimer;
            setSession(LONG_BREAK);
        } else{
            setSession(SHORT_BREAK);
            qDebug() << "Focus session skipped! Time spent: " << timeRemaining << " minutes";
            qDebug() << "-->Focus Skipped. Starting SHORT. Status ->>" << currentStatusTimer;;
        }
    }else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK){
        setSession(FOCUS);
    }
}

//Update Spins boxes dialog Settings at runtime
void MainWindow::updatePomodoroDuration(int newTime){
    defaultPomodoroDuration = newTime;
    if (currentStatusTimer == FOCUS && timer->isActive() == false){
        timeRemaining = defaultPomodoroDuration * 60;
        ui->labelTimer->setText(formatTime(timeRemaining));
    }
}
void MainWindow::updateShortBreakDuration(int newTime) {
    defaultShortBreakDuration = newTime;
    if (currentStatusTimer == SHORT_BREAK && timer->isActive() == false){
        timeRemaining = defaultShortBreakDuration * 60;
        ui->labelTimer->setText(formatTime(timeRemaining));
    }
}
void MainWindow::updateLongBreakDuration(int newTime){
    defaultLongBreakDuration = newTime;
    if (currentStatusTimer == LONG_BREAK && timer->isActive() == false){
        timeRemaining = defaultLongBreakDuration * 60;
        ui->labelTimer->setText(formatTime(timeRemaining));
    }
}
void MainWindow::updatePomodoroRounds(int newRounds) { defaultRoundsSessions = newRounds; }

//Sessions pomodoro and rest
void MainWindow::pomodoroSession()
{
    //notification
    currentStatusTimer = FOCUS;
    timer->stop();
    qDebug() << "New Pomodoro session started! / CurrentState -> " << currentStatusTimer;

    running = false;
    ui->button_resumePause->setText("Start");
    timeRemaining = defaultPomodoroDuration * 60; //defaultPomodoroDuration

    qDebug() << "Session Count --> " << currentPomodorSessions;
    ui->labelTimer->setText(formatTime(timeRemaining));
}

void MainWindow::startShortBreak()
{
    timer->stop();
    currentStatusTimer = SHORT_BREAK;
    timerStarted = false;

    running = false;
    ui->button_resumePause->setText("Start");

    //notification
    qDebug() << "Short_Break Started / CurrenState -> " << currentStatusTimer;

    timeRemaining = defaultShortBreakDuration * 60;
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
}

void MainWindow::startLongBreak()
{
    currentStatusTimer = LONG_BREAK;
    timer->stop();
    timerStarted = false;

    //notification
    qDebug() << "Long_Break Started! / CurrentState -> " << currentStatusTimer;

    running = false;
    ui->button_resumePause->setText("Start");

    currentPomodorSessions = 0;

    timeRemaining = defaultLongBreakDuration * 60;
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
}

void MainWindow::handleSessionCompletion()
{
    qDebug() << "currentStatusTimer: " << currentStatusTimer;
    currentPomodorSessions++;

    qDebug() << "Sessions Pomodoro Completed: " << currentPomodorSessions;

    if (currentStatusTimer == FOCUS){
        //TableView
        sessionsDoneCount++;
        QString endTime = QDateTime::currentDateTime().toString("HH:mm:ss");
        sessionLogs->addSession(sessionsDoneCount, QString::number(defaultPomodoroDuration) + " min", endTime);

        if (currentPomodorSessions >= defaultRoundsSessions){
            currentPomodorSessions = 0;
            qDebug() << "Starting LONG BREAK.";
            startLongBreak();
        }else{
            qDebug() << "Starting SHORT BREAK.";
            startShortBreak();
        }
    }
    else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) { //se a sessão encerrada foi SHORT OU LONG
        qDebug() <<  "Starting a new Pomodoro session.";
        pomodoroSession();
    }
}

QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

