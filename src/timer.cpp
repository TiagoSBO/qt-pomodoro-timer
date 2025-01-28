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
    , defaultPomodorSessions(0)
    , defaultPomodoroDuration(25)
    , defaultShortBreakDuration(5)
    , defaultLongBreakDuration(15)
    , defaultSessionsLongBreak(4)
    , timeRemaining(defaultPomodoroDuration * 60)
{
    ui->setupUi(this);

    ui->labelTimer->setText(QString("%1:%2")
        .arg(defaultPomodoroDuration, 2, 10, QChar('0'))
        .arg(0, 2, 10, QChar('0')));

    Settings *settings = new Settings(this);
    connect(settings, &Settings::valueChanged, this, &MainWindow::updateTimeValue);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::defaultTimerFocus);

    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::btton_settings_clicked);
    connect(ui->button_startResume, &QPushButton::clicked, this, &MainWindow::btton_startResume_clicked);
    connect(ui->button_stopDone, &QPushButton::clicked, this, &MainWindow::btton_stopDone_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
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

            ui->button_stopDone->setText("Done"); // Muda o texto para "Done"
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

        // Atualiza o texto dos botões
        ui->button_startResume->setText("Pause");
        ui->button_stopDone ->setText("Stop");
    } else if (currentStatusTimer == RUNNING) {
        timer->stop();
        currentStatusTimer = PAUSED;

        // Atualiza o texto dos botões
        ui->button_startResume->setText("Resume");
        ui->button_stopDone->setText("Done");
    }
}

void MainWindow::btton_stopDone_clicked()
{
    qDebug() << "Stop/Done clicked. Current state: " << currentStatusTimer;
    qDebug() << "Button Text: " << ui->button_stopDone->text(); // Log do texto do botão

    if (ui->button_stopDone->text() == "Stop") {
        // Se o texto for "Stop" (quando o timer está pausado), apenas para o temporizador
        timer->stop();
        currentStatusTimer = IDLE; // Reseta o timer
        ui->button_stopDone->setText("Stop"); // Texto do botão de volta para "Stop"
        resetTimer(); // Reseta o contador de tempo
    }
    else if (ui->button_stopDone->text() == "Done") {
        // Se o texto for "Done" (quando o Pomodoro ou o descanso terminou)
        // Conta mais uma sessão e inicia o descanso (curto ou longo, dependendo do número de Pomodoros)
        timer->stop(); // Garantir que o timer seja parado antes de fazer a transição
        currentStatusTimer = FINISHED;  // Marca a sessão como finalizada
        handleSessionCompletion(); // Processa a conclusão da sessão
    }
}

void MainWindow::btton_settings_clicked()
{
    Settings settingsScreen;
    settingsScreen.setPomodorDuration(defaultPomodoroDuration); //Configura o valor inicial;

    connect(&settingsScreen, &Settings::valueChanged, this, &MainWindow::updateTimeValue);
    settingsScreen.exec();
}

void MainWindow::updateTimeValue(int newTime)
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

void MainWindow::startPomodoroSession()
{
    timer->stop(); // Para o temporizador antes de iniciar uma nova sessão
    currentStatusTimer = RUNNING;
    timeRemaining = defaultPomodoroDuration * 60; // Configura o tempo padrão para o Pomodoro

    // Atualiza o rótulo do temporizador
    ui->labelTimer->setText(QString("%1:%2")
                                .arg(timeRemaining / 60, 2, 10, QChar('0'))
                                .arg(0, 2, 10, QChar('0')));

    // Ajusta os botões
    ui->button_startResume->setText("Pause");
    ui->button_stopDone->setText("Stop");

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

    ui->button_startResume->setText("Start");
    ui->button_stopDone->setText("Done");

    timer->start(1000); // Inicia o temporizador
    qDebug() << "Short Break Started!";
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

    ui->button_startResume->setText("Start");
    ui->button_stopDone->setText("Done");

    timer->start(1000); // Inicia o temporizador
    qDebug() << "Long Break Started!";
}


void MainWindow::resetTimer()
{
    timeRemaining = defaultPomodoroDuration * 60;
    currentStatusTimer = IDLE;

    ui->labelTimer->setText(QString("%1:%2")
                                .arg(defaultPomodoroDuration, 2, 10, QChar('0'))
                                .arg(0, 2, 10, QChar('0')));

    ui->button_startResume->setText("Start");
    ui->button_stopDone->setText("Stop");
}

void MainWindow::handleSessionCompletion()
{
    qDebug() << "Handling session completion. Current state: " << currentStatusTimer;

    // Só deve entrar aqui se a sessão foi finalizada e o estado for FINISHED
    if (currentStatusTimer == FINISHED) {
        defaultPomodorSessions++;  // Conta uma sessão de Pomodoro concluída

        // Decida se é um descanso curto ou longo baseado no número de sessões
        if (defaultPomodorSessions < defaultSessionsLongBreak) {
            // Inicia um descanso curto
            qDebug() << "Pomodoro Complete. Starting Short Break. Sessions: " << defaultPomodorSessions;
            startShortBreak();  // Inicia o descanso curto
        } else {
            // Após 4 sessões, começa um descanso longo
            qDebug() << "4 Pomodoro Sessions Complete. Starting Long Break.";
            defaultPomodorSessions = 0;  // Reseta o contador de sessões
            startLongBreak();  // Inicia o descanso longo
        }
    }
    // Caso o descanso tenha sido completado, reinicia o Pomodoro
    else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) {
        qDebug() << "Break Complete. Starting New Pomodoro Session.";
        startPomodoroSession();  // Inicia uma nova sessão de Pomodoro
    }
}
