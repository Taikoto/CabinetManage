#include "serialrfid.h"

#include <QMetaType>

#include "debug.h"

//! 命令字
#define CMD_HEADER          0x02    //命令串首字节
#define CMD_END             0x03    //命令串尾字节
// ISO14443 TYPE A Commands (0x03~0x06)
#define CMD_ReqA            0x03    //ISO14443-A 寻卡请求
#define CMD_AnticollA       0x04    //ISO14443-A 防冲突
#define CMD_SelectA         0x05    //ISO14443-A 选择卡
#define CMD_HaltA           0x06    //使卡进入 HALT 状态
// ISO14443 TYPE B Commands (0x09~0x0C)
#define CMD_ReqB            0x09    //ISO14443-B 寻卡命令
#define CMD_AnticollB       0x0A    //ISO14443-B 防冲突
#define CMD_AttribTypeB     0x0B    //ISO14443-B ATTRIB 命令
#define CMD_RstTypeB        0x0C    //集成了寻卡和ATTRIB命令通过此命令直接对卡进行复位
#define CMD_TransferB       0x0D    //ISO14443-B 可以通过此命令向卡发任意的命令、数据
// Mifare Application Commands (0x20~0x2F)
#define CMD_MFRead          0x20    //一个命令完成读卡操作
#define CMD_MFWrite         0x21    //一个命令完成写卡操作
#define CMD_MFInitVal       0x22    //一个命令完成块值初始化操作
#define CMD_MFDecrement     0x23    //一个命令完成块减值操作
#define CMD_MFIncrement     0x24    //一个命令完成块值加操作
#define CMD_MFGetSnr        0x25    //读取卡片序列号
#define CMD_TransferA       0x28    //ISO14443-A 可以通过此命令向卡发任意的命令、数据
// ISO15693 Commands (0x10~0x1D)
#define CMD_93Inventory     0x10    //寻卡、防冲突
#define CMD_93Read          0x11    //读卡
#define CMD_93Write         0x12    //写卡
#define CMD_93LockBlock     0x13    //锁定卡扇区内容操作
#define CMD_93StayQuiet     0x14    //将卡至于静止状态
#define CMD_93Select        0x15    //选择卡
#define CMD_93ResetReady    0x16    //使卡进入 ready 状态
#define CMD_93WriteAfi      0x17    //写AFI
#define CMD_93LockAfi       0x18    //锁定AFI
#define CMD_93WriteDsfid    0x19    //写DSFID
#define CMD_93LockDsfid     0x1A    //锁定DSFID
#define CMD_93GetInfo       0x1B    //获取卡信息
#define CMD_93GetMultip     0x1C    //获取块安全信息
#define CMD_93Transfer      0x1D    //可以通过此命令向卡发任意的命令、数据
// System Commands (0x80~0xFF)
#define CMD_SetAddress      0x80    //设置读写器地址
#define CMD_SetBaudrate     0x81    //设置通讯波特率
#define CMD_SetSerlNum      0x82    //设置读写器的序列号
#define CMD_GetSerlNum      0x83    //读取读写器的序列号
#define CMD_WriteUserInfo   0x84    //设置用户数据信息
#define CMD_ReadUserInfo    0x85    //读取用户数据信息
#define CMD_GetVersionNum   0x86    //用来读取模块的版本信息
#define CMD_ControlLed1     0x87    //控制LED1的工作方式
#define CMD_ControlLed2     0x88    //控制LED2的工作方式
#define CMD_ControlBuzzer   0x89    //控制Buzzer的工作方式

//! 生成命令串
// ISO14443 TYPE A Commands (0x03~0x06)
#define MC_ReqA(mode)                   mkCmd(CMD_ReqA,mode)
#define MC_AnticollA()                  mkCmd(CMD_AnticollA)
#define MC_SelectA()
#define MC_HaltA()
// ISO14443 TYPE B Commands (0x09~0x0C)
#define MC_ReqB()
#define MC_AnticollB()
#define MC_AttribTypeB()
#define MC_RstTypeB()
#define MC_TransferB()
// Mifare Application Commands(0x20~0x2F)
#define MC_MFGetSnr(mode,halt)          mkCmd(CMD_MFGetSnr,mode,halt)
// System Commands (0x80~0xFF)
#define MC_SetBaudrate(baud)            mkCmd(CMD_SetBaudrate,baud)

