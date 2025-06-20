#include "updater/updater.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
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
    QString html;
    // Inicia um container com alinhamento à esquerda e alguns estilos básicos.
    html.append("<div style='text-align:left; font-family:Arial, sans-serif; font-size:12pt;'>");

    QStringList lines = md.split("\n");
    bool inList = false;

    for (const QString &rawLine : std::as_const(lines)) {
        QString line = rawLine.trimmed();

        if (line.startsWith("### ")) {
            // Se estiver dentro de uma lista, fecha a lista.
            if (inList) {
                html.append("</ul>");
                inList = false;
            }
            QString headerText = line.mid(4).trimmed();
            // Cria um cabeçalho com margem adequada.
            html.append("<h3 style='margin:10px 0;'>" + headerText + "</h3>");
        }
        else if (line.startsWith("- ")) {
            // Se ainda não está em um bloco de lista, inicia um.
            if (!inList) {
                html.append("<ul style='margin:0 0 10px 20px;'>");
                inList = true;
            }
            QString listItem = line.mid(2).trimmed();
            // Converte negrito: **texto** para <b>texto</b>.
            listItem.replace(QRegularExpression(R"(\*\*(.+?)\*\*)"), "<b>\\1</b>");
            html.append("<li>" + listItem + "</li>");
        }
        else if (!line.isEmpty()) {
            // Se estiver em uma lista e encontrar outro tipo de linha, fecha a lista.
            if (inList) {
                html.append("</ul>");
                inList = false;
            }
            // Converte negrito dentro de parágrafos.
            line.replace(QRegularExpression(R"(\*\*(.+?)\*\*)"), "<b>\\1</b>");
            html.append("<p style='margin:10px 0;'>" + line + "</p>");
        }
    }
    if (inList) {
        html.append("</ul>");
    }

    html.append("</div>"); // Fecha o container.

    return html;
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
        QString changelogHtml = markdownToHtml(obj["changelog"].toString());
        QString url = obj["url"].toString();

        qDebug() << "Versão atual:" << currentVersion;
        qDebug() << "Nova versão:" << latestVersion;

        if (isNewVersionAvailable(latestVersion)) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("NEW UPDATE!");
            msgBox.setTextFormat(Qt::RichText);
            msgBox.setText(QString(
                "<b> 🚀 New version %1 available!</b><%2<br><b>Do you want to install it now?</b>"
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
