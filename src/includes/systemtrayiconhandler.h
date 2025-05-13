#ifndef SYSTEMTRAYICONHANDLER_H
#define SYSTEMTRAYICONHANDLER_H

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class SystemTrayiconHandler : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit SystemTrayiconHandler(QWidget *parent = nullptr);
    ~SystemTrayiconHandler();

private:
    QMenu *trayMenu;
    QAction *restoreAction, *minimizeAction, *quitAction, *restoreFloatWindow;

    void buildTrayIcon();
    void buildTrayIconMenu();

signals:
    void minimizeRequested();
    void restoreRequested();
    void restoreFloatingWindow();
    void quitRequested();

};

#endif
