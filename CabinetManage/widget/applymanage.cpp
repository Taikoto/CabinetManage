#include "applymanage.h"
#include "ui_applymanage.h"

#include <QDateTime>
#include <QStringList>
#include <QDebug>

#include "dialog/infodialog.h"
#include "dialog/datetimedialog.h"
#include "dialog/taskitemdialog.h"

using namespace DataModel;

ApplyManage::ApplyManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplyManage)
{
    ui->setupUi(this);

    config = Config::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this);

    //任务清单列表
    ui->itemList->setTitle(tr("Task item list"));
    ui->itemList->addColumn(ZDiy::Label, "", 50);
    ui->itemList->addColumn(ZDiy::Label, tr("Gun type"));
    ui->itemList->addColumn(ZDiy::LineEdit, tr("Gun number"), 60);
    ui->itemList->addColumn(ZDiy::Label, tr("Ammo type"));
    ui->itemList->addColumn(ZDiy::LineEdit, tr("Ammo number"), 60);
    ui->itemList->addColumn(ZDiy::LineEdit, tr("Clip number"), 60);
    ui->itemList->addColumn(ZDiy::Label, tr("Police name"));
    ui->itemList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"));
    ui->itemList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"));
    ui->itemList->setPageItemCount(7);
    connect(ui->itemList, SIGNAL(textChanged(int,int)), this, SLOT(textChanged(int,int)));
}

ApplyManage::~ApplyManage()
{
    delete ui;
}




/**
 ********************************************************************
 * public slots:
 ********************************************************************
*/

void ApplyManage::init(Police police)
{
    // *****  显示警员信息  *****
    QPixmap photo;
    photo.loadFromData(QByteArray::fromBase64(police.PhotoBase64.toLatin1()));
    if( !photo.isNull() ) ui->photo->setIcon(photo);
    else emit showMessage(tr("Police photo is empty."), 2000);
    ui->name->setText(police.Name);
    ui->no->setText(police.No);
    ui->phone->setText(police.Phone);
    // ***********************

    // *****  初始化界面  *****
    ui->startTime->clear();
    ui->endTime->clear();
    ui->approvePolice->clear();
    ui->approveStatus->clear();
    ui->itemList->clear();
    // **********************

    initModel(task);

    taskPolice = police;
    network->getTaskByPoliceId(police.Id);
}

//设置左边信息GroupBox是否禁用
void ApplyManage::setInfoBoxEnabled(bool enable)
{
    ui->name->setEnabled(false);
    ui->no->setEnabled(false);
    ui->phone->setEnabled(false);
    ui->startTime->setEnabled(false);
    ui->startTimeButton->setEnabled(enable);
    ui->endTime->setEnabled(false);
    ui->endTimeButton->setEnabled(enable);
    ui->approvePolice->setEnabled(false);
    ui->approveStatus->setEnabled(false);
    ui->taskSubType->setEnabled(enable);
}





/**
 ********************************************************************
 * private:
 ********************************************************************
*/

//更新任务清单列表
void ApplyManage::updateItemList()
{
    gunItemList.clear();
    ammoItems.clear();
    clipItems.clear();
    QStringList nameList;

    //循环任务警员列表
    for( int i = 0; i < task.TaskPolices.length(); i++ )
    {
        const TaskItemList &taskItems = task.TaskPolices[i].TaskItems;
        //循环任务清单列表
        for( int j = 0; j < taskItems.length(); j++ )
        {
            switch ( taskItems[j].ObjectTypeId/1000 ) {
            case 1:
                gunItemList.append(taskItems[j]);//将枪支清单加入gunItemList
                nameList.append(task.TaskPolices[i].Name);//将枪支清单所属警员的名称保存
                break;
            case 2: ammoItems.append(taskItems[j]); break;
            case 3: clipItems.append(taskItems[j]); break;
            default: break;
            }
        }
    }

    //!  将gunItemList显示到界面
    ui->itemList->clear();
    for( int i = 0; i < gunItemList.length(); i++ )
    {
        ui->itemList->addItem();
        setItemList(i, gunItemList[i], nameList[i]);
    }
}

