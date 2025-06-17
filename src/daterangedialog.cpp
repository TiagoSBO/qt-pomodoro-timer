#include "daterangedialog.h"
#include "ui_daterangedialog.h"
#include <QDialogButtonBox>

DateRangeDialog::DateRangeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DateRangeDialog)
{
    ui->setupUi(this);

    this->setFixedSize(230, 150);


    ui->dateEdit_start->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit_end->setDisplayFormat("dd/MM/yyyy");

    ui->dateEdit_start->setCalendarPopup(true);
    ui->dateEdit_end->setCalendarPopup(true);

    ui->dateEdit_start->setDate(QDate::currentDate());
    ui->dateEdit_end->setDate(QDate::currentDate().addDays(+2));

    connect(ui->button_confirm, &QPushButton::clicked, this, &QDialog::accept);
}

DateRangeDialog::~DateRangeDialog()
{
    delete ui;
}

QDate DateRangeDialog::startDate() const {
    return ui->dateEdit_start->date();
}

QDate DateRangeDialog::endDate() const {
    return ui->dateEdit_end->date();
}
