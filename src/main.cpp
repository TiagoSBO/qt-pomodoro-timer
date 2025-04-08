#include "timer.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("C:/Users/Tiago/Documents/QtCreator_Projects/pomodoro_timer/styles/styles.qss"); // Certifique-se do caminho do arquivo
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Erro ao abrir styles.qss: " << file.errorString();
    } else {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
    }
    file.close();

    MainWindow w;
    w.show();

    return a.exec();
}
