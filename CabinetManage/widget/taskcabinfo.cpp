#include "taskcabinfo.h"
#include "ui_taskcabinfo.h"

#include "dialog/infodialog.h"
#include "dialog/inputintdialog.h"
#include <QIcon>
#include <QPixmap>
#include <QDateTime>
#include <QDebug>

using namespace DataModel;

TaskCabInfo::TaskCabInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskCabInfo)
{
    ui->setupUi(this);

    config = Config::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    connect(ui->cabInfoBox, SIGNAL(openCabinet(bool)), ui->taskInfoBox, SLOT(setEnabled(bool)));
}

TaskCabInfo::~TaskCabInfo()
{
    delete ui;
}

void TaskCabInfo::init()
{
    initModel(task);
    cTaskItems = NULL;

    //********  初始化TaskInfo  ******
    ui->photo->setIcon(QIcon(":/img/photo/headPhoto.png"));
    ui->taskStartTime->setText(tr("Loading..."));
    ui->taskEndTime->setText(tr("Loading..."));
    ui->taskPolices->clear();
    ui->taskItemType->clear();
    ui->taskItemState->clear();
    ui->taskItemSum->clear();
    ui->taskItemNum->clear();
    //*******************************

    ui->cabInfoBox->init();
    ui->taskInfoBox->setEnabled(true);
}

void TaskCabInfo::init(Police taskPolice, const TaskType &type)
{
    init();
    this->taskType = type;

    QPixmap photo;
    photo.loadFromData(QByteArray::fromBase64(taskPolice.PhotoBase64.toLatin1()));
    if (!photo.isNull()) ui->photo->setIcon(photo);

    network->getCabById(config->cabId); // 更新枪柜信息

    if(type == Emergency) {
        ui->cabInfoBox->setData(TaskItem(), Emergency, 0);

        ui->taskType->setText(taskTypeName(DataModel::Emergency));
        QDateTime time = QDateTime::currentDateTime();
        ui->taskStartTime->setText(time.toString("yyyy-MM-dd hh:mm"));
        ui->taskEndTime->setText(time.toString("yyyy-MM-dd hh:mm"));
        ui->taskPolices->addItem(taskPolice.Name);
    } else {
        network->getTaskByPoliceId(taskPolice.Id);
    }
}

// 根据TaskId初始化CabinetInfo，只有在中控取枪时使用该函数
void TaskCabInfo::init(QString taskId)
{
    init();

    network->getCabById(config->cabId); // 更新枪柜信息
    network->getTaskById(taskId);
}




/**
 ********************************************************************
 * private:
 ********************************************************************
*/

//显示任务信息
void TaskCabInfo::showTaskInfo()
{
    ui->taskType->setText(taskTypeName(task.TaskType));

    QDateTime time = QDateTime::fromMSecsSinceEpoch(task.StartTime);
    ui->taskStartTime->setText(time.toString("yyyy-MM-dd hh:mm"));

    time = QDateTime::fromMSecsSinceEpoch(task.EndTime);
    ui->taskEndTime->setText(time.toString("yyyy-MM-dd hh:mm"));

    //显示任务警员信息
    ui->taskPolices->clear();
    for( int i = 0; i < task.TaskPolices.length(); i++ )
        ui->taskPolices->addItem(task.TaskPolices[i].Name);
}

//检查任务状态
bool TaskCabInfo::checkTaskStatus(const TaskStatus &status)
{
    QString text;
    switch ( status ) {
    case Approved:
    case Executing:     return true; // 直接退出函数
    case NotApproved:   text = tr("Your task has not been approved!"); break;
    case NotPass:       text = tr("Your task has not been passed!"); break;
    case End:           text = tr("Your task is over!"); break;
    default:            text = tr("Data error!"); break;
    }
    InfoDialog::error(this, text);
    emit backWidget();
    return false;
}

//检查任务类型
bool TaskCabInfo::checkTaskType(const TaskType &type)
{
    if( type == taskType ) return true; // 相应的任务只能从相应的按钮里面进去

    QString text;
    switch ( type ) {
    case Emergency:     text = tr("Your task is emergency out police task"); break;
    case OutPolice:     text = tr("Your task is out police task"); break;
    case Keep:          text = tr("Your task is keep task"); break;
    case Storage:       text = tr("Your task is in library task"); break;
    case Dumping:       text = tr("Your task is dumping task"); break;
    case TempDeposit:   text = tr("Your task is temp deposit task"); break;
    case OtherTaskType:
    default:            text = tr("Data error!"); break;
    }
    InfoDialog::error(this, text);
    emit backWidget();
    return false;
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void TaskCabInfo::finished(const Network::DataCore &data)
{
    if( this->isHidden() ) return ;

    if(data.code != Network::Success){
        InfoDialog::error(this, network->getErrorString(data.code));
        emit backWidget();
        return ;
    }

    switch (data.type) {
    case Network::GetTaskByPoliceId:
    case Network::GetTaskById: {
        task = data.task;
        if (!checkTaskStatus((TaskStatus)task.TaskStatus)) return ; // 检查任务状态
        if (!checkTaskType((TaskType)task.TaskType)) return ; // 检查任务类型

        showTaskInfo();//显示任务信息
        ui->approve->setText(DataLoop::findPolice(task.ApproveLeadId).Name);
    }
        break;
    default:
        break;
    }
}

void TaskCabInfo::on_taskItemType_currentIndexChanged(int index)
{
    if(index < 0 || index >= cTaskItems->length()) return ;

    const TaskItem &item = cTaskItems->at(index);

    ui->taskItemState->setText(taskItemStatus(item.TaskItemStatus, task.TaskType));
    ui->taskItemSum->setText(QString::number(item.ObjectNumber));

    if (task.TaskType == Storage || task.TaskType == TempDeposit) {
        if (isGetObject(item.TaskItemStatus)) {
            ui->taskItemNumText->setText(tr("Storaged:")); // 已存
            ui->taskItemNum->setText(QString::number(item.OneOperNumber));
        } else {
            ui->taskItemNumText->setText(tr("Received:")); // 已领
            ui->taskItemNum->setText(QString::number(item.TwoOperNumber));
        }
    } else {
        if (isGetObject(item.TaskItemStatus)) {
            ui->taskItemNumText->setText(tr("Received:")); // 已领
            ui->taskItemNum->setText(QString::number(item.OneOperNumber));
        } else {
            ui->taskItemNumText->setText(tr("Returned:")); // 已还
            ui->taskItemNum->setText(QString::number(item.TwoOperNumber));
        }
    }

    //显示Cabinet
    int count = item.ObjectNumber - ui->taskItemNum->text().toInt();//总数 - 已领 = 可操作数
    ui->cabInfoBox->setData(item, (TaskType)task.TaskType, count);//刷新显示
}

void TaskCabInfo::on_taskPolices_currentIndexChanged(int index)
{
    if(index < 0 || index >= task.TaskPolices.length()) return;

    //更新当前警员任务清单
    cTaskItems = &(task.TaskPolices[index].TaskItems);

    //添加任务清单数据
    ui->taskItemType->clear();
    ui->taskItemType->setIconSize(QSize(27, 23));
    for( int i = 0; i < cTaskItems->length(); i++ )
    {
        const int &id = cTaskItems->at(i).ObjectTypeId;
        ui->taskItemType->addItem(typePhoto(id), DataLoop::findObjType(id).Name);
    }
}
