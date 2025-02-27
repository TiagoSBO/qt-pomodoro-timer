#include "settingsdialog.h"
#include "ui_settingsdialog.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    connect(ui->boxSetTimerDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetShortDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetLongDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetIntervalDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    // connect(ui->bttnResetTimerDefaults, &QPushButton::clicked, this, &Settings::resetTimerToDefaults);
}

Settings::~Settings()
{
    delete ui;
}

// SETS
void Settings::setPomodorDuration(int duration)
{
    ui->boxSetTimerDuration->setValue(duration);
}

void Settings::setShortBreakDuration(int duration)
{
    ui->boxSetShortDuration->setValue(duration);
}

void Settings::setLongBreakDuration(int duration)
{
    ui->boxSetLongDuration->setValue(duration);
}

void Settings::setPomodoroRounds(int interval)
{
    ui->boxSetIntervalDuration->setValue(interval);
}

//GETS
int Settings::getPomodoroDuration() const
{
    return ui->boxSetTimerDuration->value();
}

int Settings::getShortBreakDuration() const
{
    return ui->boxSetShortDuration->value();
}

int Settings::getLongBreakDuration() const
{
    return ui->boxSetLongDuration->value();
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
    else if(senderSpinBox == ui->boxSetShortDuration)
        emit shortBreakDurationChanged(ui->boxSetShortDuration->value());
    else if(senderSpinBox == ui->boxSetLongDuration)
        emit longBreakDurationChanged(ui->boxSetLongDuration->value());
    else if (senderSpinBox == ui->boxSetIntervalDuration){
        emit pomodoroRoundsChanged(ui->boxSetIntervalDuration->value());
    }
}


// void Settings::resetTimerToDefaults()
// {
//     QList<QSpinBox*> spinBoxes = findChildren<QSpinBox*>();

//     ui->boxSetTimerDuration->setValue(25);
//     ui->boxSetShortDuration->setValue(5);
//     ui->boxSetLongDuration->setValue(0);
//     ui->boxSetIntervalDuration->setValue(4);

// }



