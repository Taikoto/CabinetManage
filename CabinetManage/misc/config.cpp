#include "config.h"

#include <QCoreApplication>

#define CONFIG_FILE QCoreApplication::applicationDirPath() + "/config.ini"
#define SERVER_IP                   "http://127.0.0.1:8080"
#define QSERVER_IP                  "http://127.0.0.1:8080"
#define QPOST                       8080
#define IS_QSERVER                  false
//GET API
#define API_GET_TIME                "/api/system/time"
#define API_GET_TYPE_LIST           "/api/objecttype/all"
#define API_GET_CAB_LIST            "/api/cab/byroom/"
#define API_GET_CAB_BY_ID           "/api/cab/byid/"
#define API_GET_ROOM_LIST           "/api/room/byunit/"
#define API_GET_ROOM_BY_ID          "/api/room/byid/"
#define API_GET_ROOM_CTASK_ITEM     "/api/room/currenttaskitem/byid/"
#define API_GET_TASK_BY_ID          "/api/task/byid/"
#define API_GET_TASK_BY_POLICE      "/api/task/current/"
#define API_GET_POLICE_BY_ID        "/api/police/byid/"
#define API_GET_POLICE_LIST         "/api/police/byroom/"
#define API_GET_CURRENT_LEAD        "/api/police/current/lead/"
#define API_GET_CURRENT_MANAGE      "/api/police/current/manage/"
#define API_GET_DEL_POLICE_BIO      "/api/police/bio/delete/"
#define API_GET_QPOLICE_ID          "/api/qtserver/police"
//POST API
#define API_POST_GUN_OUT            "/api/task/gunoper/out"
#define API_POST_GUN_IN             "/api/task/gunoper/in"
#define API_POST_AMMO_OUT           "/api/task/ammooper/out"
#define API_POST_AMMO_IN            "/api/task/ammooper/in"
#define API_POST_SYSTEM_LOG         "/api/system/log"
#define API_POST_HEART              "/api/cab/heart"
#define API_POST_TASK               "/api/task"
#define API_POST_TASK_URGENT        "/api/task/urgent"
#define API_POST_POLICE_BIO         "/api/police/bio"
#define API_POST_POLICE_IDCARD      "/api/police/idcard"
#define API_POST_ROOM_MANAGE        "/api/room/manage"
#define API_POST_ROOM_LEADER        "/api/room/lead"
//System
#define SYS_FULL_SCREEN             false   //是否全屏
#define SYS_HIDE_MOUSE              true    //是否隐藏鼠标
#define SYS_UPDATE_DATA             15      //更新值班信息的时间间隔
#define SYS_UPDATE_TEMPER           5       //更新温度
#define SYS_VERIFY_MAX_TIME         10      //验证身份的最长等待时间
#define SYS_V_MANAGER_NUM           2       //验证管理员的数量
//device
#ifdef Q_OS_WIN32
#define SERIAL_PORT                 "COM1"
#endif
#ifdef Q_OS_LINUX
#define SERIAL_PORT                 "/dev/ttyS0"
#endif





