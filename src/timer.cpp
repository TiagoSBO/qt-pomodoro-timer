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
    if (currentStatusTimer != RUNNING) {
        return; // Garante que apenas estados RUNNING sejam processados
    }

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
            currentStatusTimer = FINISHED;

            // ui->button_startResume->setText("Start");
            ui->button_stopDone->setText("Done");

            handleSessionCompletion();
        }

        // if (timeRemaining == 0 || currentStatusTimer == PAUSED) {
        //     ui->button_stopDone->setText("Done");
        // }

        if (timeRemaining == 0) {
            timer->stop(); // Para o temporizador ao atingir 0
            currentStatusTimer = FINISHED;

            ui->button_stopDone->setText("Done");
            handleSessionCompletion(); // Transição para o próximo estado
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
    // if (currentStatusTimer == RUNNING || currentStatusTimer == PAUSED) {
    //     // Para o timer e redefine para o estado inicial
    //     timer->stop();
    //     currentStatusTimer = IDLE;
    //     resetTimer();
    // } else if (currentStatusTimer == FINISHED) {
    //     // Confirmação do "Done" para finalizar o estado atual
    //     handleSessionCompletion();
    // }
    if (currentStatusTimer == RUNNING || currentStatusTimer == PAUSED) {
        // Para o temporizador e redefine para o estado inicial
        timer->stop();
        currentStatusTimer = IDLE;
        resetTimer();
    }
    else if (currentStatusTimer == FINISHED) {
        // Conta o Pomodoro finalizado e inicia o próximo estado
        defaultPomodorSessions++;
        qDebug() << "Pomodoro Session Completed: " << defaultPomodorSessions;

        handleSessionCompletion(); // Alterna para descanso ou novo Pomodoro
    }
    else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK) {
        // Se estiver no descanso, iniciar o próximo Pomodoro
        startPomodorSession();
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

void MainWindow::startPomodorSession()
{
    timer->stop(); // Certifique-se de que o temporizador está parado
    currentStatusTimer = RUNNING;
    timeRemaining = defaultPomodoroDuration * 60; // Reinicia o tempo do Pomodoro

    // Atualiza a interface
    ui->labelTimer->setText(QString("%1:%2")
        .arg(timeRemaining / 60, 2, 10, QChar('0'))
        .arg(0, 2, 10, QChar('0')));

    ui->button_startResume->setText("Pause");
    ui->button_stopDone->setText("Stop");

    timer->start(1000); // Inicia o temporizador
    qDebug() << "Pomodoro Session Started!";
}

void MainWindow::startBreak(TimerState breaktype)
{
    // {
    //     if (breaktype == SHORT_BREAK) {
    //         currentStatusTimer = SHORT_BREAK;
    //         timeRemaining = defaultShortBreakDuration * 60; // Tempo do descanso curto
    //     } else if (breaktype == LONG_BREAK) {
    //         currentStatusTimer = LONG_BREAK;
    //         timeRemaining = defaultLongBreakDuration * 60; // Tempo do descanso longo
    //     }

    //     // Atualiza o rótulo do temporizador
    //     ui->labelTimer->setText(QString("%1:%2")
    //         .arg(timeRemaining / 60, 2, 10, QChar('0'))
    //         .arg(0, 2, 10, QChar('0')));

    //     // Ajusta os botões para o novo estado
    //     ui->button_startResume->setText("Start");
    //     ui->button_stopDone->setText("Stop");
    // }

    if (breaktype == SHORT_BREAK) {
        currentStatusTimer = SHORT_BREAK;
        timeRemaining = defaultShortBreakDuration * 60; // Tempo do descanso curto
        qDebug() << "Short Break Started!";
    }
    else if (breaktype == LONG_BREAK) {
        currentStatusTimer = LONG_BREAK;
        timeRemaining = defaultLongBreakDuration * 60; // Tempo do descanso longo
        qDebug() << "Long Break Started!";
    }

    // Atualiza o temporizador e os botões
    ui->labelTimer->setText(QString("%1:%2")
        .arg(timeRemaining / 60, 2, 10, QChar('0'))
        .arg(0, 2, 10, QChar('0')));

    ui->button_startResume->setText("Start");
    ui->button_stopDone->setText("Done");
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
    // if (currentStatusTimer == FINISHED) {
    //     if (defaultPomodorSessions < defaultSessionsLongBreak) {
    //         // Alterna para um descanso curto após cada pomodoro
    //         defaultPomodorSessions++;
    //         qDebug() << "Sessions +1: " << defaultPomodorSessions;
    //         startBreak(SHORT_BREAK);
    //     } else if (defaultPomodorSessions == defaultSessionsLongBreak) {
    //         // Após 4 pomodoros, inicia o descanso longo
    //         qDebug() << "4 Sessions completed " << defaultPomodorSessions;
    //         defaultPomodorSessions = 0; // Reseta o contador de pomodoros
    //         startBreak(LONG_BREAK);
    //     }
    // }

    if (defaultPomodorSessions < defaultSessionsLongBreak) {
        // Se menos de 4 sessões, inicia um descanso curto
        qDebug() << "Starting Short Break. Sessions: " << defaultPomodorSessions;
        startBreak(SHORT_BREAK);
    }
    else if (defaultPomodorSessions == defaultSessionsLongBreak) {
        // Após 4 sessões de Pomodoro, inicia o descanso longo
        qDebug() << "Starting Long Break. Sessions: " << defaultPomodorSessions;
        defaultPomodorSessions = 0; // Reseta o contador de sessões
        startBreak(LONG_BREAK);
    }
}

