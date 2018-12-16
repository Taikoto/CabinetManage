#include "verify_p.h"

#include <QDebug>

using namespace DataModel;



/*! **************************************************************
    class VerifyBase : public QObject

    作者：Cat
    时间：2016-08-15
    描述：验证功能基础类，包含一些通用基础函数和功能类
    **************************************************************
*/


VerifyBase::VerifyBase(QObject *parent) :
    QObject(parent)
{
    config      = Config::instance();//配置类
    dataLoop    = DataLoop::instance();//数据池
    player      = MediaPlayer::instance();//音乐类
    uFinger     = UsbFinger::instance();//USB指纹仪
    sRfid       = SerialRfid::instance();//串口RFID

    connect(uFinger, SIGNAL(finished(UsbFinger::DevData)), this, SLOT(finished(UsbFinger::DevData)));
    connect(sRfid, SIGNAL(finished(SerialRfid::DevData)), this, SLOT(finished(SerialRfid::DevData)));

    sRfid->setPortName(config->rfidPort);

    //验证身份定时器
    connect(&verifyTimer, SIGNAL(timeout()), this, SLOT(startDevice()));
    connect(&verifyTimer, SIGNAL(timeout()), &verifyTimer, SLOT(stop()));
}


VerifyBase::~VerifyBase()
{
    uFinger->stopTask();
    sRfid->stopTask();
    uFinger->wait(100);
    sRfid->wait(100);
}


//判断是否是当前值班管理员
bool VerifyBase::isCManager(const QString &policeId)
{
    for( int i = 0; i < dataLoop->cManagers.length(); i++ )
    {
        if( policeId == dataLoop->cManagers[i].Id ) return true;
    }
    player->play(MediaPlayer::NotDutyManager);
    emit message(tr("You are not a duty manager!"));
    return false;
}


//启动身份验证设备
void VerifyBase::startDevice()
{
    if( config->haveUSBFinger ) uFinger->search(parent());
    if( config->haveRFID ) sRfid->getCardId(SerialRfid::ReadOnly, parent());

    if( !config->haveUSBFinger && !config->haveRFID )
        emit message(tr("Please open at least one verification device!"));
}


//USB指纹仪
void VerifyBase::finished(const UsbFinger::DevData data)
{
    if( data.code != PS_OK || data.type != UsbFinger::Search ){
        emit message(uFinger->getErrorString(data.code));
        return ;
    }

    if( config->haveRFID ) sRfid->stopTask();//取消SerialRfid任务

    Police police;
    if( data.fingerId == 999 ){
        police.Id = "999";
        police.PoliceType = (int)Leader;//领导
    } else {
        police = DataLoop::findPolice(data.fingerId);
    }
    emit getPolice(police);
}


//RFID
void VerifyBase::finished(const SerialRfid::DevData data)
{
    if( data.parent != parent() ) return ;
    if( data.code == SerialRfid::UserCanceled ) return ;//用户取消

    //检错
    if( data.code != SerialRfid::Success ){
        //emit message(sRfid->getErrorString(data.code));
        return ;
    }

    if( data.type == SerialRfid::GetCardId ){
        if( config->haveUSBFinger ) uFinger->stopTask();

        emit getPolice(DataLoop::findPoliceByRfid(data.cardId));
    }
}








/*! **************************************************************
    class TaskVerify : public QObject

    作者：Cat
    时间：2016-08-15
    描述：任务验证流程代码集成类，验证枪支弹药管理员、警员
    **************************************************************
*/


TaskVerify::TaskVerify(QObject *parent) :
    QObject(parent)
{
    base = new VerifyBase(this);
    connect(base, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(base, SIGNAL(getPolice(DataModel::Police)), this, SLOT(analysisPolice(DataModel::Police)));

    network = new Network(this);
    network->setServerIp(base->config->qServerIp);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));
}


TaskVerify::~TaskVerify()
{
    delete base;
    delete network;
}


//! 启动任务流程验证
void TaskVerify::start()
{
    mangerId.clear();
    base->player->play(MediaPlayer::VDutyManager);
    emit message(tr("Please verify the identity of the duty manager"));
    base->startDevice();
}