// Group name = GET
#define NAME_GET                    "GET"
#define NAME_GET_TIME               "Time"
#define NAME_GET_TYPE_LIST          "TypeList"
#define NAME_GET_CAB_LIST           "CabList"
#define NAME_GET_CAB_BY_ID          "CabById"
#define NAME_GET_ROOM_LIST          "RoomList"
#define NAME_GET_ROOM_BY_ID         "RoomById"
#define NAME_GET_ROOM_CTASK_ITEM    "RoomCTaskItem"
#define NAME_GET_TASK_BY_ID         "TaskById"
#define NAME_GET_TASK_BY_POLICE     "TaskByPoliceId"
#define NAME_GET_POLICE_BY_ID       "PoliceById"
#define NAME_GET_POLICE_LIST        "PoliceList"
#define NAME_GET_CURRENT_LEAD       "CurrentLead"
#define NAME_GET_CURRENT_MANAGE     "CurrentManage"
#define NAME_GET_DEL_POLICE_BIO     "DelPoliceBio"
#define NAME_GET_QPOLICE_ID         "QPoliceId"
// Group name = POST
#define NAME_POST                   "POST"
#define NAME_POST_GUN_OUT           "GunOut"
#define NAME_POST_GUN_IN            "GunIn"
#define NAME_POST_AMMO_OUT          "AmmoOut"
#define NAME_POST_AMMO_IN           "AmmoIn"
#define NAME_POST_SYSTEM_LOG        "SystemLog"
#define NAME_POST_HEART             "Heart"
#define NAME_POST_TASK              "Task"
#define NAME_POST_TASK_URGENT       "TaskUrgent"
#define NAME_POST_POLICE_BIO        "PoliceBio"
#define NAME_POST_POLICE_IDCARD     "PoliceIdCard"
#define NAME_POST_ROOM_MANAGE       "RoomManage"
#define NAME_POST_ROOM_LEADER       "RoomLead"
// Group name = SYSTEM
#define NAME_SYSTEM                 "SYSTEM"
#define NAME_IS_ARM4418             "IsArm4418"
#define NAME_FULL_SCREEN            "FullScreen"
#define NAME_HIDE_MOUSE             "HideMouse"
#define NAME_LCD_BACKIGHT           "LcdBackight"
#define NAME_CLOSE_LCD              "CloseLcd"
#define NAME_UPDATE_DATA            "UpdateData"
#define NAME_UPDATE_TEMPER          "UpdateTemper"
#define NAME_VERIFY_MAX_TIME        "VerifyMaxTime"
#define NAME_V_MANAGER_NUM          "VManagerNum"
#define NAME_LOCAL_VERIFY           "LocalVerify"
#define NAME_CAB_ID                 "CabId"
#define NAME_ROOM_ID                "RoomId"
#define NAME_UNIT_ID                "UnitId"
// Group name = DEVICE
#define NAME_DEVICE                 "DEVICE"
#define NAME_ARM4418_PORT           "Arm4418Port"
#define NAME_LOCK_PORT              "LockPort"
#define NAME_FINGER_PORT            "FingerPort"
#define NAME_IDCRAD_PORT            "IdCardPort"
#define NAME_MQ3_PORT               "Mq3Port"
#define NAME_RFID_PORT              "RfidPort"
#define NAME_UPS_PORT               "UpsPort"
#define NAME_HAVE_USBFINGER         "HaveUSBFinger"
#define NAME_HAVE_RFID              "HaveRFID"
#define NAME_HAVE_IDCARD            "HaveIdCard"



Config* Config::m_instance = NULL;

Config* Config::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new Config(parent);
    return m_instance;
}

Config::Config(QObject *parent) : QObject(parent)
{
    ini = new QSettings(CONFIG_FILE, QSettings::IniFormat);
}

Config::~Config()
{
    delete ini;
}