//! 发送命令
// Mifare Application Commands(0x20~0x2F)
#define SC_MFGetSnr(mode,halt,time)     sendCmd(MC_MFGetSnr(mode,halt),time)
// System Commands (0x80~0xFF)
#define SC_SetBaudrate(baud,time)       sendCmd(MC_SetBaudrate(baud),time)

SerialRfid* SerialRfid::m_instance = NULL;

SerialRfid* SerialRfid::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new SerialRfid(parent);
    return m_instance;
}



SerialRfid::SerialRfid(QObject *parent) :
    QThread(parent)
{
    m_DevAddr = 0x00;//设备地址默认 0x00

    connect(&sTimer, SIGNAL(timeout()), this, SLOT(readSerial()));
    sTimer.start(100);

    serial.setBaudRate(BAUD9600);

    m_roWTime = 10000;//默认10秒

    //注册类型
    qRegisterMetaType<SerialRfid::DevData>("SerialRfid::DevData");

    //绑定任务执行槽
    connect(&taskTimer, SIGNAL(timeout()), this, SLOT(runTask()));
}

SerialRfid::~SerialRfid()
{
    stopTask();
}

QString SerialRfid::getErrorString(ErrorCode code)
{
    switch (code) {
    case Success:           return tr("Executed successfully.");//成功
    case OpenSerialFailed:  return tr("Serial port open failed.") + portName();//打开串口失败
    case NoResponse:        return tr("Module not responding.");//模块未响应
    case DataError:         return tr("Get data error.");
    case UserCanceled:      return tr("Task canceled by the user.");
    case MultipleCards:     return tr("Multiple cards");
    case NotFoundCard:      return tr("Not found cards");
    default:    break;
    }
    return tr("Unknown error.");
}

void SerialRfid::stopTask()
{
    waitData.wakeOne(); //激活线程
    taskQueue.clear();
    m_data.code = UserCanceled;
}

//[0] 串口测试
void SerialRfid::testPort(const QString &name, void* parent)
{
    TaskData task;
    task.type = TestPort;
    task.parent = parent;
    task.portName = name;
    addTask(task);
}

//[1] 读取卡号
void SerialRfid::getCardId(DevType devType, void* parent)
{
    TaskData task;
    task.type = GetCardId;
    task.parent = parent;
    task.devType = devType;
    addTask(task);
}

//设置波特率
void SerialRfid::setDevBaudrate(uchar baud, void *parent)
{
    TaskData task;
    task.type = SetBaudrate;
    task.parent = parent;
    task.baudrate = baud;
    addTask(task);
}



/**
 ********************************************************************
 * private:
 ********************************************************************
*/

QByteArray SerialRfid::mkCmd(uchar cmd, uchar *dat, uchar len)
{
    uchar sum = 0;
    QByteArray cmds;

    cmds.append(CMD_HEADER);// 0    STX
    cmds.append(m_DevAddr); // 1    STATION_ID
    cmds.append(len + 1);   // 2    DATA_LENGTH
    cmds.append(cmd);       // 3    CMD

    // DATA[0...N]
    for( int i = 0; i < len; i++ ) cmds.append(dat[i]);

    //校验位 异或（不包括首尾字节）
    for(int i = 1; i < cmds.length(); i++) sum ^= cmds[i]&0xFF;

    cmds.append(sum);
    cmds.append(CMD_END);
    return cmds;
}

QByteArray SerialRfid::mkCmd(uchar cmd, uchar dat1)
{
    return mkCmd(cmd, &dat1, 1);
}

QByteArray SerialRfid::mkCmd(uchar cmd, uchar dat1, uchar dat2)
{
    uchar dat[2] = { dat1, dat2 };
    return mkCmd(cmd, dat, 2);
}

QByteArray SerialRfid::mkCmd(uchar cmd, uchar dat1, uchar dat2, uchar dat3)
{
    uchar dat[3] = { dat1, dat2, dat3 };
    return mkCmd(cmd, dat, 3);
}

//发送命令
SerialRfid::ErrorCode SerialRfid::sendCmd(QByteArray dat, int timeout)
{
    serial.write(dat);
    if(isDebug) debug16(dat, "Send data");

    cCmd = (uchar)dat[3];//第4个为命令字

    if(!waitData.wait(&mutex, timeout)) return NoResponse;
    return errCode;
}