//设置ItemList数据
void ApplyManage::setItemList(const int &index, const TaskItem &item, const QString &name)
{
    if( index < 0 || index >= ui->itemList->length() ) return ;

    ui->itemList->setItemIcon(index, 0, typePhoto(item.ObjectTypeId));
    ui->itemList->setItemText(index, 1, objectType(dataLoop->types, item.ObjectTypeId).Name);
    ui->itemList->setItemText(index, 2, QString::number(item.ObjectNumber));
    int typeId = objectType(dataLoop->types, item.ObjectTypeId).AmmoTypeNo;
    ui->itemList->setItemText(index, 3, objectType(dataLoop->types, typeId).Name);
    //子弹
    int ammoNumber = findTaskItem(ammoItems, item.BelongId).ObjectNumber;
    ui->itemList->setItemText(index, 4, QString::number(ammoNumber));
    //弹夹
    int clipNumber = findTaskItem(clipItems, item.BelongId).ObjectNumber;
    ui->itemList->setItemText(index, 5, QString::number(clipNumber));

    ui->itemList->setItemText(index, 6, name);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void ApplyManage::textChanged(int itemNo, int widgetNo)
{
    if( itemNo < 0 || itemNo >= ui->itemList->length() ) return ;
    if( ui->itemList->itemText(itemNo, widgetNo).isEmpty() ) return ;

    bool ok = false;
    int number = ui->itemList->itemText(itemNo, widgetNo).toInt(&ok);
    if( !ok ){
        InfoDialog::error(this, tr("Please enter a number!"));
        return ;
    }
    QString id;

    switch (widgetNo) {
    case 2: id = gunItemList[itemNo].Id; break;
    case 4: id = findTaskItem(ammoItems, gunItemList[itemNo].BelongId).Id; break;
    case 5: id = findTaskItem(clipItems, gunItemList[itemNo].BelongId).Id; break;
    default: break;
    }

    ItemPoint point = findTaskItem(task, id);
    if( point.pIndex == -1 || point.iIndex == -1 ) return ;
    task.TaskPolices[point.pIndex].TaskItems[point.iIndex].ObjectNumber = number;
}

void ApplyManage::finished(const Network::DataCore data)
{
    if( this->isHidden() ) return ;

    QDateTime dateTime;
    switch (data.type) {
    case Network::GetTaskByPoliceId:
        if( data.code != Network::Success ) goto IsError;

        setInfoBoxEnabled(false);
        ui->buttonWidget->setEnabled(false);
        dateTime = QDateTime::fromMSecsSinceEpoch(data.task.StartTime);
        ui->startTime->setText(dateTime.toString("yyyy-MM-dd hh:mm"));
        dateTime = QDateTime::fromMSecsSinceEpoch(data.task.EndTime);
        ui->endTime->setText(dateTime.toString("yyyy-MM-dd hh:mm"));

        if(data.task.ApproveLeadId == dataLoop->cLeader.Id)
            ui->approvePolice->setText(dataLoop->cLeader.Name);
        else
            network->getPoliceById(data.task.ApproveLeadId);

        ui->approveStatus->setText(taskStatus(data.task.TaskStatus));

        if( data.task.TaskSubType == OtherTaskSubType ) ui->taskSubType->setCurrentIndex(4);
        else ui->taskSubType->setCurrentIndex(data.task.TaskSubType - 1);

        task = data.task;
        updateItemList();//刷新 itemList

        InfoDialog::information(this, tr("You have applied for a task!"));
        break;
    case Network::GetPoliceById:
        if( data.code != Network::Success ) goto IsError;
        ui->approvePolice->setText(data.police.Name);
        break;
    case Network::PostTask:
        if( data.code == Network::Success ){
            InfoDialog::information(this, data.message.Body);
        } else {
            setInfoBoxEnabled(true);
            ui->buttonWidget->setEnabled(true);
            InfoDialog::error(this, network->getErrorString(data.code));
        }
        break;
    default:
        break;
    }
    return ;

IsError:
    if( data.code == Network::NoTaskData ){
        setInfoBoxEnabled(true);
        ui->buttonWidget->setEnabled(true);
        ui->approvePolice->setText(dataLoop->cLeader.Name);
        ui->approveStatus->setText(taskStatus(Applying));
    }
    else {
        InfoDialog::error(this, network->getErrorString(data.code));
        emit backWidget();
    }
}

void ApplyManage::on_startTimeButton_clicked()
{
    QDateTime dateTime = DateTimeDialog::getDateTime(this);
    ui->startTime->setText(dateTime.toString("yyyy-MM-dd hh:mm"));
}

void ApplyManage::on_endTimeButton_clicked()
{
    QDateTime dateTime = DateTimeDialog::getDateTime(this);
    ui->endTime->setText(dateTime.toString("yyyy-MM-dd hh:mm"));
}

void ApplyManage::on_addPolice_clicked()
{
    TaskPolice taskPolice = TaskItemDialog::getGunPolice(this);
    if( taskPolice.TaskItems.length() == 0 ) return ;

    int i;
    //若任务列表中已有此警员，则将新加的Item附加到该警员身上
    for( i = 0; i < task.TaskPolices.length(); i++ )
    {
        if( task.TaskPolices[i].PoliceId == taskPolice.PoliceId ){
            task.TaskPolices[i].TaskItems.append(taskPolice.TaskItems);
            break;
        }
    }
    //否则，将警员加入任务中
    if( i == task.TaskPolices.length() ) task.TaskPolices.append(taskPolice);

    //更新界面显示
    for( i = 0; i < taskPolice.TaskItems.length(); i++ )
    {
        switch (taskPolice.TaskItems[i].ObjectTypeId/1000) {
        case 1: gunItemList.append(taskPolice.TaskItems[i]); break;
        case 2: ammoItems.append(taskPolice.TaskItems[i]); break;
        case 3: clipItems.append(taskPolice.TaskItems[i]); break;
        default: break;
        }
    }
    ui->itemList->addItem();
    setItemList(ui->itemList->length()-1, gunItemList.last(), taskPolice.Name);
}

void ApplyManage::on_delPolice_clicked()
{
    int index = ui->itemList->currentIndex();
    if( index < 0 || index >= ui->itemList->length() ){
        InfoDialog::error(this, tr("Please choose a task item!"));
        return ;
    }

    QString BelongId = gunItemList[index].BelongId;
    for( int i = 0; i < task.TaskPolices.length(); i++ )
    {
        TaskItemList &items = task.TaskPolices[i].TaskItems;
        for( int j = 0; j < items.length(); j++ )
        {
            if( BelongId == items[j].BelongId ) items.removeAt(j--);
        }
        if( items.isEmpty() ) task.TaskPolices.removeAt(i--);
    }

    updateItemList();//刷新列表
}

void ApplyManage::on_ok_clicked()
{
    if( task.TaskPolices.isEmpty() ){
        InfoDialog::error(this, tr("Please add at least one task item!"));
        return ;
    }
    if( ui->startTime->text().isEmpty() ){
        InfoDialog::error(this, tr("Please select the start time of the task plan!"));
        return ;
    }
    if( ui->endTime->text().isEmpty() ){
        InfoDialog::error(this, tr("Please choose the end time of the task plan!"));
        return ;
    }

    task.Id = uuid();
    task.RoomId = config->roomId;
    task.TaskType = OutPolice;
    if( ui->taskSubType->currentIndex() == 4 ) task.TaskSubType = (int)OtherTaskSubType;
    else task.TaskSubType = ui->taskSubType->currentIndex() + 1;
    task.TaskStatus = (int)Applying;
    task.PoliceId = taskPolice.Id;
    QDateTime time;
    time = QDateTime::fromString(ui->startTime->text(), "yyyy-MM-dd hh:mm");
    task.StartTime = time.toMSecsSinceEpoch();
    time = QDateTime::fromString(ui->endTime->text(), "yyyy-MM-dd hh:mm");
    task.EndTime = time.toMSecsSinceEpoch();
    task.ApproveLeadId = dataLoop->cLeader.Id;
    task.AddTime = QDateTime::currentMSecsSinceEpoch();
    for( int i = 0; i < task.TaskPolices.length(); i++ )
        task.TaskPolices[i].TaskId = task.Id;
    qDebug() << "Apply out police, task data:\n" << serialize(task);
    network->postTask(task);

    setInfoBoxEnabled(false);
    ui->buttonWidget->setEnabled(false);
}
