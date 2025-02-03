#include "settingsdialog.h"
#include "ui_settingsdialog.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    connect(ui->boxSetTimerDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetIntervalDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
}

Settings::~Settings()
{
    delete ui;
}
void Settings::setPomodorDuration(int duration)
{
    ui->boxSetTimerDuration->setValue(duration);
}

void Settings::setPomodoroRounds(int interval)
{
    ui->boxSetIntervalDuration->setValue(interval);
}

int Settings::getPomodoroDuration() const
{
    return ui->boxSetTimerDuration->value();
}

int Settings::getPomodoroRounds() const
{
    return ui->boxSetIntervalDuration->value();
}

void Settings::emitTimeValueChanged()
{
    QSpinBox *senderSpinBox = qobject_cast<QSpinBox*>(sender());
    if (!senderSpinBox) return;

    if (senderSpinBox == ui->boxSetTimerDuration)
        emit pomodoroDurationChanged(ui->boxSetTimerDuration->value());
    else if (senderSpinBox == ui->boxSetIntervalDuration)
        emit pomodoroRoundsChanged(ui->boxSetIntervalDuration->value());
    //adicionar + aqui
}



