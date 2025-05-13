#ifndef FLOATINGTIMERWINDOW_H
#define FLOATINGTIMERWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QMouseEvent>
#include <QPalette>

class QLabel;

namespace Ui {
class FloatingTimerWindow;
}

class FloatingTimerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FloatingTimerWindow(QWidget *parent = nullptr);
    ~FloatingTimerWindow();

    void setTimeText(const QString &text);
    void setBackgroundColor(const QColor &color);

signals:
    void requestMainWindowShow();

public slots:
    void updateTimeDisplay(const QString &time);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::FloatingTimerWindow *ui;

    QPoint dragPosition;
};

#endif // FLOATINGTIMERWINDOW_H
