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

Updater::Updater(QObject *parent) : QObject(parent) {}

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
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        QString latestVersion = obj["version"].toString();
        QString changelog = obj["changelog"].toString();
        QString url = obj["url"].toString();

        qDebug() << "Status Code: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Error: " << reply->errorString();
        qDebug() << "Raw response: " << QString(response);

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
    return latestVersion > currentVersion;
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
