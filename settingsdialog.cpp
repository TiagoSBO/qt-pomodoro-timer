#include "settingsdialog.h"


Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    connect(ui->setTimeDuration, QOverload<int>::of(&QSpinBox::valueChanged), this, &Settings::emitTimeChanged);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::emitTimeChanged()
{
    emit valueChanged(ui->setTimeDuration->value());
}



