#include "timer.h"
#include <QApplication>
#include "singleapplication.h"
#include "updater/updater.h"
#include <QStyleFactory>
#include <QSysInfo>
#include <QFile>
#include <QTextStream>

//Read QSS files
QString loadStyleSheet(const QStringList &qssPaths) {
    QString finalStyleSheet;
    for (const QString &path : qssPaths) {
        QFile file;
        if (QFile::exists(path)) {
            file.setFileName(path);  // External fallback
        } else {
            file.setFileName(":/styles/" + QFileInfo(path).fileName());  // Internal fallback
        }
        if (file.open(QFile::ReadOnly)) {
            finalStyleSheet += QString::fromUtf8(file.readAll()) + "\n";
        }
    }
    return finalStyleSheet;
}

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv, true);

    if (app.isSecondary()) {
        app.sendMessage("raise");
        return 0;
    }

    QString osProductVersion = QSysInfo::productVersion();
    if (osProductVersion.startsWith("10")) {
        app.setStyle(QStyleFactory::create("Fusion"));
    }

    QStringList styleSheets = {
        ":/styles/styles/timer.qss",
        ":/styles/styles/sessionlogs.qss",
        ":/styles/styles/settingsdialog.qss",
        ":/styles/styles/helpwindow.qss"
    };

    QString combinedStyle = loadStyleSheet(styleSheets);
    app.setStyleSheet(combinedStyle);

    // VERIFICADOR DE ATUALIZAÇÃO
    Updater updater;
    updater.checkForUpdates();

    MainWindow w;

    QObject::connect(&app, &SingleApplication::receivedMessage, &w, [&](quint32, QByteArray) {
        if (w.isMinimized()){
            w.showNormal();
            w.activateWindow();
            w.raise();
        }
    });

    w.show();

    return app.exec();
}
