#ifndef VERIFY_P_H
#define VERIFY_P_H

#include <QObject>
#include <Device>

#include "config.h"
#include "network.h"
#include "dataloop.h"
#include "mediaplayer.h"
#include "datamodel.h"
#include "serialrfid.h"



/*! **************************************************************
    class VerifyBase : public QObject

    作者：Cat
    时间：2016-08-15
    描述：验证功能基础类，包含一些通用基础函数和功能类
    **************************************************************
*/

class VerifyBase : public QObject
{
    Q_OBJECT

public:
    VerifyBase(QObject *parent = 0);
    ~VerifyBase();

    bool isCManager(const QString &policeId);//判断是否是当前值班管理员

    Config *config;     //配置类
    DataLoop *dataLoop; //数据池
    MediaPlayer *player;//音乐播放类
    UsbFinger *uFinger; //USB指纹仪类
    SerialRfid *sRfid;  //串口RFID类
    QTimer verifyTimer; //验证定时器

signals:
    void message(QString);
    void getPolice(DataModel::Police police);

public slots:
    void startDevice();//启动身份验证设备
    void startDevice(int msec){ verifyTimer.start(msec); }
    void finished(const UsbFinger::DevData data);//USB指纹仪
    void finished(const SerialRfid::DevData data);//RFID
};









/*! **************************************************************
    class TaskVerify : public QObject

    作者：Cat
    时间：2016-08-15
    描述：任务验证流程代码集成类，验证枪支弹药管理员、警员
    **************************************************************
*/

class TaskVerify : public QObject
{
    Q_OBJECT

public:
    explicit TaskVerify(QObject *parent = 0);
    ~TaskVerify();

    VerifyBase *base;
    Network *network;
    QString mangerId;

signals:
    void message(QString);
    void finished(bool success, DataModel::Police police);

public slots:
    void start();
    void finished(const Network::DataCore data);
    void analysisPolice(DataModel::Police police);
    void verifyTaskPolice();
};









/*! **************************************************************
    class PoliceVerify : public QObject

    作者：Cat
    时间：2016-08-15
    描述：验证警员
    **************************************************************
*/

class PoliceVerify : public QObject
{
    Q_OBJECT

public:
    explicit PoliceVerify(QObject *parent = 0);
    ~PoliceVerify();

    VerifyBase *base;

signals:
    void message(QString);
    void finished(bool success, DataModel::Police police);

public slots:
    void start();
    void analysisPolice(DataModel::Police police);
};









/*! **************************************************************
    class ManagerVerify : public QObject

    作者：Cat
    时间：2016-08-18
    描述：验证枪支弹药管理员
    **************************************************************
*/

class ManagerVerify : public QObject
{
    Q_OBJECT

public:
    explicit ManagerVerify(QObject *parent = 0);
    ~ManagerVerify();

    VerifyBase *base;

signals:
    void message(QString);
    void finished(bool success, DataModel::Police police);

public slots:
    void start();
    void analysisPolice(DataModel::Police police);
};









/*! **************************************************************
    class DutyManagerVerify : public QObject

    作者：Cat
    时间：2016-08-18
    描述：验证值班枪支弹药管理员
    **************************************************************
*/

class DutyManagerVerify : public QObject
{
    Q_OBJECT

public:
    explicit DutyManagerVerify(QObject *parent = 0);
    ~DutyManagerVerify();

    VerifyBase *base;

signals:
    void message(QString);
    void finished(bool success, DataModel::Police police);

public slots:
    void start();
    void analysisPolice(DataModel::Police police);
};









/*! **************************************************************
    class LeaderVerify : public QObject

    作者：Cat
    时间：2016-08-18
    描述：验证值班枪支弹药管理员
    **************************************************************
*/

class LeaderVerify : public QObject
{
    Q_OBJECT

public:
    explicit LeaderVerify(QObject *parent = 0);
    ~LeaderVerify();

    VerifyBase *base;

signals:
    void message(QString);
    void finished(bool success, DataModel::Police police);

public slots:
    void start();
    void analysisPolice(DataModel::Police police);
};









/*! **************************************************************
    class EnrollFinger : public QObject

    作者：Cat
    时间：2016-08-18
    描述：录入指纹
    **************************************************************
*/

class EnrollFinger : public QObject
{
    Q_OBJECT

public:
    explicit EnrollFinger(QObject *parent = 0);
    ~EnrollFinger();

    Config *config;     //配置类
    DataLoop *dataLoop; //数据池
    MediaPlayer *player;//音乐播放类
    UsbFinger *uFinger; //USB指纹仪类

signals:
    void message(QString);
    void finished(bool success, DataModel::Police police);

public slots:
    void start();
    void finished(Network::TaskType);
    void finished(const UsbFinger::DevData data);//USB指纹仪
};

#endif // VERIFY_P_H
