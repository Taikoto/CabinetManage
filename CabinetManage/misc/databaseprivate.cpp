#include "databaseprivate.h"

//表名
#define NAME_ROOM           "Room"
#define NAME_CABINET        "Cabinet"
#define NAME_SUBCABINET     "SubCabinet"
#define NAME_GUN            "Gun"
#define NAME_AMMO           "Ammo"
#define NAME_POLICE         "Police"
#define NAME_POLICEBIO      "PoliceBio"
#define NAME_TASK           "Task"
#define NAME_TASKITEM       "TaskItem"
#define NAME_TASKPOLICE     "TaskPolice"
#define NAME_TASKGUNOPER    "TaskGunOper"
#define NAME_TASKAMMOOPER   "TaskAmmoOper"
#define NAME_SYSTEMLOG      "SystemLog"


using namespace DataModel;


DatabasePrivate::DatabasePrivate()
{
}


bool DatabasePrivate::connectDb(QString path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    return db.open();
}

//新建所有表
bool DatabasePrivate::createAllTable()
{
    //新建警员表
    delTable(NAME_POLICE);
    if(!createPolice()) return false;
    //新建警员生物模型表
    delTable(NAME_POLICEBIO);
    if(!createPoliceBio()) return false;
    return true;
}

//删除表
bool DatabasePrivate::delTable(QString table)
{
    QSqlQuery query;
    query.prepare("DROP TABLE " + table);
    return query.exec();
}

//新建监控室表
bool DatabasePrivate::createRoom()
{
    QSqlQuery query;
#define ROOM_TABLE \
    "create table " NAME_ROOM "(" \
    "Id varchar(32) constraint pk_Id primary key" \
    ", UnitId varchar(32)" \
    ", UnitName varchar(32)" \
    ")"
    query.prepare(ROOM_TABLE);
#undef ROOM_TABLE
    return query.exec();
}

//新建警员表
bool DatabasePrivate::createPolice()
{
    QSqlQuery query;
#define ROOM_TABLE \
    "create table " NAME_POLICE "(" \
    "Id varchar(128) constraint pk_Id primary key" \
    ", Salt         varchar(128)" \
    ", Name         varchar(128)" \
    ", Photo        varchar(512)" \
    ", PhotoBase64  varchar(102400)" \
    ", UnitId       varchar(128)" \
    ", UnitName     varchar(128)" \
    ", No           varchar(128)" \
    ", Sex          varchar(32)" \
    ", Rank         int" \
    ", Phone        varchar(32)" \
    ", Nation       varchar(32)" \
    ", Address      varchar(512)" \
    ", Departy      int" \
    ", PoliceType   int" \
    ", TaskId       varchar(128)" \
    ", Licence      varchar(128)" \
    ", IDCardNo     varchar(32)" \
    ", Born         varchar(32)" \
    ", GrantDept    varchar(512)" \
    ", UserLifeBegin        varchar(32)" \
    ", UserLifeEnd          varchar(32)" \
    ", reserved             varchar(128)" \
    ", PhotoFileName        varchar(102400)" \
    ", PhotoFileNameImage   varchar(512)" \
    ", IsExistIdCard        bool" \
    ", AddTime              int64" \
    ")"
    query.prepare(ROOM_TABLE);
#undef ROOM_TABLE
    return query.exec();
}

//新建警员生物模型表
bool DatabasePrivate::createPoliceBio()
{
    QSqlQuery query;
#define ROOM_TABLE \
    "create table " NAME_POLICEBIO "(" \
    "Id varchar(128) constraint pk_Id primary key" \
    ", PoliceId         varchar(128)" \
    ", BioType          int" \
    ", BioCheckType     int" \
    ", FingerprintId    int" \
    ", Key              varchar(1024)" \
    ", TemplateType     varchar(128)" \
    ")"
    query.prepare(ROOM_TABLE);
#undef ROOM_TABLE
    return query.exec();
}



/**
 ********************************************************************
 * 警员模型:
 ********************************************************************
*/

