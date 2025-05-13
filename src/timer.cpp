#include "timer.h"
#include "src/ui/ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QShortcut>

#define APP_VERSION "1.0.9"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, currentStatusTimer(FOCUS)
, sessionsDoneCount(0)
, currentPomodorSessions(0)
, defaultPomodoroDuration(25)
, defaultShortBreakDuration(5)
, defaultLongBreakDuration(15)
, defaultRoundsSessions(4)
, timeRemaining(defaultPomodoroDuration * 60)
, timerStarted(false)
, floatingTimerWindow(nullptr)
{
    ui->setupUi(this);

    //Initial Sets Updates
    updateStyleBasedOnState();
    ui->labelTimer->setText(formatTime(timeRemaining));

    //Exit App
    connect(ui->actionExit, &QAction::triggered, this, [this]() {
        isExiting = true;
        QCoreApplication::quit();
    });

    //Version
    QLabel *versionLabel = new QLabel(this);
    versionLabel->setObjectName("versionLabel");
    versionLabel->setText(QStringLiteral("Version %1").arg(APP_VERSION));
    statusBar()->addWidget(versionLabel);

    //System Tray Icon
    systemTrayIcon = new SystemTrayiconHandler(this);
    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::restoreFromTrayIcon);
    connect(systemTrayIcon, &SystemTrayiconHandler::minimizeRequested, this, &MainWindow::hide);
    connect(systemTrayIcon, &SystemTrayiconHandler::restoreRequested, this, [this]() {
        this->showNormal();
        this->raise();
        this->activateWindow();
    });
    connect(systemTrayIcon, &SystemTrayiconHandler::quitRequested, this, [this]() {
        isExiting = true;
        QCoreApplication::quit();
    });

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

    QAction *deleteTableData = new QAction("Clear table data 🗑", this);
    deleteTableData->setData(ClearTableContent);

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

    //Connect Timer - MainWindow buttons
    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::btton_settings_clicked);
    connect(ui->button_resumePause, &QPushButton::clicked, this, &MainWindow::btton_startResume_clicked);
    connect(ui->button_reset, &QPushButton::clicked, this, &MainWindow::btton_reset_clicked);
    connect(ui->button_skip, &QPushButton::clicked, this, &MainWindow::btton_skip_clicked);

    //floating Window
    ui->labelTimer->installEventFilter(this);
    connect(systemTrayIcon, &SystemTrayiconHandler::restoreFloatingWindow, this, &MainWindow::handleRestoreFloatingWindow);
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+0"), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::toggleFloatingWindow);

    ui->labelTimer->setAttribute(Qt::WA_Hover);
    ui->labelTimer->setMouseTracking(true);
    ui->labelTimer->setCursor(Qt::PointingHandCursor);
    ui->labelTimer->setEnabled(true);
    ui->labelTimer->setTextInteractionFlags(Qt::NoTextInteraction);

    //Help window
    helpWindow = new HelpWindow(this);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::openHelpDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settingsScreen;
    delete sessionLogs;
    delete floatingTimerWindow;
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
        handleSessionCompletion();
    }
    updateTimerDisplay();
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
        timerStarted = true;
        ui->button_resumePause->setText("Pause");
        qDebug() << "Start/Resume clicked / CurrentState -> " << currentStatusTimer;
    }
    updateTimerDisplay();
    running = !running;
}

void MainWindow::btton_reset_clicked()
{
    // Restores the time based on the current session
    qDebug() << "Reset button clicked: " << currentStatusTimer;
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
    updateTimerDisplay();
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

    updateTimerDisplay();
}

