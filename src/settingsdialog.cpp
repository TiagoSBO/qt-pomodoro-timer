#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>
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

    //QSettings
    QSettings settings("QtPomodoro-Timer", "QtPomodoroApp");

    int pomodoro = settings.value("pomodoroDuration", 25).toInt();
    int shortBreak = settings.value("shortBreakDuration", 5).toInt();
    int longBreak = settings.value("longBreakDuration", 15).toInt();
    int rounds = settings.value("pomodoroRounds", 4).toInt();

    int volume = settings.value("volume", 100).toInt();
    int soundIndex = settings.value("soundIndex", 0).toInt();
    bool alertEnabled = settings.value("soundAlertEnabled", true).toBool();

    setPomodorDuration(pomodoro);
    setShortBreakDuration(shortBreak);
    setLongBreakDuration(longBreak);
    setPomodoroRounds(rounds);

    ui->volume_slider->setValue(volume);
    ui->combBox_Alarm_sound->setCurrentIndex(soundIndex);
    ui->checkBox_sound_alert->setChecked(alertEnabled);
}


Settings::~Settings()
{
    //QSettings
    QSettings settings("QtPomodoro-Timer", "QtPomodoroApp");

    settings.setValue("pomodoroDuration", getPomodoroDuration());
    settings.setValue("shortBreakDuration", getShortBreakDuration());
    settings.setValue("longBreakDuration", getLongBreakDuration());
    settings.setValue("pomodoroRounds", getPomodoroRounds());
    settings.setValue("volume", ui->volume_slider->value());
    settings.setValue("soundIndex", ui->combBox_Alarm_sound->currentIndex());
    settings.setValue("soundAlertEnabled", ui->checkBox_sound_alert->isChecked());

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
        //ToolTip
        ui->boxSetTimerDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
        ui->boxSetShortDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
        ui->boxSetLongDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
        ui->boxSetIntervalDuration->setToolTip("You can only change the Timer values ​​when the timer is paused or reset");
    } else {
        ui->boxSetTimerDuration->setToolTip("");
        ui->boxSetShortDuration->setToolTip("");
        ui->boxSetLongDuration->setToolTip("");
        ui->boxSetIntervalDuration->setToolTip("");
    }
}
//Emits signals to change value, if the spinboxes have their values ​​changed.
void Settings::emitTimeValueChanged()
{
    QSpinBox *senderSpinBox = qobject_cast<QSpinBox*>(sender());
    if (!senderSpinBox) return;

    QSettings settings("QtPomodoro-Timer", "QtPomodoroApp");

    if (senderSpinBox == ui->boxSetTimerDuration) {
        int value = ui->boxSetTimerDuration->value();
        settings.setValue("pomodoroDuration", value);
        emit pomodoroDurationChanged(value);
    }
    else if (senderSpinBox == ui->boxSetShortDuration) {
        int value = ui->boxSetShortDuration->value();
        settings.setValue("shortBreakDuration", value);
        emit shortBreakDurationChanged(value);
    }
    else if (senderSpinBox == ui->boxSetLongDuration) {
        int value = ui->boxSetLongDuration->value();
        settings.setValue("longBreakDuration", value);
        emit longBreakDurationChanged(value);
    }
    else if (senderSpinBox == ui->boxSetIntervalDuration) {
        int value = ui->boxSetIntervalDuration->value();
        settings.setValue("pomodoroRounds", value);
        emit pomodoroRoundsChanged(value);
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
    emit soundEnabledChanged(checked);

}

