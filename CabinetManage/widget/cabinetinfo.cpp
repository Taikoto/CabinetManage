#include "cabinetinfo.h"
#include "ui_cabinetinfo.h"

#include "dialog/infodialog.h"
#include "dialog/rfiddialog.h"
#include <QDateTime>
#include <QFile>
#include <QDebug>

using namespace DataModel;

CabinetInfo::CabinetInfo(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::CabinetInfo)
{
    ui->setupUi(this);

    boxs.append(ui->groupBox_1);
    boxs.append(ui->groupBox_2);
    boxs.append(ui->groupBox_3);
    boxs.append(ui->groupBox_4);
    boxs.append(ui->groupBox_5);
    boxs.append(ui->groupBox_6);
    boxs.append(ui->groupBox_7);
    boxs.append(ui->groupBox_8);
    for( int i = 0; i < 8; i++ )
        connect(boxs[i], SIGNAL(checked(int)), this, SLOT(subCabinetChecked(int)));

    config = Config::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this);

    sLock = SerialLock::instance(this);
    connect(sLock, SIGNAL(finished(SerialLock::DevData)), this, SLOT(finished(SerialLock::DevData)));

    arm4418 = Arm4418::instance(this);
    connect(arm4418, SIGNAL(finished(Arm4418::DataCore)), this, SLOT(finished(Arm4418::DataCore)));

    connect(&numberTimer, SIGNAL(timeout()), this, SLOT(updateGetNumber()));
    connect(InfoDialog::instance(this), SIGNAL(accepted()), &numberTimer, SLOT(stop()));
    connect(InfoDialog::instance(this), SIGNAL(rejected()), &numberTimer, SLOT(stop()));

    init();
}

CabinetInfo::~CabinetInfo()
{
    delete ui;
}




/**
 ********************************************************************
 * public slots:
 ********************************************************************
*/

void CabinetInfo::init()
{
    DataModel::initModel(taskItem);
    this->operCount = 0;
    this->pageCount = 0;
    this->currentPage = 0;

    ui->pageLabel->setText("0 / 0");
    ui->pb_openCab->show();
    ui->openLock->setEnabled(false);
    ui->openLock->show();
    ui->getNumber->show();
    ui->openAll->hide();

    for( int i = 0; i < boxs.length(); i++ ) boxs[i]->init();
}

void CabinetInfo::setData(const TaskItem &item, TaskType type, int operCount)
{
    this->taskItem = item;
    this->taskType = type;
    this->operCount = operCount;

    foreach (SubCabInfo *box, boxs) box->setTaskType(type);
    if (type == DataModel::Emergency) ui->openAll->show();

    setCabinetData(dataLoop->cabinet);
}

void CabinetInfo::hideButton()
{
    ui->pb_openCab->hide();
    ui->openLock->hide();
    ui->getNumber->hide();
}




/**
 ********************************************************************
 * private:
 ********************************************************************
*/

void CabinetInfo::showPage(int index)
{
    if(index < 0 || index >= pageCount) return ;

    for (int i = 0; i < boxs.length(); i++) {
        if(index*8+i < dataLoop->cabinet.SubCabs.length()){
            //setData会初始化控件，故setTaskItem要在下面
            boxs[i]->setData(&dataLoop->cabinet, index*8+i);
            boxs[i]->setEnabled(taskItem);
        } else
            boxs[i]->hide();
    }

    currentPage = index;//更新当前显示的页面
    //更新页面显示
    ui->pageLabel->setText(QString("%1 / %2").arg(currentPage+1).arg(pageCount));
}

void CabinetInfo::setCabinetData(Cabinet &cabinet)
{
    //计算总页数
    pageCount = cabinet.SubCabs.length() / 8;
    if(cabinet.SubCabs.length()%8 != 0) pageCount++;//没有一页，做一页计算
    showPage(0);
    //如果是枪柜，就不显示取弹按钮
    if(cabinet.CabType == (int)GunCabinet) ui->getNumber->hide();
}

//! 用户操作检查
bool CabinetInfo::operChecked()
{
    //检测：是否有勾选枪支、是否有超过
    QMap<int, int> sMap;
    Cabinet &cab = dataLoop->cabinet;
    for( int i = 0; i < cab.SubCabs.length(); i++ )
    {
        if( cab.SubCabs[i].isChecked ){
            if( !cab.SubCabs[i].Guns.isEmpty() ){
                sMap[cab.SubCabs[i].Guns.last().ObjectTypeId] += 1;
            }
            else if( !cab.SubCabs[i].Ammos.isEmpty() ){
                sMap[cab.SubCabs[i].Ammos.last().ObjectTypeId] += cab.SubCabs[i].number;
            }
        }
    }
    if (taskItem.Id.isEmpty()) return true; // 紧急出警

    int &typeId = taskItem.ObjectTypeId;
    if( sMap[typeId] == 0 ){
        InfoDialog::information(this, tr("Please select gun or ammo!"));
        return false;
    }
    if( sMap.count() != 1 || sMap[typeId] > operCount ){
        InfoDialog::information(this, tr("You chose more!"));
        return false;
    }
    return true;
}

