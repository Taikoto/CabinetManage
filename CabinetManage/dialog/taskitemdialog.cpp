#include "taskitemdialog.h"
#include "ui_taskitemdialog.h"

#include <QDebug>

using namespace DataModel;

TaskItemDialog::TaskItemDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::TaskItemDialog)
{
    ui->setupUi(this);

    ui->policeList->setTitle(tr("Police list"));
    ui->policeList->addColumn(ZDiy::Label, tr("Name"));
    ui->policeList->addColumn(ZDiy::Label, tr("No"));
    ui->policeList->addColumn(ZDiy::CheckBox, tr("Sate"), 50);
    ui->policeList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"));
    ui->policeList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"));
    ui->policeList->setPageItemCount(5);

    ui->gunList->setTitle(tr("Gun list"));
    ui->gunList->addColumn(ZDiy::Label, "", 50);
    ui->gunList->addColumn(ZDiy::Label, tr("GunName"));
    ui->gunList->addColumn(ZDiy::Label, tr("No"));
    ui->gunList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"));
    ui->gunList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"));
    ui->policeList->setPageItemCount(5);

    dataLoop = DataLoop::instance(this);

    updateGunList(dataLoop->types);
    updatePoliceList(dataLoop->polices);
}

TaskItemDialog::~TaskItemDialog()
{
    delete ui;
}

DataModel::TaskPolice TaskItemDialog::getGunPolice(QWidget *parent)
{
    TaskItemDialog dialog(parent);

    if( dialog.exec() == QDialog::Accepted ) dialog.setTaskPolice();
    return dialog.taskp;
}

void TaskItemDialog::setTaskPolice()
{
    int pIndex = ui->policeList->currentIndex();
    int gIndex = ui->gunList->currentIndex();
    if( pIndex >= pList.length() || pIndex < 0 ) return ;
    if( gIndex >= cGunList.length() || gIndex < 0 ) return ;

    TaskItem gunItem, ammoItem, cilpItem;
    initModel(taskp);
    QString BelongId = uuid();

    gunItem.Id = uuid();
    gunItem.TaskPoliceId = pList[pIndex].Id;
    gunItem.ObjectTypeId = cGunList[gIndex].No;
    gunItem.TaskItemType = 1;//出枪支
    gunItem.TaskItemStatus = Unexecuted;
    gunItem.ObjectNumber = 1;
    gunItem.BelongId = BelongId;
    gunItem.OneOperNumber = 0;
    gunItem.TwoOperNumber = 0;

    ammoItem.Id = uuid();
    ammoItem.TaskPoliceId = pList[pIndex].Id;
    ammoItem.ObjectTypeId = cGunList[gIndex].AmmoTypeNo;
    ammoItem.TaskItemType = 2;//出子弹
    ammoItem.TaskItemStatus = Unexecuted;
    ammoItem.ObjectNumber = cGunList[gIndex].AmmoNumber;
    ammoItem.BelongId = BelongId;
    ammoItem.OneOperNumber = 0;
    ammoItem.TwoOperNumber = 0;

    cilpItem.Id = uuid();
    cilpItem.TaskPoliceId = pList[pIndex].Id;
    cilpItem.ObjectTypeId = cGunList[gIndex].AmmoBoxTypeNo;
    cilpItem.TaskItemType = 2;
    cilpItem.TaskItemStatus = Unexecuted;
    cilpItem.ObjectNumber = cGunList[gIndex].AmmoBoxNumber;
    cilpItem.BelongId = BelongId;
    cilpItem.OneOperNumber = 0;
    cilpItem.TwoOperNumber = 0;

    taskp.Id = uuid();
    taskp.PoliceId = pList[pIndex].Id;
    taskp.No = pList[pIndex].No;
    taskp.Name = pList[pIndex].Name;
    taskp.IsComplete = false;
    taskp.IsResponsible = ui->policeList->itemIsChecked(pIndex, 2);;//是否为负责人

    taskp.TaskItems.append(gunItem);//添加枪支任务清单
    taskp.TaskItems.append(ammoItem);//添加子弹任务清单
    taskp.TaskItems.append(cilpItem);
}

void TaskItemDialog::updatePoliceList(const PoliceList &list)
{
    int num = 0;//Police list 计数器
    ui->policeList->clear();
    for( int i = 0; i < list.length(); i++ )
    {
        if(list[i].PoliceType != Admin)
        {
            pList.append(list[i]);
            ui->policeList->addItem();
            ui->policeList->setItemText(num, 0, list[i].Name);
            ui->policeList->setItemText(num, 1, list[i].No);
            num++;
        }
    }
    if( num != 0) ui->policeList->sendClicked(0);
}

void TaskItemDialog::updateGunList(const ObjectTypeList &list)
{
    int num = 0;
    ui->gunList->clear();
    for(int i = 0; i < list.length(); i++)
    {
        if(list[i].No / 1000 == 1)//筛选枪支
        {
            cGunList.append(list[i]);
            ui->gunList->addItem();
            ui->gunList->setItemIcon(num, 0, typePhoto(list[i].No));
            ui->gunList->setItemText(num, 1, list[i].Name);
            ui->gunList->setItemText(num, 2, QString::number(list[i].No));
            num++;
        }
    }
    if( num != 0) ui->gunList->sendClicked(0);
}
