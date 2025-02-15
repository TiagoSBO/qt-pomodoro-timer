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
    if (timeRemaining > 0) {
        timeRemaining--;
        ui->labelTimer->setText(formatTime(timeRemaining));
    }else {
        timer->stop();
        currentStatusTimer = FINISHED; // Sessão finalizada
        handleSessionCompletion(); // Inicia a próxima etapa após finalizar a sessão
    }
}

void MainWindow::btton_startResume_clicked()
{
    if (currentStatusTimer == IDLE || currentStatusTimer == PAUSED) {
        timer->start(1000);
        currentStatusTimer = RUNNING;
        ui->button_skip->show();
        ui->button_resumePause->setText("Pause");
    }else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK){
        timer->start(1000);
        currentStatusTimer = RUNNING;
        ui->button_skip->show();
        ui->button_resumePause->setText("Pause");
    } else if (currentStatusTimer == RUNNING) {
        timer->stop();
        currentStatusTimer = PAUSED;
        ui->button_skip->hide();
        ui->button_resumePause->setText("Resume");
    }
}

void MainWindow::btton_reset_clicked() {

    timer->stop(); // Para o temporizador antes de redefinir

    // Define o tempo correto com base no estado atual
    if (currentStatusTimer == RUNNING || currentStatusTimer == PAUSED) {
        timeRemaining = defaultPomodoroDuration * 60;
        qDebug() << "currentStatusTimer do Reset: " << currentStatusTimer;
        qDebug() << "Button reset cliclado no FOCUS";
        if (currentStatusTimer == SHORT_BREAK) {
            timeRemaining = defaultShortBreakDuration * 60;
            qDebug() << "Button reset cliclado no SHORT BREAK";
        }
         else if (currentStatusTimer == LONG_BREAK) {
            timeRemaining = defaultLongBreakDuration * 60;
        }
    }else{
        qDebug() << "ELSE DO IF";
    }

    // Atualiza a interface com o tempo resetado
    ui->labelTimer->setText(formatTime(timeRemaining));
    ui->button_resumePause->setText("Start");
    ui->button_skip->hide();

    // Mantém o estado atual, mas para o timer
    currentStatusTimer = IDLE;
}


void MainWindow::btton_settings_clicked()
{
    settingsScreen->setPomodorDuration(defaultPomodoroDuration); //Configura o valor inicial;
    settingsScreen->setPomodoroRounds(defaultRoundsSessions);

    settingsScreen->exec();
}

void MainWindow::btton_skip_clicked(){
    timer->stop();

    // 🔴 Corrigindo a lógica do estado antes de chamar handleSessionCompletion()
    if (currentStatusTimer == RUNNING ) {
        currentStatusTimer = FINISHED;  // Se estiver no Pomodoro, termina a sessão
    } else if (currentStatusTimer == SHORT_BREAK && currentStatusTimer == RUNNING) {
        currentStatusTimer = FINISHED;  // Se estiver no descanso, pula para o próximo Pomodoro
    } else if (currentStatusTimer == LONG_BREAK  &&  currentStatusTimer == RUNNING){
        currentStatusTimer = FINISHED;
    }
    handleSessionCompletion();
}

// Duração inicial do Timer
void MainWindow::updatePomodoroDuration(int newTime)
{
    defaultPomodoroDuration = newTime; // Atualiza a duração em minutos

    if (currentStatusTimer == IDLE || currentStatusTimer == FINISHED){
        timeRemaining = defaultPomodoroDuration * 60;

        // Atualiza o rótulo do temporizador com o novo valor
        ui->labelTimer->setText(formatTime(defaultPomodoroDuration));
    }
}

void MainWindow::updateShortBreakDuration(int newTime) { defaultShortBreakDuration = newTime; }
void MainWindow::updateLongBreakDuration(int newTime) { defaultLongBreakDuration = newTime; }
void MainWindow::updatePomodoroRounds(int newRounds) { defaultRoundsSessions = newRounds; }


void MainWindow::startPomodoroSession() {
    timer->stop();
    currentStatusTimer = IDLE;
    timeRemaining = defaultPomodoroDuration * 60;
    ui->labelTimer->setText(formatTime(timeRemaining));
    ui->button_resumePause->setText("Start");
    // currentStatusTimer = RUNNING;

    qDebug() << "Pomodoro Session Started!";
}

void MainWindow::startShortBreak()
{
    timer->stop();
    currentStatusTimer = SHORT_BREAK;

    timeRemaining = defaultShortBreakDuration * 60; // Tempo de descanso curto

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));

    ui->button_resumePause->setText("Start");
    qDebug() << "Short_Break Currenstate: " << currentStatusTimer;
    qDebug() << "Short break Started!";

}

void MainWindow::startLongBreak()
{
    timer->stop();
    currentStatusTimer = LONG_BREAK;
    timeRemaining = defaultLongBreakDuration * 60; // Tempo de descanso longo

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));

    ui->button_resumePause->setText("Start");

    currentPomodorSessions = 0; // Reseta o contador de sessões
    qDebug() << "Short break Started!";

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
    qDebug() << "Handling session completion. Current state: " << currentStatusTimer;
    qDebug() << "Sessions Pomodoro Completed: " <<  currentPomodorSessions;

    if (currentStatusTimer == FINISHED || currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK || currentStatusTimer == IDLE) {
        // 🔴 Se está no Short Break ou Long Break e apertou Skip, precisa iniciar Pomodoro
        if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) {
            qDebug() << "Break Skipped. Starting a new Pomodoro session.";
            currentStatusTimer = IDLE; // Para garantir que o Pomodoro inicie corretamente
            startPomodoroSession();
            return; // Sai da função para evitar execuções duplas
        }
        // 🔴 Se está no Pomodoro e terminou ou pulou, iniciar um descanso
        currentPomodorSessions++;

        if (currentPomodorSessions < defaultRoundsSessions) {
            qDebug() << "Starting Short Break";
            currentStatusTimer = SHORT_BREAK; // 🔴 Atualiza o estado antes de chamar a função
            startShortBreak();
        } else {
            qDebug() << "Starting Long Break";
            currentPomodorSessions = 0;
            currentStatusTimer = LONG_BREAK; // 🔴 Atualiza o estado antes de chamar a função
            startLongBreak();
        }
    }
}


QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

