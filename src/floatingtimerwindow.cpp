#include "floatingtimerwindow.h"
#include "ui_floatingtimerwindow.h"
#include <QMessageBox>
#include <qshortcut.h>

FloatingTimerWindow::FloatingTimerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FloatingTimerWindow)
{
    ui->setupUi(this);
    setWindowModality(Qt::NonModal);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+0"), this);
    connect(shortcut, &QShortcut::activated, this, [this]() {
        emit requestMainWindowShow();
        this->hide();
    });
}

FloatingTimerWindow::~FloatingTimerWindow()
{
    delete ui;
}

void FloatingTimerWindow::setTimeText(const QString &text)
{
    ui->labelTimer->setText(text);
}

void FloatingTimerWindow::setBackgroundColor(const QColor &color)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, color);
    setAutoFillBackground(true);
    setPalette(pal);
    update();
}


void FloatingTimerWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void FloatingTimerWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit requestMainWindowShow();
    this->hide();
}

void FloatingTimerWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void FloatingTimerWindow::updateTimeDisplay(const QString &time)
{
    ui->labelTimer->setText(time);
}

void FloatingTimerWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}
