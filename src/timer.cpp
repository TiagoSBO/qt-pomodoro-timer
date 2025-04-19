#include "timer.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#define APP_VERSION "1.0.0"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, currentStatusTimer(FOCUS)
, sessionsDoneCount(0)
, currentPomodorSessions(0)
, defaultPomodoroDuration(25)
, defaultShortBreakDuration(5)
, defaultLongBreakDuration(15)
, defaultRoundsSessions(4) //Long break interval
, timeRemaining(defaultPomodoroDuration * 60)
, timerStarted(false)
{
    ui->setupUi(this);

    //Initial Sets Updates
    updateStyleBasedOnState();
    ui->labelTimer->setText(formatTime(timeRemaining));
    //Exit App
    connect(ui->actionExit, &QAction::triggered, this, &QCoreApplication::quit, Qt::QueuedConnection);

    //Version
    QLabel *versionLabel = new QLabel(this);
    versionLabel->setObjectName("versionLabel");
    versionLabel->setText(QStringLiteral("Version %1").arg(APP_VERSION));

    statusBar()->addWidget(versionLabel);

    //Config - Table View
    sessionLogs = new Sessionlogs(ui->tableSessionLogs);
    ui->tableSessionLogs->verticalHeader()->setVisible(false);
    ui->tableSessionLogs->setColumnCount(3);
    ui->tableSessionLogs->setColumnWidth(0, 150);
    QTableWidgetItem *headerItem0 = new QTableWidgetItem(QIcon(":/icons/assets/icons/Pomodoro Icon.png"), "Pomodoros Done");
    QTableWidgetItem *headerItem1 = new QTableWidgetItem(QIcon(":/icons/assets/icons/timer3.png"), "Focus Time");
    QTableWidgetItem *headerItem2 = new QTableWidgetItem(QIcon(":/icons/assets/icons/endtime.png"), "End Time");

    ui->tableSessionLogs->setHorizontalHeaderItem(0, headerItem0);
    ui->tableSessionLogs->setHorizontalHeaderItem(1, headerItem1);
    ui->tableSessionLogs->setHorizontalHeaderItem(2, headerItem2);
    ui->tableSessionLogs->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableSessionLogs->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHeaderView *header = ui->tableSessionLogs->horizontalHeader();
    ui->tableSessionLogs->setColumnWidth(0, 140);
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(2, QHeaderView::Stretch);

    //Config table buttons
    QMenu *menu = new QMenu(this);
    menu->setFocusPolicy(Qt::NoFocus);
    QAction *deleteTableData = new QAction("Clear table data 🗑️", this);
    menu->addAction(deleteTableData);
    ui->button_configTable->setMenu(menu);
    connect(deleteTableData, &QAction::triggered, this, &MainWindow::button_configTable_clicked);

    //Connect Settings buttons
    settingsScreen = new Settings(this);
    connect(settingsScreen, &Settings::pomodoroDurationChanged, this, &MainWindow::updatePomodoroDuration);
    connect(settingsScreen, &Settings::shortBreakDurationChanged, this, &MainWindow::updateShortBreakDuration);
    connect(settingsScreen, &Settings::longBreakDurationChanged, this, &MainWindow::updateLongBreakDuration);
    connect(settingsScreen, &Settings::pomodoroRoundsChanged, this, &MainWindow::updatePomodoroRounds);
    connect(settingsScreen, &Settings::notificationSoundChanged, this, &MainWindow::setAlarmSound);
    connect(settingsScreen, &Settings::volumeChanged, &soundManager, &SoundManager::setVolume);
    connect(settingsScreen, &Settings::soundEnabledChanged, &soundManager, &SoundManager::setSoundEnabled);

    //Timer creation
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerOut);

    //Connect MainWindow buttons
    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::btton_settings_clicked);
    connect(ui->button_resumePause, &QPushButton::clicked, this, &MainWindow::btton_startResume_clicked);
    connect(ui->button_reset, &QPushButton::clicked, this, &MainWindow::btton_reset_clicked);
    connect(ui->button_skip, &QPushButton::clicked, this, &MainWindow::btton_skip_clicked);

    //Help window
    helpWindow = new HelpWindow(this);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::openHelpDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete settingsScreen;
    delete sessionLogs;
}

void MainWindow::setAlarmSound(int index)
{
    selectedISoundIndex = index;
    qDebug() << "Selected sound -> " << selectedISoundIndex;
}

