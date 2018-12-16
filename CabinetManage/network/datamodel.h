#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QUuid>
#include <QIcon>
#include "json.h"

namespace DataModel {

 
QString uuid();

QByteArray getPhotoBase64(QString path);//获取图片的Base64编码

/**
 ********************************************************************
 * 枚举:
 ********************************************************************
*/

//警衔
enum Rank {
    Lv0 = 0     //无警衔
    , Lv1 = 1   //一级警员
    , Lv2 = 2   //二级警员
    , Lv3 = 3   //一级警司
    , Lv4 = 4   //二级警司
    , Lv5 = 5   //三级警司
    , Lv6 = 6   //一级警督
    , Lv7 = 7   //二级警督
    , Lv8 = 8   //三级警督
    , Lv9 = 9   //一级警监
    , Lv10 = 10 //二级警监
    , Lv11 = 11 //三级警监
};

QString rank(int rank);

//政治面貌
enum Departy {
    CPCMember = 1,		//中共党员
    CPCYouth = 2,       //共青团员
    Masses = 3			//群众
};

QString departy(int departy);

enum ObjectStatus {
    InLibrary = 1,
    PoliceOut,
    MaintainOut,
    ApplyStorage,
    ApplyScrap,         // 申请报废
    ApplyTemporary,
    TemporaryStorage,
    Abnormal = 99
};

QString objectStatus(const ObjectStatus &status);

bool isInLibrary(const ObjectStatus &status);

//操作类型
enum OperType {
    Out = 1     //出
    , In = 2    //入
};

//任务状态
enum TaskStatus {
    Applying = 0,       //申请中
    NotApproved = 1,    //未审批
    Approved = 2,       //已审批
    NotPass = 3,        //审批不通过
    Executing = 4,      //执行中
    End = 5             //结束
};

//日志类型
enum LogType {
    OperMsg = 1,    //操作信息
    WarnMsg = 2     //警告信息
};

//生物特征类型
enum BioType {
    LeftThumb = 1       //左手大拇指
    , LeftFore = 2      //左手食指
    , LeftMedius = 3    //左手中指
    , LeftRing = 4      //左手无名指
    , LeftPinky = 5     //左手小拇指
    , RightThumb = 6    //右手大拇指
    , RightFore = 7     //右手食指
    , RightMedius = 8   //右手中指
    , RightRing = 9     //右手无名指
    , RightPinky = 10   //右手小拇指
    , LeftEye = 11      //左眼虹膜
    , RightEye = 12     //右眼虹膜
    , OtherBioTye = 13  //其他
};

//警员生物特征验证方式
enum BioCheckType {
    BioNormal = 1       //正常验证
    , BioAbnormal = 2   //非正常验证
};

enum TaskType {
    Emergency = 1
    , OutPolice = 2
    , Keep = 3
    , Storage = 4
    , Dumping = 5
    , TempDeposit = 6
    , OtherTaskType = 8133
};

QString taskTypeName(int type); // 任务类型

enum TaskSubType {
    OtherTaskSubType = 99
    , Obscenity = 1
    , Gamble = 2
    , Narcotics = 3
    , Penal = 4
};

QString taskSubType(int type);      //任务子类型
QString taskStatus(int status);     //任务状态

//任务清单状态
enum TaskItemStatus {
    Unexecuted = 1      //领取中（未执行）
    , Receiving = 2     //执行中（第一阶段）
    , Returning = 3     //归还中（第二阶段）
    , Finished = 4      //执行完成
};

QString taskItemStatus(TaskItemStatus state);
QString taskItemStatus(TaskItemStatus state, TaskType type);
bool isGetObject(TaskItemStatus status);

//任务清单类型
enum TaskItemType {
    OutGun = 1      //出枪支
    , OutAmmo = 2   //出弹药
    , InGun = 3     //入枪支
    , InAmmo = 4    //入弹药
};

//警员身份类型
enum PoliceType {
    Admin = 0           //超级管理员
    , GunManager = 1    //枪械管理员
    , Constable = 2     //普通警员
    , Leader = 3        //领导
};

QString policeType(int type);//警员身份类型

enum CabinetType {
    GunCabinet = 1
    , AmmoCabinet = 2
    , MixingCabinet = 3
};

QString cabinetType(int type);//枪柜类型



QIcon typePhoto(int typeNo);

/* 解壳 "{"Success":true,"Body":1455107026429}" */
QtJson::JsonObject doDeShell(QString json);//返回 图
QtJson::JsonArray doDeListShell(QString json);//返回 表

QByteArray addShell(QByteArray json);//加壳

/*"{"Message":"未找到与请求 URI“...”匹配的 HTTP 资源。"}" */
bool doDeSerialize(QString json, QString &msg);




/**
 ********************************************************************
 * 反馈信息:
 ********************************************************************
*/

typedef struct {
    bool    Success;
    QString Body;
}Message;

void initModel(Message &msg);
bool doDeSerialize(QString json, Message &msg);
bool doDeSerialize(QtJson::JsonObject jsonData, Message &msg);
QByteArray serialize(Message &msg);



/**
 ********************************************************************
 * 时间:
 ********************************************************************
*/

typedef struct {
    bool    Success;
    unsigned long long Body;
}Time;

void initModel(Time &time);
bool doDeSerialize(QString json, Time &time);
bool doDeSerialize(QtJson::JsonObject jsonData, Time &time);
QByteArray serialize(Time &time);



/**
 ********************************************************************
 * 警员登录模型:
 ********************************************************************
*/

typedef struct {
    QString No;
    QString Password;
}Login;

void initModel(Login &model);
bool doDeSerialize(QString json, Login &model);
bool doDeSerialize(QtJson::JsonObject jsonData, Login &model);
QByteArray serialize(Login &model);



/**
 ********************************************************************
 * 设置值班领导/枪管员模型:
 ********************************************************************
*/

typedef struct {
    QString RoomId;     //监控室ID
    QString CurrentId;  //当前的值班领导/枪管员ID(如果是枪管员第一次上班，传null就可以了)
    QString NewId;      //新的值班领导/枪管员ID(如果是枪管员离班，直接传入NULL即可)
}SetCurrent;

void initModel(SetCurrent &model);
bool doDeSerialize(QString json, SetCurrent &model);
bool doDeSerialize(QtJson::JsonObject jsonData, SetCurrent &model);
QByteArray serialize(SetCurrent &model);



/**
 ********************************************************************
 * 枪支弹药类型:
 ********************************************************************
*/

typedef struct {
    int         No;             //类型编号
    QString     Name;           //类型名称
    int         AmmoTypeNo;     //关联的子弹类型
    int         AmmoBoxTypeNo;  //关联的弹夹类型
    int         AmmoNumber;     //关联的子弹类型默认数量
    int         AmmoBoxNumber;  //关联的弹夹类型默认数量
}ObjectType;

void initModel(ObjectType &item);
bool doDeSerialize(QString json, ObjectType &item);
bool doDeSerialize(QtJson::JsonObject jsonData, ObjectType &item);
QByteArray serialize(ObjectType &item);

typedef QList<ObjectType> ObjectTypeList;//枪支弹药类型列表

void initModel(ObjectTypeList &list);
bool doDeSerialize(QtJson::JsonArray array, ObjectTypeList &list);

//根据类型ID，获取类型名称
ObjectType objectType(const ObjectTypeList &typeList,int typeId);



/**
 ********************************************************************
 * 弹药模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键（GUID）
    QString     RoomId;         //监控室Id
    QString     RoomName;       //监控室ID
    QString     CabId;          //枪柜Id
    QString     CabNo;          //枪柜编号
    QString     SubCabId;       //子柜ID
    QString     SubCabNo;       //子柜编号
    int         Weight;         //单个弹药的重量
    int         ObjectNumber;   //数量
    DataModel::ObjectStatus ObjectStatus;   //枪支状态
    int         ObjectTypeId;   //弹药类型
    bool        IsBox;          //是否是弹夹
    QString     TaskId;         //任务Id
    unsigned long long AddTime; //添加时间
}Ammo;

void initModel(Ammo &item);
bool doDeSerialize(QString json, Ammo &item);
bool doDeSerialize(QtJson::JsonObject jsonData, Ammo &item);
QByteArray serialize(Ammo &item);

typedef QList<Ammo> AmmoList;//弹药列表

void initModel(AmmoList &list);
bool doDeSerialize(QtJson::JsonArray array, AmmoList &list);



/**
 ********************************************************************
 * 枪支模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键（GUID）
    QString     RoomId;         //监控室Id
    QString     RoomName;       //监控室名称
    QString     CabId;          //枪柜Id
    QString     CabNo;          //枪柜编号
    QString     SubCabId;       //子柜ID
    QString     SubCabNo;       //子柜编号
    QString     No;             //枪支编号
    QString     Eno;            //枪支电子编号
    DataModel::ObjectStatus ObjectStatus;   //枪支状态
    int         ObjectTypeId;   //枪支类型
    QString     PoliceNo;       //专属于某个警员
    QString     TaskId;         //任务Id
    unsigned long long  AddTime;//添加时间
}Gun;

void initModel(Gun &item);
bool doDeSerialize(QString json, Gun &item);
bool doDeSerialize(QtJson::JsonObject jsonData, Gun &item);
QByteArray serialize(Gun &item);

typedef QList<Gun> GunList;//枪支列表

void initModel(GunList &list);
bool doDeSerialize(QtJson::JsonArray array, GunList &list);



/**
 ********************************************************************
 * 枪支心跳包模型:
 ********************************************************************
*/

typedef struct {
    QString GunId;    //枪支ID
    int     Status;   //在位状态
}GunHeart;

void initModel(GunHeart &item);
bool doDeSerialize(QString json, GunHeart &item);
bool doDeSerialize(QtJson::JsonObject jsonData, GunHeart &item);
QByteArray serialize(GunHeart &item);



/**
 ********************************************************************
 * 枪弹子柜模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;         //主键（GUID）
    QString     RoomId;     //监控室Id
    QString     RoomName;   //监控室名称
    QString     CabId;      //枪柜Id
    QString     CabNo;      //枪柜编号
    int         No;         //子柜编号，主类中需要是int类型
    bool        isChecked;
    int         number;
    int         SubCabType; //子柜类型
    QString     NetAddress; //子柜网卡地址
    QString     IpAddress;  //子柜监控地址
    QString     Eno;        //电子编号
    GunList     Guns;       //枪柜下面的枪集合
    AmmoList    Ammos;     //子柜下面的弹药集合
    unsigned long long AddTime; //添加时间
}SubCabinet;

void initModel(SubCabinet &item);
bool doDeSerialize(QString json, SubCabinet &item);
bool doDeSerialize(QtJson::JsonObject jsonData, SubCabinet &item);
QByteArray serialize(SubCabinet &item);

typedef QList<SubCabinet> SubCabinetList;//枪弹子柜列表

void initModel(SubCabinetList &list);
bool doDeSerialize(QtJson::JsonArray array, SubCabinetList &list);
QByteArray serialize(SubCabinetList list);



/**
 ********************************************************************
 * 枪弹柜模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;         //主键（GUID）
    QString     RoomId;     //监控室Id
    QString     RoomName;   //监控室名称
    QString     No;         //枪柜编号
    int         CabType;    //枪柜类型
    QString     NetAddress; //枪柜网卡地址
    QString     IpAddress;  //枪柜监控地址
    QString     LordId;     //主柜ID
    int         NetStatus;  //网络状态，心跳包
    int         EleStatus;  //电源状态，心跳包
    int         TempStatus; //温度状态，心跳包
    unsigned long long AddTime; //添加时间
    SubCabinetList  SubCabs;    //枪柜下面的枪集合
}Cabinet;

void initModel(Cabinet &item);
bool doDeSerialize(QString json, Cabinet &item);
bool doDeSerialize(QtJson::JsonObject jsonData, Cabinet &item);
QByteArray serialize(Cabinet &item);

typedef QList<Cabinet> CabinetList;

void initModel(CabinetList &list);
bool doDeSerialize(QtJson::JsonArray array, CabinetList &list);
QByteArray serialize(CabinetList list);



/**
 ********************************************************************
 * 监控室模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键（GUID）
    QString     UnitId;         //单位Id
    QString     UnitName;       //单位名称
    QString     No;             //监控室编号
    QString     Name;           //监控室名称
    QString     Address;        //监控室地址
    QString     IpAddress;      //监控室IP监控地址
    QString     ManageId1;      //值班枪管员Id
    QString     ManageId2;      //值班枪管员Id
    QString     ManageId3;      //值班枪管员Id
    QString     TaskId;         //当前正在进行中的任务ID
    QString     LeadId;         //值班领导Id
    int         LockTime;           //自动锁定时长（分钟数）
    QString     SystemPassword; //特殊密码（不确定是否需要，暂留）
    unsigned long long AddTime; //添加时间
}Room;

void initModel(Room &item);
bool doDeSerialize(QString json, Room &item);
bool doDeSerialize(QtJson::JsonObject jsonData, Room &item);
QByteArray serialize(Room &item);

typedef QList<Room> RoomList;

void initModel(RoomList &list);
bool doDeSerialize(QtJson::JsonArray array, RoomList &list);
QByteArray serialize(RoomList list);



/**
 ********************************************************************
 * 任务枪支操作模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键（GUID）
    QString     ManageId;       //负责操作的枪管员Id
    QString     CabId;          //枪柜Id
    QString     SubCabId;       //子柜Id
    QString     TaskItemId;     //任务清单Id
    int         ObjectTypeId;   //枪支的类型
    QString     GunId;          //枪支Id
    QString     GunNo;          //枪支编号
    QString     GunEno;         //枪支电子编号
    int         OperType;       //出入类型（入、出）
    int         OperNumber;     //出入数量（枪支只能是一只，客户端传1就好了）
    unsigned long long AddTime; //添加时间
}TaskGunOper;

void initModel(TaskGunOper &item);
bool doDeSerialize(QString json, TaskGunOper &item);
bool doDeSerialize(QtJson::JsonObject jsonData, TaskGunOper &item);
QByteArray serialize(TaskGunOper &item);

typedef QList<TaskGunOper> TaskGunOperList;

void initModel(TaskGunOperList &list);
bool doDeSerialize(QtJson::JsonArray array, TaskGunOperList &list);
QByteArray serialize(TaskGunOperList list);



/**
 ********************************************************************
 * 任务弹药操作模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键（GUID）
    QString     ManageId;       //枪管员Id
    QString     CabId;          //枪柜Id
    QString     SubCabId;       //子柜Id
    QString     TaskItemId;     //任务清单Id
    int         ObjectTypeId;   //类型
    QString     AmmoId;         //弹药Id
    int         OperType;       //出入类型（入、出）
    int         OperNumber;     //出入数量
    unsigned long long AddTime; //添加时间
}TaskAmmoOper;

void initModel(TaskAmmoOper &item);
bool doDeSerialize(QString json, TaskAmmoOper &item);
bool doDeSerialize(QtJson::JsonObject jsonData, TaskAmmoOper &item);
QByteArray serialize(TaskAmmoOper &item);

typedef QList<TaskAmmoOper> TaskAmmoOperList;

void initModel(TaskAmmoOperList &list);
bool doDeSerialize(QtJson::JsonArray array, TaskAmmoOperList &list);
QByteArray serialize(TaskAmmoOperList list);



/**
 ********************************************************************
 * 任务清单模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键（GUID）
    QString     TaskPoliceId;   //任务警员Id
    int         TaskItemType;   //清单类型
    DataModel::TaskItemStatus   TaskItemStatus; //任务清单状态
    int         ObjectTypeId;   //枪支/弹药的类型
    int         ObjectNumber;   //数量
    QString     BelongId;       //所属ID
    QString     Title;          //任务清单的简单描述
    int         OneOperNumber;  //第一阶段的执行的数据条数
    int         TwoOperNumber;  //第二阶段的执行的数据条数
    TaskGunOperList     OutGunOpers;      //领枪记录子集
    TaskAmmoOperList    OutAmmoOpers;     //领弹记录子集
    TaskGunOperList     InGunOpers;       //还枪记录子集
    TaskAmmoOperList    InAmmoOpers;      //还弹记录子集
}TaskItem;

void initModel(TaskItem &item);
bool doDeSerialize(QString json, TaskItem &item);
bool doDeSerialize(QtJson::JsonObject jsonData, TaskItem &item);
QByteArray serialize(TaskItem &item);

typedef QList<TaskItem> TaskItemList;//任务清单模型列表

void initModel(TaskItemList &list);
bool doDeSerialize(QtJson::JsonArray array, TaskItemList &list);
QByteArray serialize(TaskItemList list);

//根据BelongId查找TaskItem
TaskItem findTaskItem(const TaskItemList &items, const QString &BelongId);



/**
 ********************************************************************
 * 任务警员模型:
 ********************************************************************
*/

typedef struct {
    QString Id;             //主键（GUID）
    QString PoliceId;       //任务警员Id
    QString Name;           //任务警员姓名
    QString TaskId;         //任务Id
    bool IsComplete;        //是否完成任务
    bool IsResponsible;     //是否责任人
    TaskItemList TaskItems; //任务清单子集
    QString No;             //出警任务警员编号
}TaskPolice;

void initModel(TaskPolice &item);
bool doDeSerialize(QString json, TaskPolice &item);
bool doDeSerialize(QtJson::JsonObject jsonData, TaskPolice &item);
QByteArray serialize(TaskPolice &item);

typedef QList<TaskPolice> TaskPoliceList;//任务警员模型列表

void initModel(TaskPoliceList &list);
bool doDeSerialize(QtJson::JsonArray array, TaskPoliceList &list);
QByteArray serialize(TaskPoliceList list);



/**
 ********************************************************************
 * 任务模型:
 ********************************************************************
*/

typedef struct {
    QString             Id;             //主键（GUID）
    QString             RoomId;         //监控室Id
    int                 Code;           //任务6位数字密文（用于短信回复）
    DataModel::TaskType TaskType;       //任务类型（出警、保养、紧急……）
    int                 TaskSubType;    //任务二级类型（涉赌等）
    int                 TaskStatus;     //任务状态（未审批，已审批……）
    QString             PoliceId;       //申请任务警员Id
    unsigned long long  StartTime;      //任务开始时间
    unsigned long long  EndTime;        //任务结束时间
    QString             ApproveLeadId;  //审批领导Id
    TaskPoliceList      TaskPolices;    //任务执行警员子集
    unsigned long long  AddTime;        //添加时间
}Task;

void initModel(Task &item);
bool doDeSerialize(QString json, Task &item);
bool doDeSerialize(QtJson::JsonObject jsonData, Task &item);
QByteArray serialize(Task &item);

typedef QList<Task> TaskList;//任务列表

void initModel(TaskList &list);
bool doDeSerialize(QtJson::JsonArray array, TaskList &list);
QByteArray serialize(TaskList list);

typedef struct {
    int pIndex;//任务警员位置
    int iIndex;//任务清单位置
}ItemPoint;

//根据任务清单ID，查找清单位置
ItemPoint findTaskItem(const Task &task, const QString &itemId);



/**
 ********************************************************************
 * 警员生物信息模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;             //主键
    QString     PoliceId;       //警员ID
    int         BioType;        //生物特征类型
    int         BioCheckType;   //生物特征验证类型
    int         FingerprintId;  //指纹仪产生的唯一ID
    QString     Key;            //特征数据
    QString     TemplateType;   //指纹仪模版类型
}PoliceBio;

void initModel(PoliceBio &item);
bool doDeSerialize(QString json, PoliceBio &item);
bool doDeSerialize(QtJson::JsonObject jsonData, PoliceBio &item);
QByteArray serialize(PoliceBio &item);

typedef QList<PoliceBio> PoliceBioList;

void initModel(PoliceBioList &list);
bool doDeSerialize(QtJson::JsonArray array, PoliceBioList &list);
QByteArray serialize(PoliceBioList list);



/**
 ********************************************************************
 * 警员模型:
 ********************************************************************
*/

typedef struct {
    QString     Id;                 //主键
    QString     Salt;               //盐值
    QString     Name;               //姓名
    QString     Photo;              //头像地址
    QString     PhotoBase64;        //头像Base编码
    QString     UnitId;             //单位ID
    QString     UnitName;           //单位名称
    QString     No;                 //警员编号（帐号）
    QString     Sex;                //性别
    int         Rank;               //警衔
    QString     Phone;              //移动电话
    QString     Nation;             //民族
    QString     Address;            //家庭住址
    int         Departy;            //政治面貌
    int         PoliceType;         //警员类型
    QString     TaskId;             //当前任务ID
    QString     Licence;            //持枪证
    QString     IDCardNo;           //身份证号码
    QString     Born;               //生日
    QString     GrantDept;          //发证机关
    QString     UserLifeBegin;      //有效开始日期
    QString     UserLifeEnd;        //有效截止日期
    QString     reserved;           //保留
    QString     PhotoFileName;      //身份证照片 Base64版本
    QString     PhotoFileNameImage; //身份证照片 路径版本
    bool        IsExistIdCard;      //是否上传了身份证信息
    PoliceBioList PoliceBios;       //生物特征数据子集
    unsigned long long AddTime;     //添加时间
}Police;

void initModel(Police &item);
bool doDeSerialize(QString json, Police &item);
bool doDeSerialize(QtJson::JsonObject jsonData, Police &item);
QByteArray serialize(Police &item);

typedef QList<Police> PoliceList;

void initModel(PoliceList &list);
bool doDeSerialize(QtJson::JsonArray array, PoliceList &list);
QByteArray serialize(PoliceList list);

bool isCManager(const PoliceList &list, const QString &policeId);



/**
 ********************************************************************
 * 系统日志:
 ********************************************************************
*/

typedef struct {
    QString             Id;             //主键（GUID）
    QString             RoomId;         //监控室Id
    QString             RoomName;       //监控室ID
    QString             CabId;          //枪柜Id
    QString             CabNo;          //枪柜编号
    int                 LogType;        //日志类型（报警、正常操作）
    int                 LogSubType;     //日志二级类型（因为xx原因报警）
    QString             ManageId;       //值班枪管员Id
    QString             LeadId;         //值班领导Id
    unsigned long long  LogTime;        //日志产生时间
    int                 LogStatus;      //日志状态（在警报的情况下可以选择已处理）
    QString             LogContent;     //日志的文字描述
    unsigned long long  AddTime;        //添加时间
}SystemLog;

void initModel(SystemLog &item);
bool doDeSerialize(QString json, SystemLog &item);
bool doDeSerialize(QtJson::JsonObject jsonData, SystemLog &item);
QByteArray serialize(SystemLog &item);

typedef QList<SystemLog> SystemLogList;

void initModel(SystemLogList &list);
bool doDeSerialize(QtJson::JsonArray array, SystemLogList &list);
QByteArray serialize(SystemLogList list);



/**
 ********************************************************************
 * 枪柜心跳包:
 ********************************************************************
*/

typedef struct {
    QString CabId;      //枪柜Id
    int     NetStatus;  //网络状态，心跳包
    int     EleStatus;  //电源状态，心跳包
    int     TempStatus; //温度状态，心跳包
}CabHeart;

void initModel(CabHeart &item);
bool doDeSerialize(QString json, CabHeart &item);
bool doDeSerialize(QtJson::JsonObject jsonData, CabHeart &item);
QByteArray serialize(CabHeart &item);



/**
 ********************************************************************
 * 专门的录入身份证信息的模型:
 ********************************************************************
*/

typedef struct {
    QString PoliceId;       //警员ID
    QString Name;           //姓名
    QString Sex;            //性别
    QString Nation;         //名族
    QString Address;        //家庭地址
    QString IDCardNo;       //身份证号码
    QString Born;           //生日
    QString GrantDept;      //发证机关
    QString UserLifeBegin;  //有效开始日期
    QString UserLifeEnd;    //有效截止日期
    QString reserved;       //保留
    QString PhotoFileName;  //身份证照片 Base64版本
}AddIdCard;

void initModel(AddIdCard &item);
bool doDeSerialize(QString json, AddIdCard &item);
bool doDeSerialize(QtJson::JsonObject jsonData, AddIdCard &item);
QByteArray serialize(AddIdCard &item);



/**
 ********************************************************************
 * 当前监控室执行清单模型:
 ********************************************************************
*/

typedef struct {
    QString RoomId;
    QString TaskId;
    QString TaskItemId;
}CurrentTaskItem;

void initModel(CurrentTaskItem &item);
bool doDeSerialize(QString json, CurrentTaskItem &item);
bool doDeSerialize(QtJson::JsonObject jsonData, CurrentTaskItem &item);
QByteArray serialize(CurrentTaskItem &item);

typedef QList<CurrentTaskItem> CurrentTaskItemList;

void initModel(CurrentTaskItemList &list);
bool doDeSerialize(QtJson::JsonArray array, CurrentTaskItemList &list);
QByteArray serialize(CurrentTaskItemList list);

}// namespace

#endif // DATAMODEL_H