void MainWindow::btton_skip_clicked()
{
    if (currentStatusTimer == FOCUS) {
        if (timerStarted || running == false || running != false) {
            currentPomodorSessions++;
            int focusTimeSpent = (defaultPomodoroDuration * 60 - timeRemaining);
            QString formattedFocusTime = formatTime(focusTimeSpent);
            QString endTime = QDateTime::currentDateTime().toString("HH:mm");
            updateTimerDisplay();

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
    timeRemaining = defaultPomodoroDuration * 60;

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

//Function responsible for managing focus and rest
void MainWindow::handleSessionCompletion()
{
    if (currentStatusTimer == FOCUS){
        currentPomodorSessions++;
        qDebug() << "Sessions Pomodoro Completed --->>> " << currentPomodorSessions;
        // Sound Notifcation
        soundManager.playSound(selectedISoundIndex);

        int focusTimeSpent = defaultPomodoroDuration * 60;  // Total session duration
        QString formattedFocusTime = formatTime(focusTimeSpent);
        QString endTime = QDateTime::currentDateTime().toString("HH:mm");
        updateTimerDisplay();

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
    else if (currentStatusTimer == SHORT_BREAK || currentStatusTimer == LONG_BREAK){
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

    // Applies style to the timer layout
    ui->layout_timer->setProperty("focusState", state);
    ui->layout_timer->style()->unpolish(ui->layout_timer);
    ui->layout_timer->style()->polish(ui->layout_timer);

    // Applies style to the table
    ui->tableSessionLogs->horizontalHeader()->setObjectName("sessionHeader");
    ui->tableSessionLogs->horizontalHeader()->setProperty("focusState", state);
    auto *header = ui->tableSessionLogs->horizontalHeader();
    header->setProperty("focusState", state);
    header->style()->unpolish(header);
    header->style()->polish(header);

    header->updateGeometry();
    header->viewport()->update();
    header->repaint();

    // Applies style to the layout of the timer buttons
    ui->layout_buttonsTimer->setProperty("focusState", state);
    ui->layout_buttonsTimer->style()->unpolish(ui->layout_buttonsTimer);
    ui->layout_buttonsTimer->style()->polish(ui->layout_buttonsTimer);

    // Here iterate through the children and dynamically updates the style
    const auto &children = ui->layout_buttonsTimer->findChildren<QPushButton*>();
    for (QPushButton *btn : children) {
        btn->setProperty("focusState", state);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
        btn->update();
    }

    ui->tableSessionLogs->viewport()->update();
    ui->tableSessionLogs->repaint();

    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}


//Table Label - Total focus time
void MainWindow::updateTotalFocusTime()
{
    int totalSeconds = sessionLogs->getAccumulatedFocusSeconds(); // Gets the total focus time in seconds

    // Calculate hours and minutes from total seconds
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;

    if (hours > 0) {
        ui->label_setTotalFocus->setText(QString("%1h %2min").arg(hours).arg(minutes, 2, 10, QChar('0')));
    } else {
        ui->label_setTotalFocus->setText(QString("%1 min").arg(minutes));
    }
}

void MainWindow::updateTimerDisplay()
{
    QString formattedTime = formatTime(timeRemaining);
    ui->labelTimer->setText(formattedTime);
    emit timerUpdated(formattedTime);
}

//Config table Button - Action-> Clear table data
void MainWindow::button_configTable_clicked(){

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action) return;

    QVariant data = action->data();
    if (!data.isValid()) return;

    int value = data.toInt();

    switch (value) {
    case ClearTableContent: {
        QMessageBox::StandardButton clearTableAnswer = QMessageBox::question(this, "Confirmation",
            "Are you sure you want to clear the table data?",
            QMessageBox::Yes | QMessageBox::No);

        if (clearTableAnswer == QMessageBox::Yes) {
            sessionLogs->clearTableOnly();
            updateTotalFocusTime();
        }
        break;
    }
    default:
        qDebug() << "Unknown action";
    }
}

QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;

    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

// Mainwindow floating timer
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->labelTimer) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            createFloatingTimerWindowIfNeeded();

            floatingTimerWindow->updateTimeDisplay(formatTime(timeRemaining));
            floatingTimerWindow->show();
            floatingTimerWindow->raise();
            floatingTimerWindow->activateWindow();

            this->hide();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

//Shortcut
void MainWindow::toggleFloatingWindow()
{
    if (!floatingTimerWindow) {
        createFloatingTimerWindowIfNeeded();
    }

    if (floatingTimerWindow->isVisible()) {
        floatingTimerWindow->hide();
        this->show();
        this->raise();
        this->activateWindow();
    } else {
        floatingTimerWindow->updateTimeDisplay(formatTime(timeRemaining));
        floatingTimerWindow->show();
        floatingTimerWindow->raise();
        floatingTimerWindow->activateWindow();
        this->hide();
    }
}

void MainWindow::showFromFloating()
{
    this->show();
    this->raise();
    this->activateWindow();
}


void MainWindow::handleRestoreFloatingWindow()
{
    if (floatingTimerWindow) {
        // Centraliza no centro da tela principal
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        int x = screenGeometry.center().x() - floatingTimerWindow->width() / 2;
        int y = screenGeometry.center().y() - floatingTimerWindow->height() / 2;
        floatingTimerWindow->move(x, y);
        floatingTimerWindow->show();
        floatingTimerWindow->raise();
        floatingTimerWindow->activateWindow();
    }
}

void MainWindow::createFloatingTimerWindowIfNeeded()
{
    if (!floatingTimerWindow) {
        floatingTimerWindow = new FloatingTimerWindow();
        connect(this, &MainWindow::timerUpdated, floatingTimerWindow, &FloatingTimerWindow::updateTimeDisplay);
        connect(floatingTimerWindow, &FloatingTimerWindow::requestMainWindowShow, this, &MainWindow::showFromFloating);
    }
}

// Dialog Window
void MainWindow::openHelpDialog()
{
    helpWindow->exec();
}

//TrayIcon Action Functions
void MainWindow::restoreFromTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        this->showNormal();
        this->raise();
        this->activateWindow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    static bool notifiedOnce = false;

    if (isExiting) {
        event->accept();
        return;
    }

    if (systemTrayIcon && systemTrayIcon->isVisible()) {
        this->hide();
        event->ignore();

        if (!notifiedOnce) {
            systemTrayIcon->showMessage("Pomodoro Timer",
                "The application has been minimized to the notification area.",
                QSystemTrayIcon::Information, 1000);
            notifiedOnce = true;
        }
    }
}
