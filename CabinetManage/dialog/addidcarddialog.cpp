#include "addidcarddialog.h"
#include "ui_addidcarddialog.h"

AddIdCardDialog::AddIdCardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddIdCardDialog)
{
    ui->setupUi(this);
}

AddIdCardDialog::~AddIdCardDialog()
{
    delete ui;
}
