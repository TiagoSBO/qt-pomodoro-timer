#include "timer.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QDebug>
#include <QString>
#include <QChar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)

, currentStatusTimer(FOCUS)
, currentPomodorSessions(0)
, defaultPomodoroDuration(25)
, defaultShortBreakDuration(5)
, defaultLongBreakDuration(15)
, defaultRoundsSessions(4) //Long break interval
, timeRemaining(defaultPomodoroDuration * 60)
{
    ui->setupUi(this);
    ui->labelTimer->setText(formatTime(timeRemaining));

    settingsScreen = new Settings(this);
    //Conexão dos botões do Settings
    connect(settingsScreen, &Settings::pomodoroDurationChanged, this, &MainWindow::updatePomodoroDuration);
    connect(settingsScreen, &Settings::shortBreakDurationChanged, this, &MainWindow::updateShortBreakDuration);
    connect(settingsScreen, &Settings::longBreakDurationChanged, this, &MainWindow::updateLongBreakDuration);
    connect(settingsScreen, &Settings::pomodoroRoundsChanged, this, &MainWindow::updatePomodoroRounds);

    //Criação do Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerOut);

    //Botões da Janela Principal
    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::btton_settings_clicked);
    connect(ui->button_resumePause, &QPushButton::clicked, this, &MainWindow::btton_startResume_clicked);
    connect(ui->button_reset, &QPushButton::clicked, this, &MainWindow::btton_reset_clicked);
    connect(ui->button_skip, &QPushButton::clicked, this, &MainWindow::btton_skip_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settingsScreen;
}

void MainWindow::onTimerOut()
{
    // if (currentStatusTimer == PAUSED) return;
    if (timeRemaining > 0) {
            timeRemaining--;
            ui->labelTimer->setText(formatTime(timeRemaining));
        }else {
            timer->stop();
            handleSessionCompletion();
            //notification
    }
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

void MainWindow::btton_startResume_clicked()
{
    running = timer->isActive();
    if (running){
        timer->stop();
        ui->button_resumePause->setText("Resume");
        qDebug() << "Pause Clicked / CurrentState -> " << currentStatusTimer;
    }else{
        timer->start(1000);
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
    settingsScreen->setPomodorDuration(defaultPomodoroDuration);
    settingsScreen->setShortBreakDuration(defaultShortBreakDuration);
    settingsScreen->setLongBreakDuration(defaultLongBreakDuration);
    settingsScreen->setPomodoroRounds(defaultRoundsSessions);

    //notification
    settingsScreen->exec();
}

void MainWindow::btton_skip_clicked(){
    // if (currentStatusTimer == IDLE) return;

    if (currentStatusTimer == FOCUS) {
        if (currentPomodorSessions >= defaultRoundsSessions) {
            qDebug() << "-->Focus Skipped. Starting LONG BREAK. Status ->" << currentStatusTimer;
            setSession(LONG_BREAK); //
        } else{
            setSession(SHORT_BREAK);
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

void MainWindow::pomodoroSession()
{
    qDebug() << "New Pomodoro session started! / CurrentState -> " << currentStatusTimer;
    currentStatusTimer = FOCUS;
    timeRemaining = defaultPomodoroDuration * 60; // Reinicia o tempo do Pomodoro
    ui->labelTimer->setText(formatTime(timeRemaining));
    timer->stop();

    if (running == false){
        ui->button_resumePause->setText("Pause");
    }else{
        ui->button_resumePause->setText("Start");
    }
}

void MainWindow::startShortBreak()
{
    qDebug() << "Short_Break Started / CurrenState -> " << currentStatusTimer;

    currentPomodorSessions++;

    qDebug() << "Session Count --> " << currentPomodorSessions;

    currentStatusTimer = SHORT_BREAK;
    timeRemaining = defaultShortBreakDuration * 60; // Tempo de descanso curto
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
    timer->stop();

    if (running == false){
        ui->button_resumePause->setText("Pause");
    }else{
        ui->button_resumePause->setText("Start");
    }
}

void MainWindow::startLongBreak()
{
    qDebug() << "Long_Break Started! / CurrentState -> " << currentStatusTimer;

    currentPomodorSessions = 0;

    currentStatusTimer = LONG_BREAK;
    timeRemaining = defaultLongBreakDuration * 60; // Tempo de descanso longo
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
    timer->stop();

    if (running == false){
        ui->button_resumePause->setText("Pause");
    }else{
        ui->button_resumePause->setText("Start");
    }
}

void MainWindow::handleSessionCompletion()
{
    qDebug() << "Sessions Pomodoro Completed: " << currentPomodorSessions;
    qDebug() << "currentStatusTimer: " << currentStatusTimer;

    if (currentStatusTimer == FOCUS){ //se a sessão encerrada foi focus
        if (currentPomodorSessions >= defaultRoundsSessions){
            //currentPomodorSessions > 2 && (currentPomodorSessions -1) % defaultRoundsSessions == 0
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
    ui->button_resumePause->setText("Start");
}

QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