//! 网络类
void TaskVerify::finished(const Network::DataCore data)
{
    if(data.code != Network::Success || data.type != Network::GetQPoliceId){
        base->player->play(MediaPlayer::VerifyFailed);
        emit message(network->getErrorString(data.code));
    }

    emit finished(data.police.PoliceType == (int)Constable, data.police);
}


//!分析警员
void TaskVerify::analysisPolice(Police police)
{
    static int getIntoNum = 0;

    if( mangerId.isEmpty() ){
        //! 第一次进入情况（验证第一个枪支弹药管理员）
        if( !base->isCManager(police.Id) ) return ;

        mangerId = police.Id;   //保存通过验证的管理员ID

        if( base->config->vManagerNum == 1 ){   //验证警员
            getIntoNum = 1;
            verifyTaskPolice();
        } else {                                //验证第二个管理员
            getIntoNum = 2;
            base->player->play(MediaPlayer::OtherVManager);
            emit message(tr("Please another duty manager verify the identity..."));
            base->startDevice(1000);
        }
        return ;
    }

    //! 第二次进入情况、第三次进入情况（警员验证完成）
    if( getIntoNum == 1 || getIntoNum == 3 ){
        if( police.PoliceType != (int)Constable ){
            base->player->play(MediaPlayer::NotPolice);
            emit message(tr("You are not a police!"));
        }
        emit finished(police.PoliceType == (int)Constable, police);
        return ;
    }

    //! 第二次进入情况（管理员验证成功）
    if( getIntoNum == 2 ){
        if( police.Id == mangerId ){    //同一管理员验证两次
            emit message(tr("Need another duty manager to verify!"));
            emit finished(false, police);
            return ;
        }
        getIntoNum = 3;
        verifyTaskPolice();
    }
}


void TaskVerify::verifyTaskPolice()
{
    if( base->config->localVerify ){
        //本机验证警员
        base->player->play(MediaPlayer::VPolice);
        emit message(tr("Please verify the identity of the police"));
        base->startDevice(1000);
    } else {
        //服务器验证警员
        base->player->play(MediaPlayer::WaitVPolice);
        emit message(tr("Wait for the police to verify the identity..."));
        network->getQPoliceId();
    }
}









/*! **************************************************************
    class PoliceVerify : public QObject

    作者：Cat
    时间：2016-08-15
    描述：验证警员
    **************************************************************
*/