//! 上传枪支操作记录
bool CabinetInfo::uploadGunOper(int index)
{
    Cabinet &cab = dataLoop->cabinet;
    if(cab.SubCabs[index].Guns.isEmpty()) return false;

    DataModel::TaskGunOper oper;
    oper.Id             = uuid();
    oper.ManageId       = dataLoop->cManagers.first().Id;
    oper.CabId          = cab.Id;
    oper.SubCabId       = cab.SubCabs[index].Id;
    oper.TaskItemId     = taskItem.Id;
    oper.ObjectTypeId   = taskItem.ObjectTypeId;
    oper.GunId          = cab.SubCabs[index].Guns.last().Id;
    oper.GunNo          = cab.SubCabs[index].Guns.last().No;
    oper.GunEno         = cab.SubCabs[index].Guns.last().Eno;
    oper.OperNumber     = 1;
    oper.AddTime        = QDateTime::currentMSecsSinceEpoch();

    if (taskType == Storage || taskType == TempDeposit) {
        if (isGetObject(taskItem.TaskItemStatus)) {
            oper.OperType = DataModel::In;
            network->postGunIn(oper);
        } else {
            oper.OperType = DataModel::Out;
            network->postGunOut(oper);
        }
    } else {
        if (isGetObject(taskItem.TaskItemStatus)) {
            oper.OperType = DataModel::Out;
            network->postGunOut(oper);
        } else {
            oper.OperType = DataModel::In;
            network->postGunIn(oper);
        }
    }

    return true;
}

