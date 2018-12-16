#ifndef TASKITEMDIALOG_H
#define TASKITEMDIALOG_H

#include "dialog.h"
#include "dataloop.h"
#include "zdiylist.h"


namespace Ui {
class TaskItemDialog;
}

class TaskItemDialog : public Dialog
{
    Q_OBJECT
public:
    explicit TaskItemDialog(QWidget *parent = 0);
    ~TaskItemDialog();

    static DataModel::TaskPolice getGunPolice(QWidget *parent = 0);

private:
    Ui::TaskItemDialog *ui;
    DataLoop *dataLoop;

    DataModel::PoliceList pList;
    DataModel::ObjectTypeList cGunList;
    DataModel::TaskPolice taskp;

    void updatePoliceList(const DataModel::PoliceList &list);

    void updateGunList(const DataModel::ObjectTypeList &list);

private slots:
    void setTaskPolice();
};

#endif // TASKITEMDIALOG_H
