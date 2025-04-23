#include "systemtrayiconhandler.h"

SystemTrayiconHandler::SystemTrayiconHandler(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    trayMenu = new QMenu();
    minimizeAction = new QAction("Minimize window", this);
    restoreAction = new QAction("Restore window", this);
    quitAction = new QAction("Quit", this);

    connect(minimizeAction, &QAction::triggered, this, &SystemTrayiconHandler::minimizeRequested);
    connect(restoreAction, &QAction::triggered, this, &SystemTrayiconHandler::restoreRequested);
    connect(quitAction, &QAction::triggered, this, &QApplication::quit, Qt::QueuedConnection);

    buildTrayIconMenu();
    buildTrayIcon();
    this->show();
}

SystemTrayiconHandler::~SystemTrayiconHandler(){}

void SystemTrayiconHandler::buildTrayIcon()
{
    this->setIcon(QIcon(":/icons/assets/icons/Pomodoro Icon.png"));
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

