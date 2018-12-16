#include "database.h"
#include "databaseprivate.h"
#include <QMetaType>

using namespace DataModel;

Database* Database::m_instance = NULL;

Database* Database::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new Database(parent);
    return m_instance;
}

Database::Database(QObject *parent) : QThread(parent)
{
    pBase = new DatabasePrivate;

    //注册类型
    qRegisterMetaType<Database::DataCore>("Database::DataCore");

    //绑定任务执行槽
    connect(&taskTimer, SIGNAL(timeout()), this, SLOT(runTask()));
}

Database::~Database()
{
    delete ((DatabasePrivate*)pBase);
}

//[1] 连接数据库
bool Database::connectDb(QString path)
{
    return ((DatabasePrivate*)pBase)->connectDb(path);
}

//[2] 新建所有表
bool Database::createAllTable()
{
    return ((DatabasePrivate*)pBase)->createAllTable();
}




/**
 ********************************************************************
 * 警员模型:
 ********************************************************************
*/

void Database::addPolice(Police police, void *parent)
{
    TaskData task;
    task.taskType = AddPolice;
    task.parent = parent;
    task.police = police;
    addTask(task);
}

void Database::delPolice(QString Id, void *parent)
{
    TaskData task;
    task.taskType = DelPolice;
    task.parent = parent;
    task.Id = Id;
    addTask(task);
}

void Database::clrPolice(void *parent)
{
    TaskData task;
    task.taskType = CLRPolice;
    task.parent = parent;
    addTask(task);
}

void Database::findPolice(QString Id, void *parent)
{
    TaskData task;
    task.taskType = FindPolice;
    task.parent = parent;
    task.Id = Id;
    addTask(task);
}

void Database::findAllPolice(void *parent)
{
    TaskData task;
    task.taskType = FindAllPolice;
    task.parent = parent;
    addTask(task);
}

void Database::findPIdByIdCard(QString idCard, void *parent)
{
    TaskData task;
    task.taskType = FindPIdByIdCard;
    task.parent = parent;
    task.Id = idCard;
    addTask(task);
}

void Database::findPByRfid(QString rfid, void *parent)
{
    TaskData task;
    task.taskType = FindPByRfid;
    task.parent = parent;
    task.Id = rfid;
    addTask(task);
}

void Database::findPByFId(ushort fingerId, void *parent)
{
    TaskData task;
    task.taskType = FindPByFId;
    task.parent = parent;
    task.fingerId = fingerId;
    addTask(task);
}




/**
 ********************************************************************
 * 警员生物特征模型:
 ********************************************************************
*/

void Database::addPoliceBio(PoliceBio bio, void *parent)
{
    TaskData task;
    task.taskType = AddPoliceBio;
    task.parent = parent;
    task.bio = bio;
    addTask(task);
}

void Database::delPoliceBio(QString Id, void *parent)
{
    TaskData task;
    task.taskType = DelPoliceBio;
    task.parent = parent;
    task.Id = Id;
    addTask(task);
}

void Database::clrPoliceBio(void *parent)
{
    TaskData task;
    task.taskType = CLRPoliceBio;
    task.parent = parent;
    addTask(task);
}

void Database::findPIdByFingerId(ushort fingerId, void *parent)
{
    TaskData task;
    task.taskType = FindPIdByFId;
    task.parent = parent;
    task.fingerId = fingerId;
    addTask(task);
}

void Database::findAllPoliceBio(void *parent)
{
    TaskData task;
    task.taskType = FindAllPBio;
    task.parent = parent;
    addTask(task);
}

void Database::findBioByFingerId(ushort fingerId, void *parent)
{
    TaskData task;
    task.taskType = FindBioByFId;
    task.parent = parent;
    task.fingerId = fingerId;
    addTask(task);
}

void Database::findBiosByPId(QString policeId, void *parent)
{
    TaskData task;
    task.taskType = FindBiosByPId;
    task.parent = parent;
    task.Id = policeId;
    addTask(task);
}

void Database::initDataCore(DataCore &data)
{
    data.parent = NULL;
    data.success = false;
    initModel(data.police);
    data.policeList.clear();
    data.policeId.clear();
    initModel(data.bio);
    data.bioList.clear();
}





/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void Database::run()
{
    mutex.lock();

    initDataCore(m_data);
    DatabasePrivate *base = ((DatabasePrivate*)this->pBase);
    m_data.taskType = cTask.taskType;
    m_data.parent = cTask.parent;

    switch (cTask.taskType) {
    case AddPolice:         m_data.success = base->addPolice(cTask.police); break;
    case DelPolice:         m_data.success = base->delPolice(cTask.Id); break;
    case CLRPolice:         m_data.success = base->clearPolice(); break;
    case FindPolice:        m_data.success = base->findPolice(cTask.Id, m_data.police); break;
    case FindAllPolice:     m_data.success = base->findAllPolice(m_data.policeList); break;
    case FindPIdByIdCard:   m_data.success = base->findPIdByIdCard(cTask.Id, m_data.policeId); break;
    case FindPByRfid:       m_data.success = base->findPByRfid(cTask.Id, m_data.police); break;
    case FindPByFId:        m_data.success = base->findPByFId(cTask.fingerId, m_data.police); break;

    case AddPoliceBio:      m_data.success = base->addPoliceBio(cTask.bio); break;
    case DelPoliceBio:      m_data.success = base->delPoliceBio(cTask.Id); break;
    case CLRPoliceBio:      m_data.success = base->clearPoliceBio(); break;
    case FindPIdByFId:      m_data.success = base->findPIdByFingerId(cTask.fingerId, m_data.policeId); break;
    case FindAllPBio:       m_data.success = base->findAllPoliceBio(m_data.bioList); break;
    case FindBioByFId:      m_data.success = base->findBioByFingerId(cTask.fingerId, m_data.bio); break;
    case FindBiosByPId:     m_data.success = base->findBiosByPId(cTask.Id, m_data.bioList); break;
    default: break;
    }

    emit finished(m_data);

    mutex.unlock();
}

void Database::runTask()
{
    if(this->isRunning()) return;

    if( taskQueue.isEmpty() ){ taskTimer.stop(); return; }
    cTask = taskQueue.dequeue();
    this->start();
}

void Database::addTask(TaskData data)
{
    taskQueue.enqueue(data);
    if( !taskTimer.isActive() ) taskTimer.start(100);
}
