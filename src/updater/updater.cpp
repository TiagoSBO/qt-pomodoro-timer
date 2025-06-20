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
#include <QDebug>
#include <app_version.h>

Updater::Updater(QObject *parent) : QObject(parent) {}

const QString Updater::currentVersion = QString(APP_VERSION);
QString Updater::updateUrl = "https://qtpomodoro-timer.netlify.app/update_server/version.json";

QString Updater::markdownToHtml(QString md)
{
    md.replace("\n", "<br>");
    md.replace("### ", "<h3>");
    md.replace("**", "<b>");
    md.replace("- ", "&#8226; ");

    if (md.contains("<h3>")) {
        int end = md.indexOf("<br>");
        if (end != -1) {
            md.insert(end, "</h3>");
        } else {
            md.append("</h3>");
        }
    }
    return md;
}

void Updater::checkForUpdates() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(updateUrl)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(nullptr, "Erro", "Erro ao verificar atualizações.");
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            QMessageBox::critical(nullptr, "Erro de Atualização", "Erro ao analisar JSON:\n" + parseError.errorString());
            reply->deleteLater();
            return;
        }

        if (!doc.isObject()) {
            QMessageBox::critical(nullptr, "Erro de Atualização", "Resposta do servidor inválida.");
            reply->deleteLater();
            return;
        }

        QJsonObject obj = doc.object();
        QString latestVersion = obj["version"].toString();
        QString changelogHtml = obj["changelog"].toString();
        QString url = obj["url"].toString();

        qDebug() << "Versão atual:" << currentVersion;
        qDebug() << "Nova versão:" << latestVersion;

        if (isNewVersionAvailable(latestVersion)) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("NEW UPDATE!");
            msgBox.setTextFormat(Qt::RichText);
            msgBox.setText(QString(
                "<b>New version %1 available!</b><br><br>%2<br><br><b>Do you want to install it now?</b>"
            ).arg(latestVersion, changelogHtml));

            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            int ret = msgBox.exec();

            if (ret == QMessageBox::Yes) {
                downloadInstaller(url);
            }
        }

        reply->deleteLater();
    });
}


bool Updater::isNewVersionAvailable(const QString &latestVersion) {
    QStringList latestParts = latestVersion.split(".");
    QStringList currentParts = currentVersion.split(".");

    for (int i = 0; i < std::min(latestParts.size(), currentParts.size()); ++i) {
        int latest = latestParts[i].toInt();
        int current = currentParts[i].toInt();

        if (latest > current) return true;
        else if (latest < current) return false;
    }

    return latestParts.size() > currentParts.size();
}

void Updater::downloadInstaller(const QString &installerUrl) {
    QMessageBox *progressBox = new QMessageBox(QMessageBox::Information,
                                               "Downloading update",
                                               "Update is downloading...",
                                               QMessageBox::NoButton);
    progressBox->setAttribute(Qt::WA_DeleteOnClose);
    progressBox->show();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(installerUrl)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, installerUrl, progressBox]() {
        progressBox->hide();
        progressBox->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(nullptr, "Error", "Error downloading installer.");
            reply->deleteLater();
            return;
        }

        QByteArray data = reply->readAll();
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString fileName = QUrl(installerUrl).fileName();
        QString installerFile = tempPath + "/" + fileName;

        QFile file(installerFile);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not save installer.");
            reply->deleteLater();
            return;
        }

        file.write(data);
        file.close();

        QMessageBox::information(nullptr, "Update!", "The application will be closed to apply the update.");
        QProcess::startDetached(installerFile);
        QApplication::quit();

        reply->deleteLater();
    });
}
