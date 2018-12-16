#ifndef ALCOHOLDIALOG_H
#define ALCOHOLDIALOG_H

#include <QTimer>
#include "dialog.h"
#include "arm4418.h"

namespace Ui {
class AlcoholDialog;
}

class AlcoholDialog : public Dialog
{
    Q_OBJECT

public:
    explicit AlcoholDialog(QWidget *parent = 0);
    ~AlcoholDialog();

private slots:
    void updateAlcohol();

private:
    Ui::AlcoholDialog *ui;
    Arm4418 *arm4418;
    QTimer timer;
};

#endif // ALCOHOLDIALOG_H
