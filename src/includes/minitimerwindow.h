#ifndef MINITIMERWINDOW_H
#define MINITIMERWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>

class MiniTimerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MiniTimerWindow(QWidget *parent = nullptr);
    ~MiniTimerWindow();

    QLabel *labelTimer; // Made public for easier access from MainWindow, can be changed later if needed
    // minitimerwindow.h
    QPoint dragPosition;
    bool dragging = false;

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
         // Ui::MiniTimerWindow *ui; // Not using a generated ui_*.h file with QUiLoader
};

#endif // MINITIMERWINDOW_H