void MainWindow::onTimerOut()
{
    if (timeRemaining > 0) {
            timeRemaining--;
            ui->labelTimer->setText(formatTime(timeRemaining));
        }else {
            timer->stop();
            //notification
            handleSessionCompletion();
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
        timer->start(2);
        timerStarted = true;
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
    if (timer->isActive()) {
        settingsScreen->setSpinboxesEnabled(false);
    } else {
        settingsScreen->setSpinboxesEnabled(true);
    }
    settingsScreen->setPomodorDuration(defaultPomodoroDuration);
    settingsScreen->setShortBreakDuration(defaultShortBreakDuration);
    settingsScreen->setLongBreakDuration(defaultLongBreakDuration);
    settingsScreen->setPomodoroRounds(defaultRoundsSessions);
    settingsScreen->exec();
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

void MainWindow::btton_skip_clicked()
{
    if (currentStatusTimer == FOCUS) {
        if (timerStarted || running == false || running != false) {
            currentPomodorSessions++;
            int focusTimeSpent = (defaultPomodoroDuration * 60 - timeRemaining);
            QString formattedFocusTime = formatTime(focusTimeSpent);
            QString endTime = QDateTime::currentDateTime().toString("HH:mm");

            // Add session to the table
            sessionsDoneCount++;
            sessionLogs->addSession(sessionsDoneCount, formattedFocusTime, endTime);
            updateTotalFocusTime();
            timerStarted = false;
            qDebug() << "Focus session skipped! Time spent: " << formattedFocusTime;
        }

        if (currentPomodorSessions >= defaultRoundsSessions) {
            qDebug() << "-->Focus Skipped. Starting LONG BREAK";
            setSession(LONG_BREAK);
        } else{
            qDebug() << "Focus session skipped! Time spent: " << timeRemaining << " minutes";
            qDebug() << "Sessions SKIPED Pomodoro Completed --->>> " << currentPomodorSessions;
            setSession(SHORT_BREAK);
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

//Sessions pomodoro and rest
void MainWindow::pomodoroSession()
{
    //notification
    currentStatusTimer = FOCUS;
    timer->stop();
    qDebug() << "New Pomodoro session started! / CurrentState -> " << currentStatusTimer;
    // Sound Notifcation
    soundManager.playSound(selectedISoundIndex);

    running = false;
    ui->button_resumePause->setText("Start");
    ui->labelShowState->setText("Time to Focus");
    updateStyleBasedOnState();
    timeRemaining = defaultPomodoroDuration * 60; //defaultPomodoroDuration

    // qDebug() << "Session Count --> " << currentPomodorSessions;
    ui->labelTimer->setText(formatTime(timeRemaining));
}

void MainWindow::startShortBreak()
{
    timer->stop();
    currentStatusTimer = SHORT_BREAK;
    timerStarted = false;
    running = false;

    // Sound Notifcation
    soundManager.playSound(selectedISoundIndex);

    qDebug() << "Short_Break Started / CurrenState -> " << currentStatusTimer;

    ui->button_resumePause->setText("Start");
    ui->labelShowState->setText("Time to Short Rest");
    updateStyleBasedOnState();

    timeRemaining = defaultShortBreakDuration * 60;
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
}

void MainWindow::startLongBreak()
{
    currentStatusTimer = LONG_BREAK;
    timer->stop();
    timerStarted = false;

    // Sound Notifcation
    soundManager.playSound(selectedISoundIndex);
    qDebug() << "Long_Break Started! / CurrentState -> " << currentStatusTimer;

    running = false;
    ui->button_resumePause->setText("Start");
    ui->labelShowState->setText("Time to Long Rest");
    updateStyleBasedOnState();

    currentPomodorSessions = 0;

    timeRemaining = defaultLongBreakDuration * 60;
    ui->labelTimer->setText(QString(formatTime(timeRemaining)));
}

void MainWindow::handleSessionCompletion()
{
    if (currentStatusTimer == FOCUS){
        currentPomodorSessions++;
        qDebug() << "Sessions Pomodoro Completed --->>> " << currentPomodorSessions;
        // Sound Notifcation
        soundManager.playSound(selectedISoundIndex);

        int focusTimeSpent = defaultPomodoroDuration * 60;  // Duração total da sessão
        QString formattedFocusTime = formatTime(focusTimeSpent);
        QString endTime = QDateTime::currentDateTime().toString("HH:mm");

        sessionsDoneCount++;
        sessionLogs->addSession(sessionsDoneCount, formattedFocusTime, endTime);

        updateTotalFocusTime();

        if (currentPomodorSessions >= defaultRoundsSessions){
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
}

void MainWindow::updateStyleBasedOnState()
{
    QString state;

    if (currentStatusTimer == FOCUS) {
        state = "FOCUS";
    } else if (currentStatusTimer == SHORT_BREAK) {
        state = "SHORT_BREAK";
    } else {
        state = "LONG_BREAK";
    }

    // Aplica ao layout do timer
    ui->layout_timer->setProperty("focusState", state);
    ui->layout_timer->style()->unpolish(ui->layout_timer);
    ui->layout_timer->style()->polish(ui->layout_timer);

    // Aplica à tabela
    ui->tableSessionLogs->horizontalHeader()->setProperty("focusState", state);
    ui->tableSessionLogs->horizontalHeader()->style()->unpolish(ui->tableSessionLogs->horizontalHeader());
    ui->tableSessionLogs->horizontalHeader()->style()->polish(ui->tableSessionLogs->horizontalHeader());

    ui->tableSessionLogs->repaint();
}


//Table Label - Total focus time
void MainWindow::updateTotalFocusTime()
{
    int totalSeconds = sessionLogs->getTotalTimeFocus(); // Obtém o tempo total de foco em segundos

    // Calcular as horas e minutos a partir dos segundos totais
    int hours = totalSeconds / 3600;  // Calcula o número de horas
    int minutes = (totalSeconds % 3600) / 60;  // Calcula os minutos restantes

    // Verifica se o tempo total de foco é menor que 1 hora
    if (hours > 0) {
        // Exibe o tempo no formato "Xh:YYmin" se tiver 1 ou mais horas
        ui->label_setTotalFocus->setText(QString("%1h %2min").arg(hours).arg(minutes, 2, 10, QChar('0')));
    } else {
        // Exibe apenas os minutos se o tempo for menor que 1 hora
        ui->label_setTotalFocus->setText(QString("%1 min").arg(minutes));
    }
}

//Table Button - Reset table data
void MainWindow::button_configTable_clicked()
{
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "Confirmation",
        "Are you sure you want to clear the table data?",
        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes) {
        ui->tableSessionLogs->clearContents();
        ui->tableSessionLogs->setRowCount(0);

        qDebug() << "Dados excluídos!";  // Aqui você coloca a lógica para excluir os dados
    } else {
        qDebug() << "Ação cancelada!";
    }
}

QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

void MainWindow::openHelpDialog()
{
    helpWindow->exec();
}

