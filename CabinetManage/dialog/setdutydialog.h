#ifndef SETDUTYDIALOG_H
#define SETDUTYDIALOG_H

#include "dialog.h"
#include "network.h"

namespace Ui {
class SetDutyDialog;
}

class SetDutyDialog : public Dialog
{
    Q_OBJECT

public:
    explicit SetDutyDialog(QWidget *parent = 0);
    ~SetDutyDialog();

private:
    Ui::SetDutyDialog *ui;
    Config *config;
    Network *network;
    DataModel::PoliceList leaderList;
    QString cLeaderId;

    void updatePoliceList(const DataModel::PoliceList &list);

private slots:
    void listChecked(int index);

    void finished(const Network::DataCore &data);

    void on_ok_clicked();
};

#endif // SETDUTYDIALOG_H
