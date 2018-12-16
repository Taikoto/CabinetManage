#ifndef RFIDDIALOG_H
#define RFIDDIALOG_H

#include "dialog.h"
#include "serialrfid.h"

namespace Ui {
class RfidDialog;
}

class RfidDialog : public Dialog
{
    Q_OBJECT

public:
    explicit RfidDialog(QWidget *parent = 0);
    ~RfidDialog();

    static QString getCardId(QWidget *parent = 0);

private slots:
    void finished(const SerialRfid::DevData data);
    void on_cancel_clicked();

private:
    Ui::RfidDialog *ui;
    SerialRfid *sRfid;
    QString id;
};

#endif // RFIDDIALOG_H
