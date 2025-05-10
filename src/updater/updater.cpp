#include "updater/updater.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QStandardPaths>
#include <QProcess>
#include <QMessageBox>
#include <QApplication>
#include <QUrl>
#include <QTimer>
#include <app_version.h>

Updater::Updater(QObject *parent) : QObject(parent) {}

const QString Updater::currentVersion = QString(APP_VERSION);
QString Updater::updateUrl = "https://qtpomodoro-timer.netlify.app/update_server/version.json";

void Updater::checkForUpdates() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(updateUrl)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(nullptr, "Failed", "Error checking for updates.");
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            QMessageBox::critical(nullptr, "Update Error", "Failed to parse update information:\n" + parseError.errorString());
            qDebug() << "JSON parse error:" << parseError.errorString();
            qDebug() << "Raw response:" << QString(response);
            reply->deleteLater();
            return;
        }

        if (!doc.isObject()) {
            QMessageBox::critical(nullptr, "Update Error", "Update information is invalid (not a JSON object).");
            reply->deleteLater();
            return;
        }
        QJsonObject obj = doc.object();

        QString latestVersion = obj["version"].toString();
        QString changelog = obj["changelog"].toString();
        QString url = obj["url"].toString();

        qDebug() << "Status Code: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Error: " << reply->errorString();
        qDebug() << "Raw response: " << QString(response);

        qDebug() << "App current version:" << currentVersion;
        qDebug() << "Latest version from server:" << latestVersion;

        if (isNewVersionAvailable(latestVersion)) {
            int ret = QMessageBox::information(nullptr, "Update Avaliable",
                                               QString("New version %1 avaliable!\n\n%2\n\nDo you want to update now?")
                                                   .arg(latestVersion, changelog),
                                               QMessageBox::Yes | QMessageBox::No);

            if (ret == QMessageBox::Yes) {
                downloadInstaller(url);
            }
        }

        reply->deleteLater();
    });
}

bool Updater::isNewVersionAvailable(const QString &latestVersion) {
    const QStringList latestParts = latestVersion.split(".");
    const QStringList currentParts = currentVersion.split(".");

    for (int i = 0; i < std::min(latestParts.size(), currentParts.size()); ++i) {
        int latest = latestParts[i].toInt();
        int current = currentParts[i].toInt();

        if (latest > current)
            return true;
        else if (latest < current)
            return false;
    }

    return latestParts.size() > currentParts.size();
}


void Updater::downloadInstaller(const QString &installerUrl) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(installerUrl)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, installerUrl]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(nullptr, "Failed", "Error downloading installer.");
            reply->deleteLater();
            return;
        }

        QByteArray data = reply->readAll();
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString fileName = QUrl(installerUrl).fileName();
        QString installerFile = tempPath + "/" + fileName;

        QFile file(installerFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
        } else {
            QMessageBox::critical(nullptr, "Failed", "Unable to save the installer.");
            reply->deleteLater();
            return;
        }

        QMessageBox::information(nullptr, "Download completed", "Installer saved in:\n" + installerFile);
        QMessageBox::information(nullptr, "Updating", "The application will close to apply the update.");

        QProcess::startDetached(installerFile);
        QApplication::quit();

        reply->deleteLater();
    });
}
