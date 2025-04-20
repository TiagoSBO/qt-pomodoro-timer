#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QMessageBox>

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
    , soundManager(new SoundManager(this))
    , audioOutput(new QAudioOutput(this))
{
    ui->setupUi(this);

    connect(ui->boxSetTimerDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetShortDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetLongDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->boxSetIntervalDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeValueChanged);
    connect(ui->btton_playAlarm, &QPushButton::clicked, this, &Settings::setAlarm_sound);
    connect(ui->combBox_Alarm_sound, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::setAlarm_sound);
    connect(ui->volume_slider, &QSlider::valueChanged, this, &::Settings::volume_slider_valueChanged);
    connect(ui->checkBox_sound_alert, &QCheckBox::toggled, this, &::Settings::soundAlertToggled);

    ui->label_setVolumeValue->setText(QString::number(ui->volume_slider->value()));
    soundManager->setSoundEnabled(ui->checkBox_sound_alert->isChecked());
}

Settings::~Settings()
{
    delete ui;
    delete soundManager;
    delete audioOutput;
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

//SpinBoxes configs (alerts)
void Settings::setSpinboxesEnabled(bool enabled)
{
    ui->boxSetTimerDuration->setEnabled(enabled);
    ui->boxSetShortDuration->setEnabled(enabled);
    ui->boxSetLongDuration->setEnabled(enabled);
    ui->boxSetIntervalDuration->setEnabled(enabled);
    if (!enabled) {
        // Aplica uma dica de "não disponível" com um QToolTip
        ui->boxSetTimerDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
        ui->boxSetShortDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
        ui->boxSetLongDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
        ui->boxSetIntervalDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
    } else {
        // Remove as dicas de aviso
        ui->boxSetTimerDuration->setToolTip("");
        ui->boxSetShortDuration->setToolTip("");
        ui->boxSetLongDuration->setToolTip("");
        ui->boxSetIntervalDuration->setToolTip("");
    }
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

void Settings::setAlarm_sound()
{
    int index = ui->combBox_Alarm_sound->currentIndex();
    QString namesound = ui->combBox_Alarm_sound->currentText();
    qDebug() << "Name sound:" << namesound << "Index: " << index;
    soundManager->playSound(index);

    emit notificationSoundChanged(index);
}

void Settings::volume_slider_valueChanged(int value)
{
    ui->volume_slider->setRange(0, 100);
    value = ui->volume_slider->value();
    ui->label_setVolumeValue->setText(QString::number(value));
    soundManager->setVolume(value);

    emit volumeChanged(value);
}

void Settings::soundAlertToggled(bool checked)
{
    soundManager->setSoundEnabled(checked);
    emit soundEnabledChanged(checked);  // <--- notifica para o MainWindow

}

