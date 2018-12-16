#ifndef SERIALLOCK_H
#define SERIALLOCK_H

#include <QThread>
#include <QQueue>

#include "serialbase.h"

/*! 门锁485通信协议_140620.docx
 * 1.1 波特率默认9600
 * 1.2 数据包格式
 *  0x01    0x00    CMD+DATA
 * 目的地址  源地址    数据长度   命令字    数据     校验字节（异或）
*/

class SerialLock : public QThread, public SerialBase
{
    Q_OBJECT
public:
    static SerialLock* instance(QObject *parent = 0);
private:
    static SerialLock* m_instance;

public:
    explicit SerialLock(QObject *parent = 0);
    ~SerialLock();

    enum ErrorCode {
        Success = 0x00
        , OpenSerialFailed  //打开串口失败
        , NoResponse        //模块未响应
        , DataError         //数据错误
        , UnkonwnCmd        //未知指令
        , UserCanceled      //用户取消
        , TemperExceeded    //温度超出范围
        , TemperDevErr      //温度传感器线路不通或未接
        , SetOTimeFailed    //设置开锁最长时间失败
    };

    enum TaskType {
        None = 0
        , TestPort      //测试串口
        , Reset         //重置设备
        , Query         //查询
        , OpenLock      //开锁
        , SetOpenTime   //定义开锁最长时间（默认值为0X0A,0X06）
        , Warning       //报警
        , EXQuery       //扩展板触点查询
        , EXOpenLock    //扩展板开锁
        , EXLed         //扩展板发光管控制
        , EXLedTwinkle  //扩展板发光管闪烁控制
        , EXLedState    //读扩展板发光管状态
        , ReadWeight    //读电子秤测到的重量
        , SetScale      //设置电子秤
    };

    // input data
    typedef struct {
        TaskType type;
        void* parent;//保存任务的创建者，用于任务完成时的判断
        QString portName;
        uchar exNo;         //扩展板编号
        uchar lockNo;       //开锁的二进制编号
        uchar time;         //开锁通电时间
        bool warning;       //报警
        QByteArray dat;
    }TaskData;

    // output data
    typedef struct {
        TaskType type;      //任务类型
        void* parent;       //任务的创建者，用于任务完成时的判断
        ErrorCode code;     //错误代码
        uchar exNo;         //扩展板编号
        uchar lockNo;       //锁编号
        uchar qDat1;        //查询命令 0x21 的 data1
        ushort qEXState;    //查询命令 0x21 的 扩展板状态
        uchar qOpenTime;    //查询命令 0x21 的 门未打开时开锁的最长时间，以100ms为单位
        uchar qReTime;      //查询命令 0x21 的 门原来已打开时再次开锁时间，以100ms为单位
        uchar qTemper;      //查询命令 0x21 的 温度（200 超出范围，201 设备断开）
        uchar gunState;     //枪触点状态（1 打开，0 合上）
        uchar doorState;    //门触点状态（1 打开，0 合上）
        QByteArray dat;     //LED灯状态
        long weight;        //重量
    }DevData;

    void setDAddress(uchar address){ m_dAddr = address; }//设置目的地址
    void setSAddress(uchar address){ m_sAddr = address; }//设置源地址
    uchar dAddress(){ return m_dAddr; }
    uchar sAddress(){ return m_sAddr; }
    static QString getErrorString(ErrorCode code);

    //! 功能函数
    void stop();
    void testPort(const QString &name, void* parent = 0);       //[0] 测试串口
    void reset(void* parent = 0);                               //[1] 重置设备
    void query(void* parent = 0);                               //[2] 查询
    void openLock(uchar lockNo, uchar time, void* parent = 0);  //[3] 开锁
    void setOpenTime(uchar time, uchar reTime,void* parent = 0);//[4] 设置开锁最长时间
    void warning(bool enable, void* parent = 0);                //[5] 报警
    void exQuery(uchar exNo, void* parent = 0);                 //[6] 扩展板触点查询
    void exOpenLock(uchar exNo, uchar lockNo, uchar time, void* parent = 0);//[7] 扩展版开锁
    void exLed(QByteArray dat, void* parent = 0);               //[8] 扩展板发光管控制
    void exLedTwinkle(QByteArray dat, void* parent = 0);        //[9] 扩展板发光管闪烁控制命令，闪烁周期为1秒
    void exLedState(void* parent = 0);                          //[10] 读扩展板发光管状态
    void readWeight(uchar no, void* parent = 0);                //[11] 读电子秤测到的重量
    void setScale(QByteArray dat, void* parent = 0);            //[12] 设置电子秤
    // [0] 电子秤（最多9个）编号
    // [1] 需要读取的数据长度
    // [2-3] 所需数据在电子秤内的起始地址，0x0000-0x00f2
    // [...] DATA3-DATAn：设置电子秤的相应数据
    // 置零为：0x01020018FFFFFFFF(为置零第一个电子秤)

private:
    uchar m_dAddr;//目的地址
    uchar m_sAddr;//源地址
    ErrorCode errCode;//命令返回的结果

    TaskData cTask;//当前任务
    QQueue<TaskData> taskQueue;//任务队列
    QTimer taskTimer;//任务启动定时器

    DevData m_data;     //数据

    QByteArray mkCmd(uchar cmd, uchar *dat = 0, uchar len = 0);
    QByteArray mkCmd(uchar cmd, QByteArray dat);
    QByteArray mkCmd(uchar cmd, uchar dat1);
    QByteArray mkCmd(uchar cmd, uchar dat1, uchar dat2);
    QByteArray mkCmd(uchar cmd, uchar dat1, uchar dat2, uchar dat3);
    ErrorCode sendCmd(QByteArray dat, int timeout);//发送命令串
    ErrorCode handshake();//发送握手命令（目的地址）
    ErrorCode analyzeCmd(QByteArray &dat);//解析命令串

private slots:
    void readSerial();
    void run();
    void runTask();
    void addTask(TaskData data);
signals:
    void finished(const SerialLock::DevData data);
};

#endif // SERIALLOCK_H