//创建配置文件
void Config::createFile()
{
    ini->beginGroup("HTTP");
    ini->setValue("ServerIp",   SERVER_IP);
    ini->setValue("QServerIp",  QSERVER_IP);
    ini->setValue("QPost",      QPOST);
    ini->setValue("IsQServer",  IS_QSERVER);
    ini->endGroup();

    ini->beginGroup(NAME_GET);
    ini->setValue(NAME_GET_TIME,            API_GET_TIME);
    ini->setValue(NAME_GET_TYPE_LIST,       API_GET_TYPE_LIST);
    ini->setValue(NAME_GET_CAB_LIST,        API_GET_CAB_LIST);
    ini->setValue(NAME_GET_CAB_BY_ID,       API_GET_CAB_BY_ID);
    ini->setValue(NAME_GET_ROOM_LIST,       API_GET_ROOM_LIST);
    ini->setValue(NAME_GET_ROOM_BY_ID,      API_GET_ROOM_BY_ID);
    ini->setValue(NAME_GET_ROOM_CTASK_ITEM, API_GET_ROOM_CTASK_ITEM);
    ini->setValue(NAME_GET_TASK_BY_ID,      API_GET_TASK_BY_ID);
    ini->setValue(NAME_GET_TASK_BY_POLICE,  API_GET_TASK_BY_POLICE);
    ini->setValue(NAME_GET_POLICE_BY_ID,    API_GET_POLICE_BY_ID);
    ini->setValue(NAME_GET_POLICE_LIST,     API_GET_POLICE_LIST);
    ini->setValue(NAME_GET_CURRENT_LEAD,    API_GET_CURRENT_LEAD);
    ini->setValue(NAME_GET_CURRENT_MANAGE,  API_GET_CURRENT_MANAGE);
    ini->setValue(NAME_GET_DEL_POLICE_BIO,  API_GET_DEL_POLICE_BIO);
    ini->setValue(NAME_GET_QPOLICE_ID,      API_GET_QPOLICE_ID);
    ini->endGroup();

    ini->beginGroup(NAME_POST);
    ini->setValue(NAME_POST_GUN_OUT,        API_POST_GUN_OUT);
    ini->setValue(NAME_POST_GUN_IN,         API_POST_GUN_IN);
    ini->setValue(NAME_POST_AMMO_OUT,       API_POST_AMMO_OUT);
    ini->setValue(NAME_POST_AMMO_IN,        API_POST_AMMO_IN);
    ini->setValue(NAME_POST_SYSTEM_LOG,     API_POST_SYSTEM_LOG);
    ini->setValue(NAME_POST_HEART,          API_POST_HEART);
    ini->setValue(NAME_POST_TASK,           API_POST_TASK);
    ini->setValue(NAME_POST_TASK_URGENT,    API_POST_TASK_URGENT);
    ini->setValue(NAME_POST_POLICE_BIO,     API_POST_POLICE_BIO);
    ini->setValue(NAME_POST_POLICE_IDCARD,  API_POST_POLICE_IDCARD);
    ini->setValue(NAME_POST_ROOM_MANAGE,    API_POST_ROOM_MANAGE);
    ini->setValue(NAME_POST_ROOM_LEADER,    API_POST_ROOM_LEADER);
    ini->endGroup();

    ini->beginGroup(NAME_SYSTEM);
    ini->setValue(NAME_IS_ARM4418,          false);
    ini->setValue(NAME_FULL_SCREEN,         SYS_FULL_SCREEN);
    ini->setValue(NAME_HIDE_MOUSE,          SYS_HIDE_MOUSE);
    ini->setValue(NAME_LCD_BACKIGHT,        100);
    ini->setValue(NAME_CLOSE_LCD,           600);
    ini->setValue(NAME_UPDATE_DATA,         SYS_UPDATE_DATA);
    ini->setValue(NAME_UPDATE_TEMPER,       SYS_UPDATE_TEMPER);
    ini->setValue(NAME_VERIFY_MAX_TIME,     SYS_VERIFY_MAX_TIME);
    ini->setValue(NAME_V_MANAGER_NUM,       SYS_V_MANAGER_NUM);
    ini->setValue(NAME_LOCAL_VERIFY,        true);
    ini->setValue(NAME_CAB_ID,              "");
    ini->setValue(NAME_ROOM_ID,             "");
    ini->setValue(NAME_UNIT_ID,             "");
    ini->setValue("OpenAllCard",            "");
    ini->endGroup();

    ini->beginGroup(NAME_DEVICE);
    ini->setValue(NAME_LOCK_PORT,           SERIAL_PORT);
    ini->setValue(NAME_ARM4418_PORT,        SERIAL_PORT);
    ini->setValue(NAME_FINGER_PORT,         SERIAL_PORT);
    ini->setValue(NAME_IDCRAD_PORT,         SERIAL_PORT);
    ini->setValue(NAME_MQ3_PORT,            SERIAL_PORT);
    ini->setValue(NAME_RFID_PORT,           SERIAL_PORT);
    ini->setValue(NAME_UPS_PORT,            SERIAL_PORT);
    ini->setValue(NAME_HAVE_USBFINGER,      false);
    ini->setValue(NAME_HAVE_RFID,           false);
    ini->setValue(NAME_HAVE_IDCARD,         false);
    ini->endGroup();
}

