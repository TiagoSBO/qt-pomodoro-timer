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
    , defaultRoundsSessions(4) //Long break interval
    , timeRemaining(defaultPomodoroDuration * 60)
{
    ui->setupUi(this);

    //Configurações iniciais
    ui->labelTimer->setText(QString("%1:%2")
        .arg(defaultPomodoroDuration, 2, 10, QChar('0'))
        .arg(0, 2, 10, QChar('0')));

    ui->button_skip->hide();

    settingsScreen = new Settings(this);

    //Conexão dos botões do Settings
    connect(settingsScreen, &Settings::pomodoroDurationChanged, this, &MainWindow::updatePomodoroDuration);
    connect(settingsScreen, &Settings::shortBreakDurationChanged, this, &MainWindow::updateShortBreakDuration);
    connect(settingsScreen, &Settings::longBreakDurationChanged, this, &MainWindow::updateLongBreakDuration);
    connect(settingsScreen, &Settings::pomodoroRoundsChanged, this, &MainWindow::updatePomodoroRounds);

    //Criação do Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::defaultTimerFocus);

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

void MainWindow::defaultTimerFocus()
{
    if (currentStatusTimer == RUNNING) {
        if (timeRemaining > 0) {
            timeRemaining--;

            // Atualiza a interface no formato "MM:SS"
            int minutes = timeRemaining / 60;
            int seconds = timeRemaining % 60;

            ui->labelTimer->setText(QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0')));
        }

        if (timeRemaining == 0) {
            timer->stop();
            currentStatusTimer = FINISHED; // Sessão finalizada

            handleSessionCompletion(); // Inicia a próxima etapa após finalizar a sessão
        }
    }
}

void MainWindow::btton_startResume_clicked()
{
    if (currentStatusTimer == IDLE || currentStatusTimer == PAUSED ||
        currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) {

        if (currentStatusTimer == IDLE) {
            // Configura o tempo restante no início (em segundos) apenas para o estado IDLE
            timeRemaining = defaultPomodoroDuration * 60;
        }
        // Começa a contagem decrescente
        timer->start(1000);
        currentStatusTimer = RUNNING;
        ui->button_skip->show();

        // Atualiza o texto dos botões
        ui->button_resumePause->setText("Pause");
    } else if (currentStatusTimer == RUNNING) {
        timer->stop();
        currentStatusTimer = PAUSED;
        ui->button_skip->hide();
        // Atualiza o texto dos botões
        ui->button_resumePause->setText("Resume");
    }
}

void MainWindow::btton_reset_clicked()
{
    timeRemaining = defaultPomodoroDuration * 60;
    currentStatusTimer = IDLE;
    ui->button_skip->hide();

    ui->labelTimer->setText(QString("%1:%2")
                                .arg(defaultPomodoroDuration, 2, 10, QChar('0'))
                                .arg(0, 2, 10, QChar('0')));

    ui->button_resumePause->setText("Start");
}


void MainWindow::btton_settings_clicked()
{
    settingsScreen->setPomodorDuration(defaultPomodoroDuration); //Configura o valor inicial;
    settingsScreen->setPomodoroRounds(defaultRoundsSessions);

    settingsScreen->exec();
}

void MainWindow::btton_skip_clicked()
{
    qDebug() << "Clickado";
    if (currentStatusTimer == RUNNING && defaultRoundsSessions <= 4){
        currentStatusTimer = FINISHED;
        handleSessionCompletion();
    }
}

// Duração inicial do Timer
void MainWindow::updatePomodoroDuration(int newTime)
{
    defaultPomodoroDuration = newTime; // Atualiza a duração em minutos

    if (currentStatusTimer == IDLE || currentStatusTimer == FINISHED){
        timeRemaining = defaultPomodoroDuration * 60;

        // Atualiza o rótulo do temporizador com o novo valor
        ui->labelTimer->setText(QString("%1:%2")
                                    .arg(defaultPomodoroDuration, 2, 10, QChar('0'))
                                    .arg(0, 2, 10, QChar('0')));
    }
}

void MainWindow::updateShortBreakDuration(int newTime)
{
    defaultShortBreakDuration = newTime;
}

void MainWindow::updateLongBreakDuration(int newTime)
{
    defaultLongBreakDuration = newTime;
}

//Intervalo de sessões de pomodoro até chegar ao descanso longo
void MainWindow::updatePomodoroRounds(int newRounds)
{
    defaultRoundsSessions = newRounds;
}

void MainWindow::startPomodoroSession() {
    timer->stop(); // Para o temporizador antes de iniciar uma nova sessão
    currentStatusTimer = RUNNING;
    timeRemaining = defaultPomodoroDuration * 60; // Configura o tempo padrão para o Pomodoro

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString("%1:%2")
                                .arg(timeRemaining / 60, 2, 10, QChar('0'))
                                .arg(0, 2, 10, QChar('0')));

    // Ajusta os botões
    ui->button_resumePause->setText("Pause");
    timer->start(1000); // Inicia o temporizador
    qDebug() << "Pomodoro Session Started!";
}

void MainWindow::startShortBreak()
{
    timer->stop();
    currentStatusTimer = SHORT_BREAK;
    timeRemaining = defaultShortBreakDuration * 60; // Tempo de descanso curto

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString("%1:%2")
                                .arg(timeRemaining / 60, 2, 10, QChar('0'))
                                .arg(0, 2, 10, QChar('0')));

    ui->button_resumePause->setText("Start");
    timer->start(1000); // Inicia o temporizador
    qDebug() << "Short break Started!";

}

void MainWindow::startLongBreak()
{
    timer->stop();
    currentStatusTimer = LONG_BREAK;
    timeRemaining = defaultLongBreakDuration * 60; // Tempo de descanso longo

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString("%1:%2")
                                .arg(timeRemaining / 60, 2, 10, QChar('0'))
                                .arg(0, 2, 10, QChar('0')));

    ui->button_resumePause->setText("Start");

    timer->start(1000); // Inicia o temporizador
    currentPomodorSessions = 0;  // Reseta o contador de sessões
    qDebug() << "Short break Started!";

}


// void MainWindow::setTimerDefaults(int pomodoroDuration, int shortBreak, int longBreak, int sessionsBeforeLong)
// {
//     defaultPomodoroDuration = pomodoroDuration;
//     defaultShortBreakDuration = shortBreak;
//     defaultLongBreakDuration = longBreak;
//     defaultRoundsSessions = sessionsBeforeLong;
// }

void MainWindow::handleSessionCompletion()
{
    qDebug() << "Handling session completion. Current state: " << currentStatusTimer;
    // Só deve entrar aqui se a sessão foi finalizada e o estado for FINISHED
    if (currentStatusTimer == FINISHED) {
        currentPomodorSessions++;
        // Decida se é um descanso curto ou longo baseado no número de sessões
        if (currentPomodorSessions < defaultRoundsSessions) {
            // Inicia um descanso curto
            qDebug() << "Starting Short Break";
            startShortBreak();  // Inicia o descanso curto
        } else {
            // Após 4 sessões, começa um descanso longo
            qDebug() << "Starting Long Break";
            currentPomodorSessions = 0;
            startLongBreak();  // Inicia o descanso longo
        }
    }
    // Caso o descanso tenha sido completado, reinicia o Pomodoro
    else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) {
        qDebug() << "Break Complete. Starting New Pomodoro Session.";
        startPomodoroSession();  // Inicia uma nova sessão de Pomodoro
    }
}