bool DatabasePrivate::addPolice(const DataModel::Police &police)
{
#define SAVE_POLICE \
    "INSERT INTO " NAME_POLICE "(" \
    "Id, Salt, Name, Photo," \
    "PhotoBase64, UnitId, UnitName, No, Sex, Rank," \
    "Phone, Nation, Address, Departy, PoliceType, TaskId," \
    "Licence, IDCardNo, Born, GrantDept, UserLifeBegin," \
    "UserLifeEnd, reserved, PhotoFileName, PhotoFileNameImage," \
    "IsExistIdCard, AddTime )" \
    "values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) "
    QSqlQuery query;
    query.prepare(SAVE_POLICE);
#undef SAVE_POLICE
    query.addBindValue(police.Id);
    query.addBindValue(police.Salt);
    query.addBindValue(police.Name);
    query.addBindValue(police.Photo);
    query.addBindValue(police.PhotoBase64);
    query.addBindValue(police.UnitId);
    query.addBindValue(police.UnitName);
    query.addBindValue(police.No);
    query.addBindValue(police.Sex);
    query.addBindValue(police.Rank);
    query.addBindValue(police.Phone);
    query.addBindValue(police.Nation);
    query.addBindValue(police.Address);
    query.addBindValue(police.Departy);
    query.addBindValue(police.PoliceType);
    query.addBindValue(police.TaskId);
    query.addBindValue(police.Licence);
    query.addBindValue(police.IDCardNo);
    query.addBindValue(police.Born);
    query.addBindValue(police.GrantDept);
    query.addBindValue(police.UserLifeBegin);
    query.addBindValue(police.UserLifeEnd);
    query.addBindValue(police.reserved);
    query.addBindValue(police.PhotoFileName);
    query.addBindValue(police.PhotoFileNameImage);
    query.addBindValue(police.IsExistIdCard);
    query.addBindValue(police.AddTime);
    //if(!addPoliceBioList(police.PoliceBios)) return false;
    return query.exec();
}

