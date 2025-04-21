#include "timer.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

//Read Qsss files
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
    QApplication a(argc, argv);

    QStringList styleSheets = {
        ":/styles/styles/timer.qss",
        ":/styles/styles/sessionlogs.qss",
        ":/styles/styles/settingsdialog.qss",
        ":/styles/styles/helpwindow.qss"
    };

    QString combinedStyle = loadStyleSheet(styleSheets);
    a.setStyleSheet(combinedStyle);

    MainWindow w;
    w.show();

    return a.exec();
}