//读取配置文件
void Config::readConfig()
{
    // HTTP
    serverIp            = ini->value("HTTP/ServerIp", SERVER_IP).toString();
    qServerIp           = ini->value("HTTP/QServerIp", QSERVER_IP).toString();
    qPost               = ini->value("HTTP/QPost", QPOST).toInt();
    isQServer           = ini->value("HTTP/IsQServer", false).toBool();
    // GET API
    apiGetTime          = ini->value(NAME_GET "/" NAME_GET_TIME, API_GET_TIME).toString();
    apiGetTypeList      = ini->value(NAME_GET "/" NAME_GET_TYPE_LIST, API_GET_TYPE_LIST).toString();
    apiGetCabList       = ini->value(NAME_GET "/" NAME_GET_CAB_LIST, API_GET_CAB_LIST).toString();
    apiGetCabById       = ini->value(NAME_GET "/" NAME_GET_CAB_BY_ID, API_GET_CAB_BY_ID).toString();
    apiGetRoomList      = ini->value(NAME_GET "/" NAME_GET_ROOM_LIST, API_GET_ROOM_LIST).toString();
    apiGetRoomById      = ini->value(NAME_GET "/" NAME_GET_ROOM_BY_ID, API_GET_ROOM_BY_ID).toString();
    apiGetRoomCTaskItem = ini->value(NAME_GET "/" NAME_GET_ROOM_CTASK_ITEM, API_GET_ROOM_CTASK_ITEM).toString();
    apiGetTaskById      = ini->value(NAME_GET "/" NAME_GET_TASK_BY_ID, API_GET_TASK_BY_ID).toString();
    apiGetTaskByPId     = ini->value(NAME_GET "/" NAME_GET_TASK_BY_POLICE, API_GET_TASK_BY_POLICE).toString();
    apiGetPoliceById    = ini->value(NAME_GET "/" NAME_GET_POLICE_BY_ID, API_GET_POLICE_BY_ID).toString();
    apiGetPoliceList    = ini->value(NAME_GET "/" NAME_GET_POLICE_LIST, API_GET_POLICE_LIST).toString();
    apiGetCurrentLead   = ini->value(NAME_GET "/" NAME_GET_CURRENT_LEAD, API_GET_CURRENT_LEAD).toString();
    apiGetCurrentManage = ini->value(NAME_GET "/" NAME_GET_CURRENT_MANAGE, API_GET_CURRENT_MANAGE).toString();
    apiGetDelPoliceBio  = ini->value(NAME_GET "/" NAME_GET_DEL_POLICE_BIO, API_GET_DEL_POLICE_BIO).toString();
    apiGetQPoliceId     = ini->value(NAME_GET "/" NAME_GET_QPOLICE_ID, API_GET_QPOLICE_ID).toString();
    // POST API
    apiPostGunOut       = ini->value(NAME_POST "/" NAME_POST_GUN_OUT, API_POST_GUN_OUT).toString();
    apiPostGunIn        = ini->value(NAME_POST "/" NAME_POST_GUN_IN, API_POST_GUN_IN).toString();
    apiPostAmmoOut      = ini->value(NAME_POST "/" NAME_POST_AMMO_OUT, API_POST_AMMO_OUT).toString();
    apiPostAmmoIn       = ini->value(NAME_POST "/" NAME_POST_AMMO_IN, API_POST_AMMO_IN).toString();
    apiPostSystemLog    = ini->value(NAME_POST "/" NAME_POST_SYSTEM_LOG, API_POST_SYSTEM_LOG).toString();
    apiPostHeart        = ini->value(NAME_POST "/" NAME_POST_HEART, API_POST_HEART).toString();
    apiPostTask         = ini->value(NAME_POST "/" NAME_POST_TASK, API_POST_TASK).toString();
    apiPostTaskUrgent   = ini->value(NAME_POST "/" NAME_POST_TASK_URGENT, API_POST_TASK_URGENT).toString();
    apiPostPoliceBio    = ini->value(NAME_POST "/" NAME_POST_POLICE_BIO, API_POST_POLICE_BIO).toString();
    apiPostPoliceIdCard = ini->value(NAME_POST "/" NAME_POST_POLICE_IDCARD, API_POST_POLICE_IDCARD).toString();
    apiPostRoomManage   = ini->value(NAME_POST "/" NAME_POST_ROOM_MANAGE, API_POST_ROOM_MANAGE).toString();
    apiPostRoomLead     = ini->value(NAME_POST "/" NAME_POST_ROOM_LEADER, API_POST_ROOM_LEADER).toString();
    // SYSTEM
    isArm4418           = ini->value(NAME_SYSTEM "/" NAME_IS_ARM4418, false).toBool();
    fullScreen          = ini->value(NAME_SYSTEM "/" NAME_FULL_SCREEN, false).toBool();
    hideMouse           = ini->value(NAME_SYSTEM "/" NAME_HIDE_MOUSE, false).toBool();
    lcdBackight         = ini->value(NAME_SYSTEM "/" NAME_LCD_BACKIGHT, 100).toInt();
    closeLcd            = ini->value(NAME_SYSTEM "/" NAME_CLOSE_LCD, 600).toInt() * 1000;
    updateData          = ini->value(NAME_SYSTEM "/" NAME_UPDATE_DATA).toInt() * 1000;
    updateTemper        = ini->value(NAME_SYSTEM "/" NAME_UPDATE_TEMPER, 10).toInt() * 1000;
    verifyMaxTime       = ini->value(NAME_SYSTEM "/" NAME_VERIFY_MAX_TIME, 10).toInt() * 1000;
    vManagerNum         = ini->value(NAME_SYSTEM "/" NAME_V_MANAGER_NUM, 1).toInt();
    localVerify         = ini->value(NAME_SYSTEM "/" NAME_LOCAL_VERIFY, true).toBool();
    cabId               = ini->value(NAME_SYSTEM "/" NAME_CAB_ID).toString();
    roomId              = ini->value(NAME_SYSTEM "/" NAME_ROOM_ID).toString();
    unitId              = ini->value(NAME_SYSTEM "/" NAME_UNIT_ID).toString();
    openAllCard         = ini->value(NAME_SYSTEM "/" "OpenAllCard").toString();
    // DEVICE
    arm4418Port         = ini->value(NAME_DEVICE "/" NAME_ARM4418_PORT).toString();
    lockPort            = ini->value(NAME_DEVICE "/" NAME_LOCK_PORT).toString();
    fingerPort          = ini->value(NAME_DEVICE "/" NAME_FINGER_PORT).toString();
    idCardPort          = ini->value(NAME_DEVICE "/" NAME_IDCRAD_PORT).toString();
    rfidPort            = ini->value(NAME_DEVICE "/" NAME_RFID_PORT).toString();
    upsPort             = ini->value(NAME_DEVICE "/" NAME_UPS_PORT).toString();
    haveUSBFinger       = ini->value(NAME_DEVICE "/" NAME_HAVE_USBFINGER, false).toBool();
    haveRFID            = ini->value(NAME_DEVICE "/" NAME_HAVE_RFID, false).toBool();
    haveIdCard          = ini->value(NAME_DEVICE "/" NAME_HAVE_IDCARD, false).toBool();
}