//分析接收到的数据
SerialRfid::ErrorCode SerialRfid::analyzeCmd(QByteArray &dat)
{
    if(cTask.type == TestPort) return Success;

    switch (cCmd) {//第4个为命令字
    case CMD_MFGetSnr://读取卡片序列号
        // dat[3] 为状态字，0x00 OK, 0x01 Fail
        if((uchar)dat[3] == 0x00){
            if((uchar)dat[4] == 0x00){
                //获取到卡号
                m_data.cardId.clear();
                for( int i = 0; i < 4; i++ )
                    m_data.cardId += QString("%1").arg(dat[i+5]&0xFF, 2, 16, QLatin1Char('0'));
                qDebug() << "Debug SerialRfid: Card id" << m_data.cardId;
                return Success;
            }
            else if((uchar)dat[4] == 0x01) return MultipleCards;//多张卡
            else return DataError;
        }
        else if((uchar)dat[3] == 0x01){
            return (ErrorCode)((int)dat[4]);//获取失败代码
        }
        else return DataError;
    }
    return DataError;
}



/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void SerialRfid::readSerial()
{
    static QByteArray datTemp;

    if(serial.bytesAvailable() <= 0) return;
    QByteArray dat = serial.readAll();

    //累加前面缓存的数据
    if(!datTemp.isEmpty()){
        dat = datTemp + dat; datTemp.clear();
    }

    if(isDebug) debug16(dat, "Get data");

    //! 此处为 只读设备 的处理代码
    if(cTask.type == GetCardId && cTask.devType == ReadOnly){
        if(dat.length() == 14){
            //获取到卡号
            m_data.cardId.clear();
            for( int i = 0; i < 4; i++ )
                m_data.cardId += QString("%1").arg(int(dat[i+5]), 2, 16, QLatin1Char('0'));
            qDebug() << "Debug SerialRfid: Card id" << m_data.cardId;
            m_data.code = Success;
        } else { datTemp.clear(); m_data.code = DataError; }
        waitData.wakeOne(); //激活线程
        return;
    }

    if(dat.length() < 3){ datTemp = dat; return; } //防止 dat[2] 越界
    if(int(dat[2])+5 > dat.length()) { datTemp = dat; return; } // 没有接收完一串命令

    if(int(dat[2])+5 == dat.length()){   //完整的命令串
        uchar sum = 0;
        for(int i = 1; i < dat.length()-2; i++) sum ^= dat[i]&0xFF;//校验
        if(sum == (uchar)dat[dat.length()-2]) errCode = analyzeCmd(dat);//分析命令串
        else { datTemp.clear(); errCode = DataError; }//校验位不相等，数据可能损坏
    }
    else { datTemp.clear(); errCode = DataError; }

    waitData.wakeOne(); //激活线程
}

//[1] 线程函数
void SerialRfid::run()
{
    mutex.lock();

    //直接传递的外部数据
    m_data.type = cTask.type;//任务类型
    m_data.parent = cTask.parent;//任务创建者

    //开始 测试串口任务 之前再设置串口端口
    if(cTask.type == TestPort) setPortName(cTask.portName);

    //串口未打开时，打开串口
    if(!serial.isOpen()){
        if(!openSerial()){ m_data.code = OpenSerialFailed; goto End; }
    }

    switch (cTask.type) {
    case TestPort:      m_data.code = SC_MFGetSnr(0x26, 0x00, cmdWTime); break;
    case GetCardId:
        //! 只读设备 与 读写设备不同
        if(cTask.devType == ReadWrite) m_data.code = SC_MFGetSnr(0x26, 0x00, cmdWTime);
        else if(!waitData.wait(&mutex, m_roWTime)) m_data.code = NotFoundCard;
        break;
    case SetBaudrate:   m_data.code = SC_SetBaudrate(cTask.baudrate, cmdWTime); break;
    default: break;
    }

    //若一组任务完成后，关闭串口
    if(taskQueue.isEmpty()) serial.close();

End:
    emit finished(m_data);
    mutex.unlock();
}

//[2] 执行任务
void SerialRfid::runTask()
{
    if(this->isRunning()) return;

    if( taskQueue.isEmpty() ){ taskTimer.stop(); return; }
    cTask = taskQueue.dequeue();
    this->start();
}

//[3] 添加任务
void SerialRfid::addTask(TaskData data)
{
    if( !taskTimer.isActive() ) taskTimer.start(50);
    taskQueue.enqueue(data);
}