//! 上传弹药操作记录
bool CabinetInfo::uploadAmmoOper(int index)
{
    Cabinet &cab = dataLoop->cabinet;
    if(cab.SubCabs[index].Ammos.isEmpty()) return false;

    DataModel::TaskAmmoOper oper;
    oper.Id             = uuid();
    oper.ManageId       = dataLoop->cManagers.first().Id;
    oper.CabId          = cab.Id;
    oper.SubCabId       = cab.SubCabs[index].Id;
    oper.TaskItemId     = taskItem.Id;
    oper.ObjectTypeId   = taskItem.ObjectTypeId;
    oper.AmmoId         = cab.SubCabs[index].Ammos.last().Id;
    oper.OperNumber     = cab.SubCabs[index].number;
    oper.AddTime        = QDateTime::currentMSecsSinceEpoch();

    if (taskType == Storage || taskType == TempDeposit) {
        if (isGetObject(taskItem.TaskItemStatus)) {
            oper.OperType = DataModel::In;
            network->postAmmoIn(oper);
        } else {
            oper.OperType = DataModel::Out;
            network->postAmmoOut(oper);
        }
    } else {
        if (isGetObject(taskItem.TaskItemStatus)) {
            oper.OperType = DataModel::Out;
            network->postAmmoOut(oper);
        } else {
            oper.OperType = DataModel::In;
            network->postAmmoIn(oper);
        }
    }

    return true;
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void CabinetInfo::updateGetNumber()
{
    if( isHidden() || parentWidget()->isHidden() || !(config->isArm4418) ){
        numberTimer.stop();
        return ;
    }

    for( int i = 0; i < dataLoop->cabinet.SubCabs.length(); i++ )
    {
        if(dataLoop->cabinet.SubCabs[i].isChecked) arm4418->readNumber(this, i+1);
    }
}

void CabinetInfo::finished(const Network::DataCore &data)
{
    if( isHidden() || parentWidget()->isHidden() ) return ;

    if(data.code == Network::Success){
        if(data.type == Network::GetCabById){
            dataLoop->cabinet = data.cabinet;
            setCabinetData(dataLoop->cabinet);
        }
    }
}

void CabinetInfo::finished(const SerialLock::DevData data)
{
    Q_UNUSED(data);
//    static int errCount = 0, errCount2 = 0;
//    if( this != data.parent ) return ;

//    switch (data.code) {
//    case SerialLock::Success:
//        switch (data.type) {
//        case SerialLock::OpenLock:      errCount = 0; break;
//        case SerialLock::EXOpenLock:    errCount2 = 0; break;
//        default: break;
//        }
//        break;
//    case SerialLock::OpenSerialFailed:  break;//串口打开失败，不重试
//    case SerialLock::UserCanceled:      break;//用户取消，不重试
//    default:
//        switch (data.type) {
//        case SerialLock::OpenLock:
//            if( ++errCount >= 2 ) break;
//            sLock->openLock(data.lockNo, 0x05, this);
//            break;
//        case SerialLock::EXOpenLock:
//            if( ++errCount2 >= 2 ) break;
//            sLock->exOpenLock(data.exNo, data.lockNo, 0x05, this);
//            break;
//        default: break;
//        }
//        break;
//    }
}

void CabinetInfo::finished(Arm4418::DataCore data)
{
    if( this != data.parent ) return ;

    if( data.code == Arm4418::Success ){
        if( data.type == Arm4418::ReadNumber ){
            if( data.dAddr >= dataLoop->cabinet.SubCabs.length() ) return ;
            if( dataLoop->cabinet.SubCabs[data.dAddr].Ammos.isEmpty() ) return ;
            int &number = dataLoop->cabinet.SubCabs[data.dAddr].Ammos.last().ObjectNumber;
            QString text = tr("No: %1, Sum: %2, Current: %3, Changed: %4")
                    .arg(data.dAddr).arg(number).arg(data.number).arg(abs(number-data.number));
            InfoDialog::information(this, text);
        }
    }
}

void CabinetInfo::subCabinetChecked(int no)
{
    SubCabinet &subCab = dataLoop->cabinet.SubCabs[no];
    if( subCab.Ammos.isEmpty() ) return ;//判断是否是弹柜
    if( subCab.isChecked ) return ;//判断是否是选中状态

    //为 0 ，则只完成任务，不用还弹药
    if( subCab.Ammos.last().ObjectNumber == 0 ){
        InfoDialog::information(this, tr("You have chosen an empty ammunition cabinet!"));
        subCab.number = 0;
        return ;
    }

    //当前抽屉子弹数量不够
    if( subCab.Ammos.last().ObjectNumber < taskItem.ObjectNumber ){
        InfoDialog::information(this, tr("Insufficient ammunition for the current drawer!"));
        return ;
    }
    subCab.number = taskItem.ObjectNumber;
}

void CabinetInfo::on_pb_back_clicked()
{
    showPage(currentPage-1);
}

void CabinetInfo::on_pb_next_clicked()
{
    showPage(currentPage+1);
}

void CabinetInfo::on_pb_openCab_clicked()
{
    static bool isFast;
    if( !operChecked() ) return ;

    emit openCabinet(false);

    isFast = !(ui->openLock->isEnabled());//只有开锁按钮禁用时，才是第一次点击
    ui->openLock->setEnabled(true);

    if( config->isArm4418 ){
        arm4418->openLock(this, 0x00);
        qWarning() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                   << "(lock v2.0) Open 0x00 lock...";
    } else {
        sLock->openLock(0x00, 0x14, this);
        sLock->openLock(0x01, 0x14, this);
        qWarning() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                   << "(lock v1.0) Open 0x00 and 0x01 lock...";
    }

    if( !isFast ) return ;//只有第一次点击才上传数据

    for( int i = 0; i < dataLoop->cabinet.SubCabs.length(); i++ )
    {
        if(dataLoop->cabinet.SubCabs[i].isChecked){
            switch (taskItem.ObjectTypeId/1000) {
            case 1: uploadGunOper(i); break;    //枪支
            case 2:                             //弹药
            case 3: uploadAmmoOper(i); break;   //弹夹
            default: break;
            }
        }
    }

    //使能选中模式
    for( int i = 0; i < boxs.length(); i++ ) boxs[i]->setEnabledChecked(true);
    showPage(currentPage);
}

void CabinetInfo::on_openLock_clicked()
{
    if (!config->isArm4418) {
        QFile file(QApplication::applicationDirPath() + "/lock.txt");
        QList<int> lockList;
        if( file.open(QIODevice::ReadOnly | QIODevice::Text) ){
            while (!file.atEnd()) {
                QByteArray line = file.readLine();
                lockList << QString(line).toInt();
            }
            file.close();
        }
        for( int i = 0; i < dataLoop->cabinet.SubCabs.length(); i++ )
        {
            if (dataLoop->cabinet.SubCabs[i].isChecked) {
                sLock->exOpenLock( lockList[i]/8+1, 1 << (lockList[i]%8), 0x00, this);
                qWarning() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                           << QString("(lock v1.0) Open 0x%1 lock...").arg(lockList[i], 2, 16);
            }
        }
    } else {
        for (int i = 0; i < dataLoop->cabinet.SubCabs.length(); i++) {
            if (dataLoop->cabinet.SubCabs[i].isChecked) {
                arm4418->openLock(this, i+1);
                qWarning() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                           << QString("(lock v2.0) Open 0x%1 lock...").arg(i+1, 2, 16);
            }
        }
    }
}

void CabinetInfo::on_getNumber_clicked()
{
    if(numberTimer.isActive()) return ;
    numberTimer.start(500);
    InfoDialog::information(this, tr("Loading..."));
}

void CabinetInfo::on_openAll_clicked()
{
    QString cardId = RfidDialog::getCardId(this);
    if (cardId.isEmpty()) return ;
    if (cardId != config->openAllCard) {
        InfoDialog::information(this, tr("Validation failure!"));
        return ;
    }

    qWarning() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
               << "Open all lock!";
    for (int i = 1; i <= dataLoop->cabinet.SubCabs.length(); i++) {
        if (config->isArm4418) arm4418->openLock(this, i);
        else sLock->exOpenLock( i/8+1, 1 << (i%8), 0x00, this);
    }
}
