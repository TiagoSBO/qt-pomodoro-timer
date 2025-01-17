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
    , initialSeconds(0) //BOXTIMEDURATION
{
    ui->setupUi(this);

    Settings *settings = new Settings(this);
    // connect(settings, &Settings::valueChanged, this, &MainWindow::UpdateTimer)

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
    if (currentStatusTimer == RUNNING){
        initialSeconds++;

        int minutes = initialSeconds / 60;
        int secondsRemaining = initialSeconds % 60;

        ui->labelTimer->setText(QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secondsRemaining, 2, 10, QChar('0')));

        if (initialSeconds == 25 * 60){
            timer->stop();
            currentStatusTimer = FINISHED;
            ui->button_startResume->setText("Start");
        }
    }
}

//TO-DO LATER = REFACTOR
void MainWindow::btton_startResume_clicked()
{
    if (currentStatusTimer == IDLE || currentStatusTimer == PAUSED){
        timer->start(1000);
        currentStatusTimer = RUNNING;
        ui->button_startResume->setText("Pause");
    }else if(currentStatusTimer == RUNNING){
        timer->stop();
        currentStatusTimer = PAUSED;
        ui->button_startResume->setText("Resume");
        ui->button_stopDone->setText("Done");
    }
}

//TO-DO LATER = REFACTOR
void MainWindow::btton_stopDone_clicked()
{
    if (currentStatusTimer == RUNNING || PAUSED){
        timer->stop();
        currentStatusTimer = IDLE;
        initialSeconds = 0;
        ui->labelTimer->setText("00:00");
        ui->button_startResume->setText("Start");
    }
}

void MainWindow::btton_settings_clicked()
{
    Settings settingsScreen;
    settingsScreen.exec();
}