PoliceVerify::PoliceVerify(QObject *parent) :
    QObject(parent)
{
    base = new VerifyBase(this);
    connect(base, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(base, SIGNAL(getPolice(DataModel::Police)), this, SLOT(analysisPolice(DataModel::Police)));
}


PoliceVerify::~PoliceVerify()
{
    delete base;
}


//! 启动验证警员
void PoliceVerify::start()
{
    base->player->play(MediaPlayer::VPolice);
    emit message(tr("Please verify the identity of the police"));
    base->startDevice();
}


//!分析警员
void PoliceVerify::analysisPolice(Police police)
{
    if( police.PoliceType != (int)Constable ){
        base->player->play(MediaPlayer::NotPolice);
        emit message(tr("You are not a police!"));
    }
    emit finished(police.PoliceType == (int)Constable, police);
}









/*! **************************************************************
    class ManagerVerify : public QObject

    作者：Cat
    时间：2016-08-18
    描述：验证枪支弹药管理员
    **************************************************************
*/


ManagerVerify::ManagerVerify(QObject *parent) :
    QObject(parent)
{
    base = new VerifyBase(this);
    connect(base, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(base, SIGNAL(getPolice(DataModel::Police)), this, SLOT(analysisPolice(DataModel::Police)));
}


ManagerVerify::~ManagerVerify()
{
    delete base;
}


//! 启动验证
void ManagerVerify::start()
{
    base->player->play(MediaPlayer::VManger);
    emit message(tr("Please verify the identity of the manager"));
    base->startDevice();
}


//!分析
void ManagerVerify::analysisPolice(Police police)
{
    if( police.PoliceType != (int)GunManager ){
        base->player->play(MediaPlayer::NotManager);
        emit message(tr("You are not a manager!"));
    }
    emit finished(police.PoliceType == (int)GunManager, police);
}









/*! **************************************************************
    class DutyManagerVerify : public QObject

    作者：Cat
    时间：2016-08-18
    描述：验证值班枪支弹药管理员
    **************************************************************
*/


DutyManagerVerify::DutyManagerVerify(QObject *parent) :
    QObject(parent)
{
    base = new VerifyBase(this);
    connect(base, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(base, SIGNAL(getPolice(DataModel::Police)), this, SLOT(analysisPolice(DataModel::Police)));
}


DutyManagerVerify::~DutyManagerVerify()
{
    delete base;
}


//! 启动验证
void DutyManagerVerify::start()
{
    base->player->play(MediaPlayer::VDutyManager);
    emit message(tr("Please verify the identity of the duty manager"));
    base->startDevice();
}


//!分析
void DutyManagerVerify::analysisPolice(Police police)
{
    emit finished(base->isCManager(police.Id), police);
}









/*! **************************************************************
    class LeaderVerify : public QObject

    作者：Cat
    时间：2016-08-18
    描述：验证值班枪支弹药管理员
    **************************************************************
*/


LeaderVerify::LeaderVerify(QObject *parent) :
    QObject(parent)
{
    base = new VerifyBase(this);
    connect(base, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(base, SIGNAL(getPolice(DataModel::Police)), this, SLOT(analysisPolice(DataModel::Police)));
}


LeaderVerify::~LeaderVerify()
{
    delete base;
}


//! 启动验证
void LeaderVerify::start()
{
    base->player->play(MediaPlayer::VLeader);
    emit message(tr("Please verify the identity of the leader"));
    base->startDevice();
}


//!分析
void LeaderVerify::analysisPolice(Police police)
{
    if( police.PoliceType != (int)Leader ){
        base->player->play(MediaPlayer::NotLeader);
        emit message(tr("You are not a leader!"));
    }
    emit finished(police.PoliceType == (int)Leader, police);
}









/*! **************************************************************
    class EnrollFinger : public QObject

    作者：Cat
    时间：2016-08-18
    描述：录入指纹
    **************************************************************
*/


EnrollFinger::EnrollFinger(QObject *parent) :
    QObject(parent)
{
    config      = Config::instance();//配置类
    dataLoop    = DataLoop::instance();//数据池
    player      = MediaPlayer::instance();//音乐类
    uFinger     = UsbFinger::instance();//USB指纹仪

    connect(dataLoop, SIGNAL(finished(Network::TaskType)), this, SLOT(finished(Network::TaskType)));
    connect(uFinger, SIGNAL(finished(UsbFinger::DevData)), this, SLOT(finished(UsbFinger::DevData)));
}

EnrollFinger::~EnrollFinger()
{
    uFinger->stopTask();
    uFinger->wait(100);
}

void EnrollFinger::start()
{
    emit message(tr("Updating police data..."));
    dataLoop->updatePoliceList();//更新警员数据，以防多台设备同时在录指纹
}

static bool sortInt(const PoliceBio &first, const PoliceBio &second)
{
    return (first.FingerprintId < second.FingerprintId);
}

void EnrollFinger::finished(Network::TaskType type)
{
    if( type != Network::GetPoliceList ) return ;

    PoliceBioList bios = DataLoop::findAllBio();

    //生成指纹ID
    qSort(bios.begin(), bios.end(), sortInt);//从小到大排序
    int i;//指纹ID最小为 0
    for( i = 0; i < bios.length(); i++ ){
        if(i != bios[i].FingerprintId) break;
    }

    player->play(MediaPlayer::PressFinger);
    emit message(tr("Please press the fingerprint"));
    uFinger->enroll(i);
}

void EnrollFinger::finished(const UsbFinger::DevData)
{
    emit finished(true, Police());
}
