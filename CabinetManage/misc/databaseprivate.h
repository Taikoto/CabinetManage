#ifndef DATABASEPRIVATE_H
#define DATABASEPRIVATE_H

#include "datamodel.h" //Json数据模型

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtSql>

class DatabasePrivate
{
public:
    DatabasePrivate();

    bool connectDb(QString path);//连接数据库

    bool createAllTable();//新建所有表
    bool delTable(QString table);//删除表
    bool createRoom();//新建监控室表
    bool createPolice();//新建警员表
    bool createPoliceBio();//新建警员生物模型表

    /**
     ********************************************************************
     * 警员模型:
     ********************************************************************
    */
    bool addPolice(const DataModel::Police &police);
    bool delPolice(QString Id);
    bool clearPolice();
    void getModel(DataModel::Police &police, QSqlQuery &query);
    bool sqlFindPolice(QString sql, QVariant value, DataModel::Police &police);//in sql语句，out Police
    bool findPolice(QString Id, DataModel::Police &police);
    bool findAllPolice(DataModel::PoliceList &list);
    bool findPIdByIdCard(QString idCard, QString &pId);//根据身份证ID查找警员ID
    bool findPByRfid(QString rfid, DataModel::Police &police);//根据RFID查找警员
    bool findPByFId(ushort fingerId, DataModel::Police &police);//根据指纹ID查找警员

    /**
     ********************************************************************
     * 警员生物特征模型:
     ********************************************************************
    */
    bool addPoliceBio(const DataModel::PoliceBio &bio);
    bool addPoliceBioList(const DataModel::PoliceBioList &bios);
    bool delPoliceBio(QString Id);
    bool clearPoliceBio();
    void getModel(DataModel::PoliceBio &bio, QSqlQuery &query);
    bool findPIdByFingerId(ushort fingerId, QString &pId);//根据指纹ID获取警员ID
    bool findAllPoliceBio(DataModel::PoliceBioList &list);//查找所有警员指纹
    bool findBioByFingerId(ushort fingerId, DataModel::PoliceBio &bio);//根据指纹ID查找生物特征模型
    bool findBiosByPId(QString policeId, DataModel::PoliceBioList &list);//根据警员ID查找其所有的生物特征模型

    /**
     ********************************************************************
     * 任务模型（2016-06-21 task没改）:
     ********************************************************************
    */
    bool addTask(DataModel::Task &task);
    bool delTask(QString Id);
    void getModel(DataModel::Task &task, QSqlQuery &query);
    DataModel::Task findTask(QString Id, bool *ok = 0);//根据任务ID，获取任务信息
    DataModel::TaskList findTaskList(QSqlQuery &query);//根据QSqlQuery，查询任务列表
    DataModel::TaskList findExecTaskListByPoliceId(QString policeId);//根据警员ID，获取正在执行的所有任务
    DataModel::TaskList findTaskListByPoliceId(QString policeId);//根据警员ID，获取其所有任务
    int findTaskCountByPoliceId(QString policeId);//根据警员ID查找任务总数
    bool updateTaskStatusById(QString id, DataModel::TaskStatus status);//跟据ID，更新任务状态

    /**
     ********************************************************************
     * 任务清单模型:
     ********************************************************************
    */
    bool addTaskItem(DataModel::TaskItem &item);
    bool addTaskItemList(DataModel::TaskItemList &list);
    bool delTaskItem(QString Id);
    bool updateTwoOperNumber(QString Id, int number);//根据任务清单ID，更新第二阶段的执行的数据条数
    void getModel(DataModel::TaskItem &item, QSqlQuery &query);
    DataModel::TaskItemList findTaskItemList(QString taskId);//根据任务ID，获取任务清单列表

    /**
     ********************************************************************
     * 任务枪支操作模型:
     ********************************************************************
    */
    bool addTaskGunOper(DataModel::TaskGunOper &oper);
    bool delTaskGunOper(QString Id);
    void getModel(DataModel::TaskGunOper &oper, QSqlQuery &query);
    DataModel::TaskGunOperList findTaskGunOperList(QString taskItemId,DataModel::OperType type);

    /**
     ********************************************************************
     * 任务弹药操作模型:
     ********************************************************************
    */
    bool addTaskAmmoOper(DataModel::TaskAmmoOper &oper);
    bool delTaskAmmoOper(QString Id);
    void getModel(DataModel::TaskAmmoOper &oper, QSqlQuery &query);
    DataModel::TaskAmmoOperList findTaskAmmoOperList(QString taskItemId,DataModel::OperType type);

    /**
     ********************************************************************
     * 系统日志:
     ********************************************************************
    */
    bool addSystemLog(DataModel::SystemLog &log);
    bool delSystemLog(QString Id);
    void getModel(DataModel::SystemLog &log, QSqlQuery &query);
    DataModel::SystemLog findSystemLog(QString Id, bool *ok = 0);
    //根据监控室ID、枪柜ID、日志类型，查找日志列表
    DataModel::SystemLogList findSystemLogList(QString RoomId, QString CabId, DataModel::LogType type);
    //根据监控室ID、枪柜ID，查找日志列表
    DataModel::SystemLogList findSystemLogList(QString RoomId, QString CabId);
    int findSystemLogCount();//查询日志数

    /**
     ********************************************************************
     * 断网未上传信息（未写数据模型）:
     ********************************************************************
    */
    bool addUnUpload(QString type,QString Id);//保存信息
    bool delUnUpload(QString Id);
    QStringList findUnUpload();//查找没有上传的信息
    int findUnUploadCount();//查找断网未上传的总数
private:
    QSqlDatabase db;
};

#endif // DATABASEPRIVATE_H
