#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QStringList>

class Config : public QObject
{
    Q_OBJECT
public:
    Config(QObject *parent = 0);
    ~Config();

    static Config* instance(QObject *parent = 0);

    void createFile(); // create config.ini file

public slots:
    void readConfig(); // read config.ini file

public:
    // HTTP
    QString serverIp;   // web server ip
    QString qServerIp;  // my qt server ip
    qint16 qPost;       // my qt server port
    bool isQServer;     // enable my qt server
    // GET API
    QString apiGetTime;
    QString apiGetTypeList;
    QString apiGetCabList;
    QString apiGetCabById;
    QString apiGetRoomList;
    QString apiGetRoomById;
    QString apiGetRoomCTaskItem;
    QString apiGetTaskById;
    QString apiGetTaskByPId;
    QString apiGetPoliceById;
    QString apiGetPoliceList;
    QString apiGetCurrentLead;
    QString apiGetCurrentManage;
    QString apiGetDelPoliceBio;
    QString apiGetQPoliceId;
    // POST API
    QString apiPostGunOut;
    QString apiPostGunIn;
    QString apiPostAmmoOut;
    QString apiPostAmmoIn;
    QString apiPostSystemLog;
    QString apiPostHeart;
    QString apiPostTask;
    QString apiPostTaskUrgent;
    QString apiPostPoliceBio;
    QString apiPostPoliceIdCard;
    QString apiPostRoomManage;
    QString apiPostRoomLead;
    // SYSTEM
    bool isArm4418;
    bool fullScreen;
    bool hideMouse;
    int lcdBackight;    //LCD背光亮度(0~255)
    int closeLcd;       //关闭LCD背光延时 单位：秒（实际存的毫秒）
    int updateData;     //更新数据间隔 单位：秒（实际存的毫秒）
    int updateTemper;   //更新温度间隔 单位：秒（实际存的毫秒）
    int verifyMaxTime;  //验证身份最长等待时间 单位：秒（实际存的毫秒）
    int vManagerNum;    //验证管理员数量
    bool localVerify;   //本机验证警员
    QString cabId;      //枪弹柜ID
    QString roomId;     //监控室ID
    QString unitId;     //单位ID
    QString openAllCard;
    // DEVICE
    QStringList portList; // uart port list
    QString arm4418Port;
    QString lockPort;
    QString fingerPort;
    QString idCardPort;
    QString rfidPort;
    QString upsPort;
    bool haveUSBFinger;     //是否有USB指纹仪
    bool haveRFID;          //是否有RFID读卡器
    bool haveIdCard;        //是否有身份证读卡器设备

    // HTTP
    void setServerIp(QString ip);
    // SYSTEM
    void setLcdBackight(int backight);
    void setCloseLcd(int sec);
    void setUpdateData(int sec);
    void setUpdateTemper(int sec);
    void setVManagerNum(int number);
    void setLocalVerify(bool enable);
    void setCabId(QString id);
    void setRoomId(QString id);
    void setOpenAllCard(QString id);
    // DEVICE
    void setLockPort(const QString &name); // set lock uart port
    void setHaveUSBFinger(const bool have);
    void setHaveRFID(const bool have);

private:
    QSettings *ini;
    static Config* m_instance;
};

#endif // CONFIG_H