bool DatabasePrivate::delPolice(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from " NAME_POLICE " where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

bool DatabasePrivate::clearPolice()
{
    QSqlQuery query;
    query.prepare("delete from " NAME_POLICE);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::Police &police, QSqlQuery &query)
{
    police.Id           = query.value(0).toString();
    police.Salt         = query.value(1).toString();
    police.Name         = query.value(2).toString();
    police.Photo        = query.value(3).toString();
    police.PhotoBase64  = query.value(4).toString();
    police.UnitId       = query.value(5).toString();
    police.UnitName     = query.value(6).toString();
    police.No           = query.value(7).toString();
    police.Sex          = query.value(8).toString();
    police.Rank         = query.value(9).toInt();
    police.Phone        = query.value(10).toString();
    police.Nation       = query.value(11).toString();
    police.Address      = query.value(12).toString();
    police.Departy      = query.value(13).toInt();
    police.PoliceType   = query.value(14).toInt();
    police.TaskId       = query.value(15).toString();
    police.Licence      = query.value(16).toString();
    police.IDCardNo     = query.value(17).toString();
    police.Born         = query.value(18).toString();
    police.GrantDept    = query.value(19).toString();
    police.UserLifeBegin = query.value(20).toString();
    police.UserLifeEnd  = query.value(21).toString();
    police.reserved     = query.value(22).toString();
    police.PhotoFileName = query.value(23).toString();
    police.PhotoFileNameImage = query.value(24).toString();
    police.IsExistIdCard = query.value(25).toBool();
    police.AddTime      = query.value(26).toULongLong();
    findBiosByPId(police.Id, police.PoliceBios);
}

bool DatabasePrivate::sqlFindPolice(QString sql, QVariant value, Police &police)
{
    QSqlQuery query;

    query.prepare(sql);
    query.addBindValue(value);
    bool ok = query.exec();
    while(query.next())
    {
        getModel(police, query);
    }
    return ok;
}

bool DatabasePrivate::findPolice(QString Id, Police &police)
{
    QString sql = "select * from " NAME_POLICE " where Id = ?";
    return sqlFindPolice(sql, Id, police);
}

bool DatabasePrivate::findAllPolice(PoliceList &list)
{
    QSqlQuery query;
    DataModel::Police police;

    query.prepare("select * from " NAME_POLICE);
    bool ok = query.exec();
    while(query.next())
    {
        getModel(police, query);
        list << police;
    }
    return ok;
}

bool DatabasePrivate::findPIdByIdCard(QString idCard, QString &pId)
{
    QSqlQuery query;

    query.prepare("select Id from " NAME_POLICE " where IDCardNo = ?");
    query.addBindValue(idCard);
    bool ok = query.exec();
    while(query.next())
    {
        pId = query.value(0).toString();
    }
    return ok;
}

bool DatabasePrivate::findPByRfid(QString rfid, Police &police)
{
    QString sql = "select * from " NAME_POLICE " where reserved = ?";
    return sqlFindPolice(sql, rfid, police);
}

bool DatabasePrivate::findPByFId(ushort fingerId, Police &police)
{
    if( !findPIdByFingerId(fingerId, police.Id) ) return false;
    return findPolice(police.Id, police);
}



/**
 ********************************************************************
 * 警员生物特征模型:
 ********************************************************************
*/

bool DatabasePrivate::addPoliceBio(const DataModel::PoliceBio &bio)
{
#define SAVE_POLICE_BIO \
    "insert into " NAME_POLICEBIO "(" \
    "Id, PoliceId, BioType, BioCheckType, " \
    "FingerprintId, Key, TemplateType )" \
    "values ( ?, ?, ?, ?, ?, ?, ?)"
    QSqlQuery query;
    query.prepare(SAVE_POLICE_BIO);
#undef SAVE_POLICE_BIO
    query.addBindValue(bio.Id);
    query.addBindValue(bio.PoliceId);
    query.addBindValue(bio.BioType);
    query.addBindValue(bio.BioCheckType);
    query.addBindValue(bio.FingerprintId);
    query.addBindValue(bio.Key);
    query.addBindValue(bio.TemplateType);
    return query.exec();
}

bool DatabasePrivate::addPoliceBioList(const DataModel::PoliceBioList &bios)
{
    bool ok = true;
    for(int i=0; i < bios.length(); i++)
        if( !addPoliceBio(bios[i]) ) ok = false;
    return ok;//当只要其中一个失败，就报失败
}

bool DatabasePrivate::delPoliceBio(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from " NAME_POLICEBIO " where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

bool DatabasePrivate::clearPoliceBio()
{
    QSqlQuery query;
    query.prepare("delete from " NAME_POLICEBIO);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::PoliceBio &bio, QSqlQuery &query)
{
    bio.Id              = query.value(0).toString();
    bio.PoliceId        = query.value(1).toString();
    bio.BioType         = query.value(2).toInt();
    bio.BioCheckType    = query.value(3).toInt();
    bio.FingerprintId   = query.value(4).toInt();
    bio.Key             = query.value(5).toString();
    bio.TemplateType    = query.value(6).toString();
}

//根据指纹ID，获取警员ID
bool DatabasePrivate::findPIdByFingerId(ushort fingerId, QString &pId)
{
    QSqlQuery query;

    query.prepare("select PoliceId from " NAME_POLICEBIO " where FingerprintId = ?");
    query.addBindValue(fingerId);
    bool ok = query.exec();
    while(query.next())
    {
        pId = query.value(0).toString();
    }
    return ok;
}

//查找所有警员指纹
bool DatabasePrivate::findAllPoliceBio(PoliceBioList &list)
{
    QSqlQuery query;
    DataModel::PoliceBio bio;

    query.prepare("select * from " NAME_POLICEBIO);
    bool ok = query.exec();
    while(query.next())
    {
        getModel(bio, query);
        list << bio;
    }
    return ok;
}

bool DatabasePrivate::findBioByFingerId(ushort fingerId, PoliceBio &bio)
{
    QSqlQuery query;

    query.prepare("select * from " NAME_POLICEBIO " where FingerprintId = ?");
    query.addBindValue(fingerId);
    bool ok = query.exec();
    while(query.next())
    {
        getModel(bio, query);
    }
    return ok;
}

bool DatabasePrivate::findBiosByPId(QString policeId, PoliceBioList &list)
{
    QSqlQuery query;
    DataModel::PoliceBio bio;

    query.prepare("select * from " NAME_POLICEBIO " where PoliceId = ?");
    query.addBindValue(policeId);
    bool ok = query.exec();
    while(query.next())
    {
        getModel(bio, query);
        list << bio;
    }
    return ok;
}



/**
 ********************************************************************
 * 任务模型:
 ********************************************************************
*/

bool DatabasePrivate::addTask(DataModel::Task &task)
{
#define SAVE_TASK \
    "insert into TaskModel ( Id, RoomId, Code," \
    "TaskType, TaskSubType, TaskStatus, PoliceId," \
    "StartTime, EndTime, ApproveLeadId, IsReport," \
    "AddTime ) " \
    "values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
    QSqlQuery query;
    query.prepare(SAVE_TASK);
#undef SAVE_TASK
    query.addBindValue(task.Id);
    query.addBindValue(task.RoomId);
    query.addBindValue(task.Code);
    query.addBindValue(task.TaskType);
    query.addBindValue(task.TaskSubType);
    query.addBindValue(task.TaskStatus);//已审批，已完成，第n阶段
    query.addBindValue(task.PoliceId);
    query.addBindValue(task.StartTime);
    query.addBindValue(task.EndTime);
    query.addBindValue(task.ApproveLeadId);
    query.addBindValue(task.AddTime);
    return query.exec();
}

bool DatabasePrivate::delTask(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from TaskModel where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::Task &task, QSqlQuery &query)
{
    task.Id             = query.value(0).toString();
    task.RoomId         = query.value(1).toString();
    task.Code           = query.value(2).toInt();
    task.TaskType       = TaskType(query.value(3).toInt());
    task.TaskSubType    = query.value(4).toInt();
    task.TaskStatus     = query.value(5).toInt();
    task.PoliceId       = query.value(6).toString();
    task.StartTime      = query.value(7).toULongLong();
    task.EndTime        = query.value(8).toULongLong();
    task.ApproveLeadId  = query.value(9).toString();
    task.AddTime        = query.value(11).toULongLong();
}

DataModel::Task DatabasePrivate::findTask(QString Id, bool *ok)
{
    QSqlQuery query;
    DataModel::Task task;
    if(ok != 0) *ok = false;

    query.prepare("select * from TaskModel where Id=?");
    query.addBindValue(Id);
    query.exec();
    while(query.next())
    {
        if(ok != 0) *ok = true;
        getModel(task, query);
    }
    return task;
}

DataModel::TaskList DatabasePrivate::findTaskList(QSqlQuery &query)
{
    DataModel::Task task;
    DataModel::TaskList list;

    query.exec();
    while(query.next())
    {
        getModel(task, query);
        list << task;
    }
    return list;
}

DataModel::TaskList DatabasePrivate::findExecTaskListByPoliceId(QString policeId)
{
    QSqlQuery query;
    query.prepare("select * from TaskModel where (TaskStatus = 4 and PoliceId = ?)");
    query.addBindValue(policeId);
    return findTaskList(query);
}

DataModel::TaskList DatabasePrivate::findTaskListByPoliceId(QString policeId)
{
    QSqlQuery query;
    query.prepare("select * from TaskModel where PoliceId = ?");
    query.addBindValue(policeId);
    return findTaskList(query);
}

int DatabasePrivate::findTaskCountByPoliceId(QString policeId)
{
    QSqlQuery query;
    query.prepare("select count(*) from TaskModel where PoliceId = ?");
    query.addBindValue(policeId);
    query.exec();
    while(query.next())
    {
        return query.value(0).toInt();
    }
    return 0;
}

bool DatabasePrivate::updateTaskStatusById(QString id, TaskStatus status)
{
    QSqlQuery query;
    query.prepare("update TaskModel set TaskStatus = ? where Id= ?");
    query.addBindValue((int)status);
    query.addBindValue(id);
    return query.exec();
}



/**
 ********************************************************************
 * 任务清单模型:
 ********************************************************************
*/

bool DatabasePrivate::addTaskItem(DataModel::TaskItem &item)
{
#define SAVE_TASK_ITEM \
    "insert into TaskItemModel ( Id, PoliceId, " \
    "TaskId, TaskItemType, TaskItemStatus, ObjectTypeID, " \
    "ObjectNumber, Title, OneOperNumber, TwoOperNumber )" \
    "values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    QSqlQuery query;
    query.prepare(SAVE_TASK_ITEM);
#undef SAVE_TASK_ITEM
    query.addBindValue(item.Id);
    query.addBindValue(item.TaskPoliceId);
    query.addBindValue(item.TaskItemType);
    query.addBindValue(item.TaskItemStatus);
    query.addBindValue(item.ObjectTypeId);
    query.addBindValue(item.ObjectNumber);
    query.addBindValue(item.Title);
    query.addBindValue(item.OneOperNumber);
    query.addBindValue(item.TwoOperNumber);
    return query.exec();
}

bool DatabasePrivate::addTaskItemList(DataModel::TaskItemList &list)
{
    bool ok = true;
    for(int i=0; i < list.length(); i++)
        if(!addTaskItem(list[i])) ok = false;
    return ok;//当只要其中一个失败，就报失败
}

bool DatabasePrivate::delTaskItem(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from TaskItemModel where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

bool DatabasePrivate::updateTwoOperNumber(QString Id, int number)
{
    QSqlQuery query;
    query.prepare("update TaskItemModel set TwoOperNumber = ? where Id = ?");
    query.addBindValue(number);
    query.addBindValue(Id);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::TaskItem &item, QSqlQuery &query)
{
    item.Id             = query.value(0).toString();
    item.TaskPoliceId   = query.value(1).toString();
    item.TaskItemType   = query.value(3).toInt();
    item.TaskItemStatus = TaskItemStatus(query.value(4).toInt());
    item.ObjectTypeId   = query.value(5).toInt();
    item.ObjectNumber   = query.value(6).toInt();
    item.Title          = query.value(7).toString();
    item.OneOperNumber  = query.value(8).toInt();
    item.TwoOperNumber  = query.value(9).toInt();
}

DataModel::TaskItemList DatabasePrivate::findTaskItemList(QString taskId)
{
    QSqlQuery query;
    DataModel::TaskItem item;
    DataModel::TaskItemList list;

    query.prepare("select * from TaskItemModel where TaskId = ?");
    query.addBindValue(taskId);
    query.exec();
    while(query.next())
    {
        getModel(item, query);
        item.OutGunOpers = findTaskGunOperList(item.Id, Out);
        item.OutAmmoOpers = findTaskAmmoOperList(item.Id, Out);
        item.InGunOpers = findTaskGunOperList(item.Id, In);
        item.InAmmoOpers = findTaskAmmoOperList(item.Id, In);
        list.append(item);
    }
    return list;
}



/**
 ********************************************************************
 * 任务枪支操作模型:
 ********************************************************************
*/

bool DatabasePrivate::addTaskGunOper(DataModel::TaskGunOper &oper)
{
#define SAVE_TASK_GUN_OPER \
    "insert into TaskGunOperModel ( Id, ManageId, CabId, " \
    "SubCabId, TaskItemId, ObjectTypeId, GunId, GunEno, " \
    "OperType, OperNumber, AddTime, GunNo ) " \
    "values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
    QSqlQuery query;
    query.prepare(SAVE_TASK_GUN_OPER);
#undef SAVE_TASK_GUN_OPER
    query.addBindValue(oper.Id);
    query.addBindValue(oper.ManageId);
    query.addBindValue(oper.CabId);
    query.addBindValue(oper.SubCabId);
    query.addBindValue(oper.TaskItemId);
    query.addBindValue(oper.ObjectTypeId);
    query.addBindValue(oper.GunId);
    query.addBindValue(oper.GunEno);
    query.addBindValue(oper.OperType);
    query.addBindValue(oper.OperNumber);
    query.addBindValue(oper.AddTime);
    query.addBindValue(oper.GunNo);
    return query.exec();
}

bool DatabasePrivate::delTaskGunOper(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from TaskGunOperModel where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::TaskGunOper &oper, QSqlQuery &query)
{
    oper.Id         = query.value(0).toString();
    oper.ManageId   = query.value(1).toString();
    oper.CabId      = query.value(2).toString();
    oper.SubCabId   = query.value(3).toString();
    oper.TaskItemId = query.value(4).toString();
    oper.ObjectTypeId = query.value(5).toInt();
    oper.GunId      = query.value(6).toString();
    oper.GunEno     = query.value(7).toString();
    oper.OperType   = query.value(8).toInt();
    oper.OperNumber = query.value(9).toInt();
    oper.AddTime    = query.value(10).toULongLong();
    oper.GunNo      = query.value(11).toString();
}

DataModel::TaskGunOperList DatabasePrivate::findTaskGunOperList(QString taskItemId,OperType type)
{
    QSqlQuery query;
    DataModel::TaskGunOper item;
    DataModel::TaskGunOperList list;

    query.prepare("select * from TaskGunOperModel where (TaskItemId = ? and OperType =?)");
    query.addBindValue(taskItemId);
    query.addBindValue((int)type);
    query.exec();
    while(query.next())
    {
        getModel(item, query);
        list.append(item);
    }
    return list;
}



/**
 ********************************************************************
 * 任务弹药操作模型:
 ********************************************************************
*/

bool DatabasePrivate::addTaskAmmoOper(DataModel::TaskAmmoOper &oper)
{
#define SAVE_TASK_AMMO_OPER \
    "insert into TaskAmmoOperModel ( Id, ManageId, CabId, " \
    "SubCabId, TaskItemId, ObjectTypeId, AmmoId, OperType," \
    "OperNumber, AddTime) " \
    "values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    QSqlQuery query;
    query.prepare(SAVE_TASK_AMMO_OPER);
#undef SAVE_TASK_AMMO_OPER
    query.addBindValue(oper.Id);
    query.addBindValue(oper.ManageId);
    query.addBindValue(oper.CabId);
    query.addBindValue(oper.SubCabId);
    query.addBindValue(oper.TaskItemId);
    query.addBindValue(oper.ObjectTypeId);
    query.addBindValue(oper.AmmoId);
    query.addBindValue(oper.OperType);
    query.addBindValue(oper.OperNumber);
    query.addBindValue(oper.AddTime);
    return query.exec();
}

bool DatabasePrivate::delTaskAmmoOper(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from TaskAmmoOperModel where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::TaskAmmoOper &oper, QSqlQuery &query)
{
    oper.Id         = query.value(0).toString();
    oper.ManageId   = query.value(1).toString();
    oper.CabId      = query.value(2).toString();
    oper.SubCabId   = query.value(3).toString();
    oper.TaskItemId = query.value(4).toString();
    oper.ObjectTypeId = query.value(5).toInt();
    oper.AmmoId     = query.value(6).toString();
    oper.OperType   = query.value(7).toInt();
    oper.OperNumber = query.value(8).toInt();
    oper.AddTime    = query.value(9).toULongLong();
}

DataModel::TaskAmmoOperList DatabasePrivate::findTaskAmmoOperList(QString taskItemId,OperType type)
{
    QSqlQuery query;
    DataModel::TaskAmmoOperList list;
    DataModel::TaskAmmoOper item;

    query.prepare("select * from TaskAmmoOperModel where (TaskItemId = ? and OperType= ?)");
    query.addBindValue(taskItemId);
    query.addBindValue((int)type);
    query.exec();
    while(query.next())
    {
        getModel(item, query);
        list.append(item);
    }
    return list;
}



/**
 ********************************************************************
 * 系统日志:
 ********************************************************************
*/

bool DatabasePrivate::addSystemLog(DataModel::SystemLog &log)
{
#define SAVE_SYSTEM_LOG \
    "insert into SystemLogModel ( Id, RoomId, RoomName, " \
    "CabId, CabNo, LogType, LogSubType, ManageId, LeadId, " \
    "LogTime, LogStatus, LogContent, AddTime ) " \
    "values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    QSqlQuery query;
    query.prepare(SAVE_SYSTEM_LOG);
#undef SAVE_SYSTEM_LOG
    query.addBindValue(log.Id);
    query.addBindValue(log.RoomId);
    query.addBindValue(log.RoomName);
    query.addBindValue(log.CabId);
    query.addBindValue(log.CabNo);
    query.addBindValue(log.LogType);
    query.addBindValue(log.LogSubType);
    query.addBindValue(log.ManageId);
    query.addBindValue(log.LeadId);
    query.addBindValue(log.LogTime);
    query.addBindValue(log.LogStatus);
    query.addBindValue(log.LogContent);
    query.addBindValue(log.AddTime);
    return query.exec();
}

bool DatabasePrivate::delSystemLog(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from SystemLogModel where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

void DatabasePrivate::getModel(DataModel::SystemLog &log, QSqlQuery &query)
{
    log.Id          = query.value(0).toString();
    log.RoomId      = query.value(1).toString();
    log.RoomName    = query.value(2).toString();
    log.CabId       = query.value(3).toString();
    log.CabNo       = query.value(4).toString();
    log.LogType     = query.value(5).toUInt();
    log.LogSubType  = query.value(6).toUInt();
    log.ManageId    = query.value(7).toString();
    log.LeadId      = query.value(8).toString();
    log.LogTime     = query.value(9).toULongLong();
    log.LogStatus   = query.value(10).toUInt();
    log.LogContent  = query.value(11).toString();
    log.AddTime     = query.value(12).toULongLong();
}

DataModel::SystemLog DatabasePrivate::findSystemLog(QString Id, bool *ok)
{
    QSqlQuery query;
    DataModel::SystemLog log;
    if(ok != 0) *ok = false;

    query.prepare("select * from SystemLogModel where Id=?");
    query.addBindValue(Id);
    query.exec();
    while(query.next())
    {
        if(ok != 0) *ok = true;
        getModel(log, query);
    }
    return log;
}

DataModel::SystemLogList DatabasePrivate::findSystemLogList(QString RoomId, QString CabId, LogType type)
{
    QSqlQuery query;
    DataModel::SystemLogList list;
    DataModel::SystemLog log;

    query.prepare("select * from SystemLogModel where (RoomId = ? and CabId = ? and LogType = ?)");
    query.addBindValue(RoomId);
    query.addBindValue(CabId);
    query.addBindValue((int)type);
    query.exec();
    while(query.next())
    {
        getModel(log, query);
        list.append(log);
    }
    return list;
}

DataModel::SystemLogList DatabasePrivate::findSystemLogList(QString RoomId, QString CabId)
{
    QSqlQuery query;
    DataModel::SystemLogList list;
    DataModel::SystemLog log;

    query.prepare("select * from SystemLogModel where (RoomId = ? and CabId = ?)");
    query.addBindValue(RoomId);
    query.addBindValue(CabId);
    query.exec();
    while(query.next())
    {
        getModel(log, query);
        list.append(log);
    }
    return list;
}

int DatabasePrivate::findSystemLogCount()
{
    QSqlQuery query;
    query.prepare("select count(*) from SystemLogModel");
    query.exec();
    while(query.next())
    {
        return query.value(0).toInt();
    }
    return 0;
}



/**
 ********************************************************************
 * 断网未上传信息（未写数据模型）:
 ********************************************************************
*/

bool DatabasePrivate::addUnUpload(QString type,QString Id)
{
    QSqlQuery query;
    query.prepare("insert into unUploadInfo(type, Id) values (?,?)");
    query.addBindValue(type);
    query.addBindValue(Id);
    return query.exec();
}

bool DatabasePrivate::delUnUpload(QString Id)
{
    QSqlQuery query;
    query.prepare("delete from unUploadInfo where Id = ?");
    query.addBindValue(Id);
    return query.exec();
}

QStringList DatabasePrivate::findUnUpload()
{
    QSqlQuery query;
    QStringList timlist;

    query.prepare("select * from unUploadInfo");
    query.exec();
    while(query.next())
    {
        timlist.append(query.value(0).toString());
        timlist.append(query.value(1).toString());
    }
    return timlist;
}

int DatabasePrivate::findUnUploadCount()
{
    QSqlQuery query;
    query.prepare("select count(*) from unUploadInfo");
    query.exec();
    while(query.next())
    {
        return query.value(0).toInt();
    }
    return 0;
}
