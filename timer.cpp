#include "timer.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QChar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentStatusTimer(IDLE)
    , initialSeconds(0)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    connect(ui->b_startResume, &QPushButton::clicked, this, &MainWindow::btton_startResume_clicked);
    connect(ui->b_stopDone, &QPushButton::clicked, this, &MainWindow::btton_stopDone_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimer()
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
            ui->b_startResume->setText("Start");
        }
    }
}

void MainWindow::btton_startResume_clicked()
{
    if (currentStatusTimer == IDLE || currentStatusTimer == PAUSED){
        timer->start(1000);
        currentStatusTimer = RUNNING;
        ui->b_startResume->setText("Pause");
    }else if(currentStatusTimer == RUNNING){
        timer->stop();
        currentStatusTimer = PAUSED;
        ui->b_startResume->setText("Resume");
    }

}

void MainWindow::btton_stopDone_clicked()
{
    if (currentStatusTimer == RUNNING){
        timer->stop();
        currentStatusTimer = IDLE;
        initialSeconds = 0;
        ui->labelTimer->setText("00:00");
        ui->b_startResume->setText("Start");
    }

    //QUando pausado -> setText("Done");
}







