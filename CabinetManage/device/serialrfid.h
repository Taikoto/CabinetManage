#ifndef SERIALRFID_H
#define SERIALRFID_H

#include <QThread>
#include <QQueue>

#include "serialbase.h"

/*! RFID通信协议v1.3（ISO14443A+15693）.pdf
 * 1.1 波特率默认9600
 * 1.2 数据包格式
 *   STX  STATION_ID  DATA_LENGTH  CMD  DATA[0...N]     BCC        ETX
 *   0x02    0x00      CMD+DATA                                    0x03
 * 起始字节  设备地址      数据长度   命令字    数据     校验字节（异或） 终止字节
*/

// QThread要放在前面
class SerialRfid : public QThread, public SerialBase
{
    Q_OBJECT
public:
    static SerialRfid* instance(QObject *parent = 0);
private:
    static SerialRfid* m_instance;

public:
    explicit SerialRfid(QObject *parent = 0);
    ~SerialRfid();

    //设备类型
    enum DevType {
        ReadWrite   //读写器
        , ReadOnly  //只读
    };

    //错误代码
    enum ErrorCode {
        Success = 0x00
        , OpenSerialFailed      //打开串口失败
        , NoResponse            //模块未响应
        , DataError             //数据错误
        , UserCanceled          //用户取消
        , MultipleCards         //多张卡
        , NotFoundCard = 0x83   //没有找到卡
    };

    // 任务类型
    enum TaskType {
        None = 0
        , TestPort      //测试串口
        , GetCardId     //获取卡号
        , SetBaudrate   //设置波特率
    };

    // 任务数据
    typedef struct {
        TaskType type;
        void* parent;//保存任务的创建者，用于任务完成时的判断
        QString portName;
        uchar baudrate;//波特率
        DevType devType;//设备类型
    }TaskData;

    // 输出数据
    typedef struct {
        TaskType type;      //任务类型
        void* parent;       //任务的创建者，用于任务完成时的判断
        ErrorCode code;     //错误代码
        QString cardId;     //卡号
    }DevData;

    QString getErrorString(ErrorCode code);
    void setDeviceAddress(uchar address){ m_DevAddr = address; }

    void setROWTime(int ms){ m_roWTime = ms; }//! 设置只读设备的 寻卡等待时间
    int roWTime(){ return m_roWTime; }

public slots:
    //! 功能函数
    void stopTask();
    void testPort(const QString &name, void* parent = 0);           //[0] 测试串口
    void getCardId(DevType devType = ReadWrite, void* parent = 0);  //[1] 读取卡号
    void setDevBaudrate(uchar baud, void* parent = 0);              //设置波特率

private:
    int m_roWTime;//只读设备，等待刷卡时间
    uchar m_DevAddr;//设备地址
    uchar cCmd;//保存当前的命令字
    ErrorCode errCode;//命令返回的结果

    TaskData cTask;//当前任务
    QQueue<TaskData> taskQueue;//任务队列
    QTimer taskTimer;//任务启动定时器
    QMutex mutex;//线程锁定

    DevData m_data;     //函数数据

    QByteArray mkCmd(uchar cmd, uchar *dat = 0, uchar len = 0);
    QByteArray mkCmd(uchar cmd, uchar dat1);
    QByteArray mkCmd(uchar cmd, uchar dat1, uchar dat2);
    QByteArray mkCmd(uchar cmd, uchar dat1, uchar dat2, uchar dat3);
    ErrorCode sendCmd(QByteArray dat, int timeout);//发送命令串
    ErrorCode analyzeCmd(QByteArray &dat);//解析命令串

private slots:
    void readSerial();//读取串口数据
    void run();                 //[1] 线程函数
    void runTask();             //[2] 执行任务
    void addTask(TaskData data);//[3] 添加任务
signals:
    void finished(const SerialRfid::DevData data);
};

#endif // SERIALRFID_H
