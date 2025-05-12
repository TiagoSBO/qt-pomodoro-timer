#include "includes/minitimerwindow.h"
#include <QtUiTools/QUiLoader>
#include <QtCore/QFile>
#include <QLabel>
#include <QMouseEvent>

MiniTimerWindow::MiniTimerWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QUiLoader loader;
    QFile file(":/files/src/ui/minitimerwindow.ui"); // Use resource path
    file.open(QFile::ReadOnly);
    QWidget *uiWidget = loader.load(&file, this);
    file.close();

    labelTimer->setStyleSheet("color: white; font-size: 32px;");

    if (uiWidget) {
        setCentralWidget(uiWidget);
        labelTimer = findChild<QLabel*>("labelTimer");
        // setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        // setAttribute(Qt::WA_TranslucentBackground);
    } else {
        // Handle error if UI file loading fails
        labelTimer = new QLabel("Error loading UI", this);
        setCentralWidget(labelTimer);
    }
}

MiniTimerWindow::~MiniTimerWindow()
{
    // Destructor
}

void MiniTimerWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked();
        dragging = true;
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }

    Q_UNUSED(event);
    dragging = false;

    QMainWindow::mouseDoubleClickEvent(event); // Call base class implementation
}

