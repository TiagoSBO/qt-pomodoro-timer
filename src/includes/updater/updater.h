#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QWidget>
#include <QString>

class Updater : public QObject
{
    Q_OBJECT

public:
    explicit Updater(QObject *parent = nullptr);
    void checkForUpdates();

    static const QString currentVersion;
    static QString updateUrl;

private:
    void downloadInstaller(const QString &installerUrl);
    bool isNewVersionAvailable(const QString &latestVersion);
};

#endif // UPDATER_H
