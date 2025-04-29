#include "systemtrayiconhandler.h"

SystemTrayiconHandler::SystemTrayiconHandler(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    trayMenu = new QMenu();
    restoreAction = new QAction("Open window", this);
    minimizeAction = new QAction("Minimize window", this);
    quitAction = new QAction("Quit", this);

    connect(minimizeAction, &QAction::triggered, this, &SystemTrayiconHandler::minimizeRequested);
    connect(restoreAction, &QAction::triggered, this, &SystemTrayiconHandler::restoreRequested);
    connect(quitAction, &QAction::triggered, this, [this]() {
        emit quitRequested();
    });
    buildTrayIconMenu();
    buildTrayIcon();
    this->show();
}

SystemTrayiconHandler::~SystemTrayiconHandler(){}

void SystemTrayiconHandler::buildTrayIcon()
{
    this->setIcon(QIcon(":/icons/assets/icons/pomodoroIcon.png"));
    this->setToolTip("Pomodoro Timer");
    this->setContextMenu(trayMenu);
}

void SystemTrayiconHandler::buildTrayIconMenu()
{
    trayMenu->addAction(minimizeAction);
    trayMenu->addSeparator();
    trayMenu->addAction(restoreAction);

    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
}

