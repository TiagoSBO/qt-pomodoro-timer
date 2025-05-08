#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QWidget>

class Updater : public QObject
{
    Q_OBJECT

public:
    explicit Updater(QObject *parent = nullptr);
    void checkForUpdates();

private:
    const QString updateUrl = "https://seusite.netlify.app/update.json";
    const QString currentVersion = "1.0.0";

    void downloadInstaller(const QString &installerUrl);
    bool isNewVersionAvailable(const QString &latestVersion);
};

#endif // UPDATER_H
