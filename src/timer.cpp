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
    , customDuration(25)
    , timeRemaining(customDuration * 60)
{
    ui->setupUi(this);

    ui->labelTimer->setText(QString("%1:%2")
        .arg(customDuration, 2, 10, QChar('0'))
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

        // Finaliza o temporizador ao atingir 0
        if (timeRemaining == 0) {
            timer->stop();
            currentStatusTimer = FINISHED;

            ui->button_startResume->setText("Start");
            ui->button_stopDone->setText("Done");
        }
    }
}

void MainWindow::btton_startResume_clicked()
{
    if (currentStatusTimer == IDLE || currentStatusTimer == PAUSED) {
        if (currentStatusTimer == IDLE) {
            // Configura o tempo restante no início (em segundos)
            timeRemaining = customDuration * 60;
        }

        timer->start(1000); // Começa a contagem decrescente
        currentStatusTimer = RUNNING;

        ui->button_startResume->setText("Pause");
        ui->button_stopDone->setText("Stop");
    } else if (currentStatusTimer == RUNNING) {
        timer->stop(); // Pausa o temporizador
        currentStatusTimer = PAUSED;

        ui->button_startResume->setText("Resume");
        ui->button_stopDone->setText("Done");
    }
}

void MainWindow::btton_stopDone_clicked()
{
    if (currentStatusTimer == RUNNING || currentStatusTimer == PAUSED) {
        timer->stop();
        currentStatusTimer = IDLE; // Reseta o status do temporizador

        // Reinicia o tempo restante com base no valor inicial (customDuration)
        timeRemaining = customDuration * 60;

        // Atualiza o rótulo do temporizador no formato "MM:SS"
        ui->labelTimer->setText(QString("%1:%2")
            .arg(customDuration, 2, 10, QChar('0'))
            .arg(0, 2, 10, QChar('0')));

        ui->button_startResume->setText("Start");
        ui->button_stopDone->setText("Stop");
    }
}
void MainWindow::btton_settings_clicked()
{
    Settings settingsScreen;
    connect(&settingsScreen, &Settings::valueChanged, this, &MainWindow::updateTimeValue);
    settingsScreen.exec();
}

void MainWindow::updateTimeValue(int newTime)
{
    customDuration = newTime; // Atualiza a duração em minutos
    timeRemaining = customDuration * 60; // Atualiza o tempo restante

    // Atualiza o rótulo do temporizador com o novo valor
    ui->labelTimer->setText(QString("%1:%2")
        .arg(customDuration, 2, 10, QChar('0'))
        .arg(0, 2, 10, QChar('0')));
}

