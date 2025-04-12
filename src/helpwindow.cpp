#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HelpWindow)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &HelpWindow::close);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
