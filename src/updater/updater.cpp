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
#include <qapplication.h>

Updater::Updater(QObject *parent) : QObject(parent) {}

void Updater::checkForUpdates() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(updateUrl)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        QString latestVersion = obj["version"].toString();
        QString changelog = obj["changelog"].toString();
        QString url = obj["url"].toString();

        if (isNewVersionAvailable(latestVersion)) {
            int ret = QMessageBox::information(nullptr, "Atualização disponível",
                                               QString("Nova versão %1 disponível!\n\n%2\n\nDeseja atualizar agora?")
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
    return latestVersion > currentVersion; // simples comparação
}

void Updater::downloadInstaller(const QString &installerUrl) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(installerUrl)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(nullptr, "Erro", "Erro ao baixar instalador.");
            reply->deleteLater();
            return;
        }

        QByteArray data = reply->readAll();
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString installerFile = tempPath + "/PomodoroSetup_Update.exe";

        QFile file(installerFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
        }

        QMessageBox::information(nullptr, "Atualizando", "A aplicação será fechada para aplicar a atualização.");
        QProcess::startDetached(installerFile); // executa instalador
        QApplication::quit();

        reply->deleteLater();
    });
}
