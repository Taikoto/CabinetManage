#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include "dialog.h"
#include <QCalendarWidget>
#include <QDateTime>
#include <QTime>
#include <QPainter>

namespace Ui {
class DateTimeDialog;
}

class DateTimeDialog : public Dialog
{
    Q_OBJECT

public:
    explicit DateTimeDialog(QWidget *parent = 0);
    ~DateTimeDialog();

    static QDateTime getDateTime(QWidget *parent = 0,
                                 QDateTime date = QDateTime::currentDateTime(),
                                 QDate mindate = QDate::currentDate());

public slots:
    void setTime(const QDateTime &time);
    void setHour(int hour);
    void setMinute(int minute);
    void setMinDate(const QDate &mindate);

private:
    Ui::DateTimeDialog *ui;
    QDateTime time;

private slots:
    void on_hAdd_clicked();
    void on_hSubtract_clicked();
    void on_mAdd_clicked();
    void on_mSubtract_clicked();
    void on_hour_textChanged(const QString &arg1);
    void on_minute_textChanged(const QString &arg1);
    void on_calendarWidget_clicked(const QDate &date);
};

#endif // DATETIMEDIALOG_H
