#include "timer.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QDebug>
#include <QString>
#include <QChar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)

, currentStatusTimer(IDLE)
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
    if (currentStatusTimer == PAUSED) return;

    if (currentStatusTimer == FOCUS){
        if (timeRemaining > 0) {
            timeRemaining--;
            ui->labelTimer->setText(formatTime(timeRemaining));
        }else {
            timer->stop();
            //notification
            handleSessionCompletion(); // Inicia a próxima etapa após finalizar a sessão
        }
    }else if (currentStatusTimer == SHORT_BREAK){
        //notification
        handleSessionCompletion();
    }else if (currentStatusTimer == LONG_BREAK){
        //notification
        handleSessionCompletion();
    }
}

void MainWindow::btton_startResume_clicked()
{
    {
        if (currentStatusTimer == IDLE) { // ANTES DE INICIAR
            timer->start(1000);
            currentStatusTimer = FOCUS;
            ui->button_resumePause->setText("Pause");
        }
        // else if (timer->isActive()) { // PAUSANDO
        else if (currentStatusTimer == FOCUS){
            timer->stop();
            ui->button_resumePause->setText("Resume");
            currentStatusTimer = PAUSED;  // ADICIONADO STATUS PAUSED
        }
        else if(currentStatusTimer == PAUSED){
            timer->start();
            qDebug() << "Estou aqui";
            qDebug() << "Status Value" << currentStatusTimer;
            ui->button_resumePause->setText("Pause");

            if (currentStatusTimer == PAUSED) {
                currentStatusTimer = FOCUS;  // Retorna para FOCUS
            }
        }
        else{
            timer->start(1000);
            qDebug() << "Estou aqui";
            qDebug() << "Status Value" << currentStatusTimer;
            ui->button_resumePause->setText("Pause");
        }
        // else { // RETOMANDO
        //     timer->start(1000);
        //     qDebug() << "Estou aqui";
        //     qDebug() << "Status Value" << currentStatusTimer;
        //     ui->button_resumePause->setText("Pause");

        //     if (currentStatusTimer == PAUSED) {
        //         currentStatusTimer = FOCUS;  // Retorna para FOCUS
        //     }
        //}
    }
}

void MainWindow::btton_reset_clicked() {

    if (currentStatusTimer == IDLE) return;

    timer->stop(); // Para o temporizador antes de redefinir
    timeRemaining = 0;

    // Define o tempo correto com base no estado atual
    if (currentStatusTimer == FOCUS){
        qDebug() << "Button reset cliclado no FOCUS";
        timeRemaining = defaultPomodoroDuration * 60;
        currentStatusTimer = FOCUS;
    }else if (currentStatusTimer == SHORT_BREAK) {
        qDebug() << "Button reset cliclado no SHORT BREAK";
        timeRemaining = defaultShortBreakDuration * 60;
        currentStatusTimer = SHORT_BREAK;
    }else if (currentStatusTimer == LONG_BREAK) {
        qDebug() << "Button reset cliclado no LONG BREAK";
        timeRemaining = defaultLongBreakDuration * 60;
        currentStatusTimer = LONG_BREAK;
    }else{
        qDebug() << "ELSE DO IF";
    }

    // Atualiza a interface com o tempo resetado
    ui->labelTimer->setText(formatTime(timeRemaining));
    ui->button_resumePause->setText("Start");

    currentStatusTimer = IDLE;

    // Mantém o estado atual, mas para o timer
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
    timer->stop();

    if (currentStatusTimer == FOCUS) {
        if (currentPomodorSessions > 2 && (currentPomodorSessions - 1) % defaultRoundsSessions == 0) {
            currentStatusTimer = FOCUS;
            qDebug() << "Focus Skipped. Starting LONG BREAK.";
            handleSessionCompletion();
        } else {
            currentStatusTimer = FOCUS;
            qDebug() << "Focus Skipped. Starting SHORT BREAK.";
            handleSessionCompletion();
        }
    }
    else if (currentStatusTimer == SHORT_BREAK){
        currentStatusTimer = SHORT_BREAK;
        qDebug() << "Break Skipped. Starting a new Pomodoro session.";
        handleSessionCompletion();
    }else{
        currentStatusTimer = LONG_BREAK;
        qDebug() << "Break Skipped. Starting a new Pomodoro session.";
        handleSessionCompletion();
    }
}


void MainWindow::updatePomodoroDuration(int newTime)
{
    defaultPomodoroDuration = newTime;

    if (currentStatusTimer == IDLE){
        timeRemaining = defaultPomodoroDuration * 60; // Atualiza o tempo total
        ui->labelTimer->setText(formatTime(timeRemaining)); // Atualiza a label
    }
}
void MainWindow::updateShortBreakDuration(int newTime) { defaultShortBreakDuration = newTime; }
void MainWindow::updateLongBreakDuration(int newTime) { defaultLongBreakDuration = newTime; }
void MainWindow::updatePomodoroRounds(int newRounds) { defaultRoundsSessions = newRounds; }


void MainWindow::startShortBreak()
{
    currentPomodorSessions++;
    currentStatusTimer = SHORT_BREAK;

    timer->stop();

    timeRemaining = defaultShortBreakDuration * 60; // Tempo de descanso curto
    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));

    ui->button_resumePause->setText("Start");
    qDebug() << "Short break Started!";
    qDebug() << "Short_Break Currenstate: " << currentStatusTimer;

    if (timeRemaining < 1){
        //notification
        handleSessionCompletion();
    }

}

void MainWindow::startLongBreak()
{
    timer->stop();
    currentStatusTimer = LONG_BREAK;
    timeRemaining = defaultLongBreakDuration * 60; // Tempo de descanso longo

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));

    ui->button_resumePause->setText("Start");
    qDebug() << "Long break Started!";
    qDebug() << "Long_Break Currenstate: " << currentStatusTimer;

    if (timeRemaining < 1){
        //notification
        handleSessionCompletion();
    }
}


// void MainWindow::setTimerDefaults(int pomodoroDuration, int shortBreak, int longBreak, int sessionsBeforeLong)
// {
// defaultPomodoroDuration = pomodoroDuration;
// defaultShortBreakDuration = shortBreak;
// defaultLongBreakDuration = longBreak;
// defaultRoundsSessions = sessionsBeforeLong;
// }


void MainWindow::handleSessionCompletion()
{
    timeRemaining = 0;
    qDebug() << "Sessions Pomodoro Completed: " << currentPomodorSessions;
    qDebug() << "currentStatusTimer: " << currentStatusTimer;


    if (currentStatusTimer == FOCUS){
        if (currentPomodorSessions >= defaultRoundsSessions){
            //currentPomodorSessions > 2 && (currentPomodorSessions -1) % defaultRoundsSessions == 0
            currentPomodorSessions = 0;
            currentStatusTimer = LONG_BREAK;
            qDebug() << "Starting LONG BREAK.";
            startLongBreak();
        }else{
            currentStatusTimer = SHORT_BREAK;
            qDebug() << "Starting SHORT BREAK.";
            startShortBreak();
        }
    }
    else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) {
        currentStatusTimer = FOCUS;
        qDebug() <<  "Starting a new Pomodoro session.";
        timeRemaining = defaultPomodoroDuration * 60;
        ui->labelTimer->setText(formatTime(timeRemaining));
        ui->button_resumePause->setText("Start");
    }
    else{
        qDebug() << "else do if do handless";
    }

}



QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

