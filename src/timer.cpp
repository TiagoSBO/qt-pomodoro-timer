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
, currentPomodorSessions(0) //contagem inicial das sessões
, defaultPomodoroDuration(25)
, defaultShortBreakDuration(5)
, defaultLongBreakDuration(15)
, defaultRoundsSessions(4) //Long break intervalSS
, timeRemaining(defaultPomodoroDuration * 60)

{
    ui->setupUi(this);
    //Configurações iniciais
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

        // if (currentStatusTimer == IDLE) { // ANTES DE INICIAR
        //     ui->button_resumePause->setText("Pause");
        //     timeRemaining = defaultPomodoroDuration * 60;
        //     timer->start(1000);
        //     previousStatusTimer = IDLE;
        //     currentStatusTimer = FOCUS;
        //     qDebug() << "Botão START clicado: " << currentStatusTimer;
        // }
        // // else if (timer->isActive()) { // PAUSANDO
        // else if (currentStatusTimer == FOCUS){
        //     if (timer->isActive()){
        //         timer->stop();
        //         currentStatusTimer = PAUSED;
        //         ui->button_resumePause->setText("Start");
        //     }else{
        //         currentStatusTimer = FOCUS;
        //     }
        //     // ui->button_resumePause->setText("Resume");
        //     qDebug() << "Botão Pause clicado: " << currentStatusTimer;
        // }
        // else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK){
        //     if (timer->isActive() == false){ //se o timer estiver pausado
        //         qDebug() << "alllo" << currentStatusTimer;
        //         timer->start();
        //         // currentStatusTimer = FOCUS;
        //         ui->button_resumePause->setText("Pause");
        //     }else{
        //         timer->stop();
        //         qDebug() << "alllo 2"  << currentStatusTimer;
        //         // currentStatusTimer = PAUSED;
        //     }
        // }
        // else if(currentStatusTimer == PAUSED){
        //     timer->start();
        //     previousStatusTimer = PAUSED;
        //     currentStatusTimer = FOCUS;
        //     qDebug() << "Botão RESUME clicado: " << currentStatusTimer;
        //     ui->button_resumePause->setText("Pause");

        //     if (currentStatusTimer == PAUSED) {
        //         currentStatusTimer = FOCUS;  // Retorna para FOCUS
        //     }
        // }
}

void MainWindow::btton_reset_clicked()
{
    // Restaura o tempo com base na sessão atual


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
    currentStatusTimer = FOCUS;
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
    // timer->stop();

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
    // if (currentStatusTimer == PAUSED){
    //     qDebug() << "--> Skipped in Paused";
    //     if (timeRemaining == defaultShortBreakDuration * 60){ //if (previousStatusTimer == FOCUS)
    //         currentStatusTimer = SHORT_BREAK; // SHORT_BREAK
    //         qDebug() << "-->Short Break Skipped. Starting a new Pomodoro session.";
    //     }else if(timeRemaining == defaultLongBreakDuration * 60){
    //         currentStatusTimer = LONG_BREAK;
    //         qDebug() << "-->Long Break Skipped. Starting a new Pomodoro session.";
    //     }
    // }


    // else if (currentStatusTimer == SHORT_BREAK){
    //     currentStatusTimer = SHORT_BREAK; // SHORT_BREAK
    //     qDebug() << "-->Short Break Skipped. Starting a new Pomodoro session.";
    // }else if (currentStatusTimer == LONG_BREAK){
    //     currentStatusTimer = LONG_BREAK;
    //     qDebug() << "-->Long Break Skipped. Starting a new Pomodoro session.";
    // }
}


void MainWindow::updatePomodoroDuration(int newTime)
{
    defaultPomodoroDuration = newTime;

    if (currentStatusTimer == FOCUS){
        timeRemaining = defaultPomodoroDuration * 60; // Atualiza o tempo total
        ui->labelTimer->setText(formatTime(timeRemaining)); // Atualiza a label
    }
}
void MainWindow::updateShortBreakDuration(int newTime) { defaultShortBreakDuration = newTime; }
void MainWindow::updateLongBreakDuration(int newTime) { defaultLongBreakDuration = newTime; }
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

    currentStatusTimer = LONG_BREAK;
    timeRemaining = defaultLongBreakDuration * 60; // Tempo de descanso longo
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
    timer->stop();

    if (running == false){
        ui->button_resumePause->setText("Pause");
    }else{
        ui->button_resumePause->setText("Start");
    }

    currentPomodorSessions = 0;
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
    // ui->labelTimer->setText(formatTime(timeRemaining));
    ui->button_resumePause->setText("Start");

}



QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