void Config::setServerIp(QString ip)
{
    if( serverIp != ip && !ip.isEmpty() ){
        serverIp = ip;
        ini->setValue("HTTP/ServerIp",   ip);
    }
}

void Config::setLcdBackight(int backight)
{
    if( lcdBackight != backight && backight >= 0 && backight < 256 ){
        lcdBackight = backight;
        ini->setValue(NAME_SYSTEM "/" NAME_LCD_BACKIGHT, backight);
    }
}

void Config::setCloseLcd(int sec)
{
    if( closeLcd/1000 != sec && sec >= 0 ){
        closeLcd = sec*1000;
        ini->setValue(NAME_SYSTEM "/" NAME_CLOSE_LCD, sec);
    }
}

//设置更新数据间隔 单位：秒
void Config::setUpdateData(int sec)
{
    if( updateData/1000 != sec && sec >= 0 ){
        updateData = sec*1000;
        ini->setValue(NAME_SYSTEM "/" NAME_UPDATE_DATA, sec);
    }
}

//设置更新温度间隔 单位：秒
void Config::setUpdateTemper(int sec)
{
    if( updateTemper/1000 != sec && sec >= 0 ){
        updateTemper = sec*1000;
        ini->setValue(NAME_SYSTEM "/" NAME_UPDATE_TEMPER, sec);
    }
}

//设置验证管理员数量
void Config::setVManagerNum(int number)
{
    if( vManagerNum != number && number > 0 && number < 3 ){
        vManagerNum = number;
        ini->setValue(NAME_SYSTEM "/" NAME_V_MANAGER_NUM, number);
    }
}

void Config::setLocalVerify(bool enable)
{
    localVerify = enable;
    ini->setValue(NAME_SYSTEM "/" NAME_LOCAL_VERIFY, enable);
}

void Config::setCabId(QString id)
{
    cabId = id;
    ini->setValue(NAME_SYSTEM "/" NAME_CAB_ID, id);
}

void Config::setRoomId(QString id)
{
    roomId = id;
    ini->setValue(NAME_SYSTEM "/" NAME_ROOM_ID, id);
}

void Config::setOpenAllCard(QString id)
{
    openAllCard = id;
    ini->setValue(NAME_SYSTEM "/" "OpenAllCard", id);
}

void Config::setLockPort(const QString &name)
{
    ini->setValue(NAME_DEVICE "/" NAME_LOCK_PORT, name);
}

void Config::setHaveUSBFinger(const bool have)
{
    haveUSBFinger = have;
    ini->setValue(NAME_DEVICE "/" NAME_HAVE_USBFINGER, have);
}

void Config::setHaveRFID(const bool have)
{
    haveRFID = have;
    ini->setValue(NAME_DEVICE "/" NAME_HAVE_RFID, have);
}
