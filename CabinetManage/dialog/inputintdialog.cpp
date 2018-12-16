#include "inputintdialog.h"
#include "ui_inputintdialog.h"

#include <QPainter>
#include <QPixmap>

InputIntDialog::InputIntDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::InputIntDialog)
{
    ui->setupUi(this);

    m_value = 0;
    max = 2147483647;
    min = -2147483647;

    ui->lineEdit->setText(QString::number(m_value));
    ui->lineEdit->setValidator(new QIntValidator(0, 1000, this));
}

InputIntDialog::~InputIntDialog()
{
    delete ui;
}

int InputIntDialog::getInteger(QWidget *parent, QString text, int value, int min, int max)
{
    InputIntDialog dialog(parent);
    dialog.setText(text);
    dialog.setValue(value);
    dialog.setMinValue(min);
    dialog.setMaxValue(max);
    dialog.exec();
    return dialog.value();
}




/**
 ********************************************************************
 * public slots:
 ********************************************************************
*/

void InputIntDialog::setValue(int value)
{
    if(value > max || value < min) return;

    this->m_value = value;
    ui->lineEdit->setText(QString::number(value));
}

void InputIntDialog::setText(QString text)
{
    ui->label->setText(text);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void InputIntDialog::on_subtract_clicked()
{
    setValue(m_value - 1);
}

void InputIntDialog::on_add_clicked()
{
    setValue(m_value + 1);
}

void InputIntDialog::on_ok_clicked()
{
    this->accept();
}

void InputIntDialog::on_lineEdit_textChanged(const QString &arg1)
{
    m_value = arg1.toInt();
}
