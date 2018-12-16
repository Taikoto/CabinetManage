#ifndef DATABASE_H
#define DATABASE_H

#include <QThread>
#include <QQueue>
#include <QTimer>
#include <QMutex>

#include "datamodel.h"

class Database : public QThread
{
    Q_OBJECT
public:
    static Database* instance(QObject *parent = 0);
private:
    static Database* m_instance;

public:
    Database(QObject *parent = 0);
    ~Database();

    //任务类型
    enum TaskType {
        None = 0
        , AddPolice
        , DelPolice
        , CLRPolice
        , FindPolice
        , FindAllPolice     // get all police
        , FindPIdByIdCard   // find police by card id
        , FindPByRfid       // find police by rfid id
        , FindPByFId        // find police by finger id

        , AddPoliceBio
        , DelPoliceBio
        , CLRPoliceBio
        , FindPIdByFId      // find police id by finger id
        , FindAllPBio       // get all police bio
        , FindBioByFId      // find bio by finger id
        , FindBiosByPId     // find bios by police id
    };

    typedef struct {
        TaskType taskType;
        void* parent; // task creator
        DataModel::Police police;
        QString Id;
        DataModel::PoliceBio bio;
        ushort fingerId;
    }TaskData;

    typedef struct {
        TaskType taskType;
        void* parent; // task creator
        bool success;
        DataModel::Police police;
        DataModel::PoliceList policeList;
        QString policeId;
        DataModel::PoliceBio bio;
        DataModel::PoliceBioList bioList;
    }DataCore;

    bool connectDb(QString path);

    bool createAllTable();

    /**
     ********************************************************************
     * 警员模型:
     ********************************************************************
    */
    void addPolice(DataModel::Police police, void *parent = 0);
    void delPolice(QString Id, void *parent = 0);
    void clrPolice(void *parent = 0);
    void findPolice(QString Id, void *parent = 0);
    void findAllPolice(void *parent = 0);
    void findPIdByIdCard(QString idCard, void *parent = 0);
    void findPByRfid(QString rfid, void *parent = 0);
    void findPByFId(ushort fingerId, void *parent = 0);

    /**
     ********************************************************************
     * 警员生物特征模型:
     ********************************************************************
    */
    void addPoliceBio(DataModel::PoliceBio bio, void *parent = 0);
    void delPoliceBio(QString Id, void *parent = 0);
    void clrPoliceBio(void *parent = 0);
    void findPIdByFingerId(ushort fingerId, void *parent = 0);
    void findAllPoliceBio(void *parent = 0);
    void findBioByFingerId(ushort fingerId, void *parent = 0);
    void findBiosByPId(QString policeId, void *parent = 0);

signals:
    void finished(Database::DataCore data);

private:
    void *pBase;// DatabasePrivate

    TaskData cTask; // current task
    QQueue<TaskData> taskQueue;
    QTimer taskTimer;
    QMutex mutex;

    DataCore m_data;

    void initDataCore(DataCore &data);

private slots:
    void run();
    void runTask();
    void addTask(TaskData data);
};

#endif // DATABASE_H
