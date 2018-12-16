#include "datetimedialog.h"
#include "ui_datetimedialog.h"

#include <QDebug>

DateTimeDialog::DateTimeDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::DateTimeDialog)
{
    ui->setupUi(this);

    ui->hour->setValidator(new QIntValidator(0, 23, this));
    ui->minute->setValidator(new QIntValidator(0, 59, this));

    ui->calendarWidget->setGridVisible(true);//显示网格
}

DateTimeDialog::~DateTimeDialog()
{
    delete ui;
}

QDateTime DateTimeDialog::getDateTime(QWidget *parent, QDateTime date, QDate mindate)
{
    DateTimeDialog dialog(parent);

    dialog.setTime(date);
    dialog.setMinDate(mindate);

    dialog.exec();
    return dialog.time;
}

void DateTimeDialog::setTime(const QDateTime &time)
{
    this->time = time;
    ui->calendarWidget->setSelectedDate(this->time.date());
    ui->hour->setText(time.toString("h"));
    ui->minute->setText(time.toString("m"));
}

void DateTimeDialog::setHour(int hour)
{
    QTime tt = time.time();

    if( hour >= 24 ) hour = 0;
    else if( hour <= -1 ) hour = 23;

    tt.setHMS(hour, tt.minute(), tt.second());
    time.setTime(tt);

    setTime(time);
}

void DateTimeDialog::setMinute(int minute)
{
    QTime tt = time.time();

    if( minute >= 60 ) minute = 0;
    else if( minute <= -1 ) minute = 59;

    tt.setHMS(tt.hour(), minute, tt.second());
    time.setTime(tt);

    setTime(time);
}

void DateTimeDialog::setMinDate(const QDate &mindate)
{
    ui->calendarWidget->setMinimumDate(mindate);
}



void DateTimeDialog::on_hAdd_clicked()
{
    setHour(time.time().hour() + 1);
}

void DateTimeDialog::on_hSubtract_clicked()
{
    setHour(time.time().hour() - 1);
}

void DateTimeDialog::on_mAdd_clicked()
{
    setMinute(time.time().minute() + 1);
}

void DateTimeDialog::on_mSubtract_clicked()
{
    setMinute(time.time().minute() - 1);
}

void DateTimeDialog::on_hour_textChanged(const QString &arg1)
{
    setHour(arg1.toInt());
}

void DateTimeDialog::on_minute_textChanged(const QString &arg1)
{
    setMinute(arg1.toInt());
}

void DateTimeDialog::on_calendarWidget_clicked(const QDate &date)
{
    time.setDate(date);
}
