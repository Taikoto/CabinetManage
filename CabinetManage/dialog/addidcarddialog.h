#ifndef ADDIDCARDDIALOG_H
#define ADDIDCARDDIALOG_H

#include <QDialog>

namespace Ui {
class AddIdCardDialog;
}

class AddIdCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddIdCardDialog(QWidget *parent = 0);
    ~AddIdCardDialog();

private:
    Ui::AddIdCardDialog *ui;
};

#endif // ADDIDCARDDIALOG_H
