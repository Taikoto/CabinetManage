#include "alcoholdialog.h"
#include "ui_alcoholdialog.h"

AlcoholDialog::AlcoholDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::AlcoholDialog)
{
    ui->setupUi(this);

    arm4418 = Arm4418::instance();

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateAlcohol()));
    timer.start(500);
}

AlcoholDialog::~AlcoholDialog()
{
    delete ui;
}

void AlcoholDialog::updateAlcohol()
{
    double alcohol = arm4418->getAlcoholic();
    ui->alcohol->setText(QString::number(alcohol, 'f', 2));

    if (alcohol >= 20)
        ui->infoLabel->setText(tr("You drank the wine!"));
    else if (alcohol >= 80)
        ui->infoLabel->setText(tr("You are drunk!"));
}
