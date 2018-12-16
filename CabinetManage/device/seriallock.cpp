#include "seriallock.h"

#include <QMetaType>
#include <debug.h>

//默认值
#define DEFAULT_DA      0x01    //Destination address 目的地址
#define DEFAULT_SA      0x00    //source address 源地址

//命令字
#define CMD_RESET       0x20    //复位
#define CMD_QUERY       0x21    //查询
#define CMD_OPENLOCK    0x22    //开锁
#define CMD_SETOTIME    0x23    //设置开锁最长时间
#define CMD_WARNING     0x25    //报警控制
#define CMD_EXQUERY     0x26    //扩展板触点查询
#define CMD_EXOLOCK     0x27    //扩展板开锁
#define CMD_EXLED       0x28    //扩展板发光管控制
#define CMD_EXLEDTK     0x29    //扩展板发光管闪烁控制
#define CMD_EXLEDSTATE  0x2A    //读扩展板发光管状态
#define CMD_WEIGHT      0x2B    //读电子秤测到的重量
#define CMD_SCALEDATA   0x2C    //读取电子秤指定数据
#define CMD_SETSCALE    0x2D    //设置电子秤

//生成命令串
#define MC_RESET()                      mkCmd(CMD_RESET)
#define MC_QUERY()                      mkCmd(CMD_QUERY)
#define MC_OPENLOCK(dat1,dat2)          mkCmd(CMD_OPENLOCK,dat1,dat2)
#define MC_SETOTIME(dat1,dat2)          mkCmd(CMD_SETOTIME,dat1,dat2)
#define MC_WARNING(dat1)                mkCmd(CMD_WARNING,dat1)
#define MC_EXQUERY(dat1)                mkCmd(CMD_EXQUERY,dat1)
#define MC_EXOLOCK(dat1,dat2,dat3)      mkCmd(CMD_EXOLOCK,dat1,dat2,dat3)
#define MC_EXLED(dat1)                  mkCmd(CMD_EXLED,dat1)
#define MC_EXLEDTK(dat1)                mkCmd(CMD_EXLEDTK,dat1)
#define MC_EXLEDSTATE()                 mkCmd(CMD_EXLEDSTATE)
#define MC_WEIGHT(dat1)                 mkCmd(CMD_WEIGHT,dat1)
#define MC_SCALEDATA(dat1,dat2,word)    mkCmd(CMD_SCALEDATA,dat1,dat2,word>>8,word&0xF)
#define MC_SETSCALE(dat1)               mkCmd(CMD_SETSCALE,dat1)

//发送命令
#define SC_Reset(time)                  sendCmd(MC_RESET(),time)
#define SC_Query(time)                  sendCmd(MC_QUERY(),time)
#define SC_OpenLock(d1,d2,time)         sendCmd(MC_OPENLOCK(d1,d2),time)
#define SC_SetOTime(d1,d2,time)         sendCmd(MC_SETOTIME(d1,d2),time)
#define SC_Warning(d1,time)             sendCmd(MC_WARNING(d1),time)
#define SC_EXQuery(d1,time)             sendCmd(MC_EXQUERY(d1),time)
#define SC_EXOLock(d1,d2,d3,time)       sendCmd(MC_EXOLOCK(d1,d2,d3),time)
#define SC_EXLed(d1,time)               sendCmd(MC_EXLED(d1),time)
#define SC_EXLedTK(d1,time)             sendCmd(MC_EXLEDTK(d1),time)
#define SC_EXLedState(time)             sendCmd(MC_EXLEDSTATE(),time)
#define SC_Weight(d1,time)              sendCmd(MC_WEIGHT(d1),time)
#define SC_ScaleData(d1,d2,w,time)      sendCmd(MC_SCALEDATA(d1,d2,w),time)
#define SC_SetScale(d1,time)            sendCmd(MC_SETSCALE(d1),time)



SerialLock* SerialLock::m_instance = NULL;

SerialLock* SerialLock::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new SerialLock(parent);
    return m_instance;
}



SerialLock::SerialLock(QObject *parent) :
    QThread(parent)
{
    m_dAddr = DEFAULT_DA;
    m_sAddr = DEFAULT_SA;

    serial.setPortName("COM3");
    serial.setBaudRate(BAUD9600);

    connect(&sTimer, SIGNAL(timeout()), this, SLOT(readSerial()));
    sTimer.start(50);

    //注册类型
    qRegisterMetaType<SerialLock::DevData>("SerialLock::DevData");

    //绑定任务执行槽
    connect(&taskTimer, SIGNAL(timeout()), this, SLOT(runTask()));
}

SerialLock::~SerialLock()
{

}

QString SerialLock::getErrorString(ErrorCode code)
{
    switch (code) {
    case Success:           return tr("Executed successfully.");//成功
    case OpenSerialFailed:  return tr("Serial port open failed.");//打开串口失败
    case NoResponse:        return tr("Module not responding.");//模块未响应
    case DataError:         return tr("Get data error.");
    case UnkonwnCmd:        return tr("Unknown command.");
    case UserCanceled:      return tr("Task canceled by the user.");
    case TemperExceeded:    return tr("Temperature goes beyond the set valves.");
    case TemperDevErr:      return tr("The temperature sensor circuit may or not installed.");
    case SetOTimeFailed:    return tr("Set open lock time failed.");
    default:    break;
    }
    return tr("Unknown error.");
}

void SerialLock::stop()
{
    waitData.wakeOne(); //激活线程
    errCode = UserCanceled;
    taskQueue.clear();
}

//[0] 串口测试
void SerialLock::testPort(const QString &name, void* parent)
{
    TaskData task;
    task.type = TestPort;
    task.parent = parent;
    task.portName = name;
    addTask(task);
}

//[1] 重置设备
void SerialLock::reset(void* parent)
{
    TaskData data;
    data.type = Reset;
    data.parent = parent;
    addTask(data);
}

//[2] 查询
void SerialLock::query(void* parent)
{
    TaskData data;
    data.type = Query;
    data.parent = parent;
    addTask(data);
}

//[3] 开锁
void SerialLock::openLock(uchar lockNo, uchar time, void* parent)
{
    TaskData data;
    data.type = OpenLock;
    data.parent = parent;
    data.lockNo = lockNo;
    data.time = time;
    addTask(data);
}

//[4] 设置开锁最长时间
void SerialLock::setOpenTime(uchar time, uchar reTime, void* parent)
{
    TaskData data;
    data.type = SetOpenTime;
    data.parent = parent;
    data.time = time;
    data.time = reTime;
    addTask(data);
}

//[5] 报警
void SerialLock::warning(bool enable, void* parent)
{
    TaskData data;
    data.type = Warning;
    data.parent = parent;
    data.warning = enable;
    addTask(data);
}

//[6] 扩展板触点查询
void SerialLock::exQuery(uchar exNo, void* parent)
{
    TaskData data;
    data.type = EXQuery;
    data.parent = parent;
    data.exNo = exNo;
    addTask(data);
}

//[7] 扩展版开锁
void SerialLock::exOpenLock(uchar exNo, uchar lockNo, uchar time, void* parent)
{
    TaskData data;
    data.type = EXOpenLock;
    data.parent = parent;
    data.exNo = exNo;
    data.lockNo = lockNo;
    data.time = time;
    addTask(data);
}

//[8] 扩展板发光管控制
void SerialLock::exLed(QByteArray dat, void* parent)
{
    TaskData data;
    data.type = EXLed;
    data.parent = parent;
    data.dat = dat;
    addTask(data);
}

//[9] 扩展板发光管闪烁控制命令，闪烁周期为1秒
void SerialLock::exLedTwinkle(QByteArray dat, void* parent)
{
    TaskData data;
    data.type = EXLedTwinkle;
    data.parent = parent;
    data.dat = dat;
    addTask(data);
}

//[10] 读扩展板发光管状态
void SerialLock::exLedState(void* parent)
{
    TaskData data;
    data.type = EXLedState;
    data.parent = parent;
    addTask(data);
}

//[11] 读电子秤测到的重量
void SerialLock::readWeight(uchar no, void* parent)
{
    TaskData data;
    data.type = ReadWeight;
    data.parent = parent;
    data.lockNo = no;
    addTask(data);
}

//[12] 设置电子秤
void SerialLock::setScale(QByteArray dat, void *parent)
{
    TaskData data;
    data.type = SetScale;
    data.parent = parent;
    data.dat = dat;
    addTask(data);
}



/**
 ********************************************************************
 * private:
 ********************************************************************
*/

QByteArray SerialLock::mkCmd(uchar cmd, uchar *dat, uchar len)
{
    uchar sum = 0;
    QByteArray cmds;

    cmds.append(m_dAddr);   // 0
    cmds.append(m_sAddr);   // 1
    cmds.append(len + 1);   // 2    DATA_LENGTH
    cmds.append(cmd);       // 3    CMD

    // DATA[0...N]
    for( int i = 0; i < len; i++ ) cmds.append(dat[i]);

    //校验位 异或
    for(int i = 0; i < cmds.length(); i++) sum ^= cmds[i]&0xFF;

    cmds.append(sum);
    return cmds;
}

QByteArray SerialLock::mkCmd(uchar cmd, QByteArray dat)
{
    return mkCmd(cmd, (uchar*)dat.data(), dat.length());
}

QByteArray SerialLock::mkCmd(uchar cmd, uchar dat1)
{
    return mkCmd(cmd, &dat1, 1);
}

QByteArray SerialLock::mkCmd(uchar cmd, uchar dat1, uchar dat2)
{
    uchar dat[2] = { dat1, dat2 };
    return mkCmd(cmd, dat, 2);
}

QByteArray SerialLock::mkCmd(uchar cmd, uchar dat1, uchar dat2, uchar dat3)
{
    uchar dat[3] = { dat1, dat2, dat3 };
    return mkCmd(cmd, dat, 3);
}

//发送命令
SerialLock::ErrorCode SerialLock::sendCmd(QByteArray dat, int timeout)
{
    serial.write(dat);
    if(isDebug) debug16(dat, portName() + " Send data");

    //cCmd = (uchar)dat[3];//第4个为命令字，设备会返回命令字

    if(!waitData.wait(&mutex, timeout)) return NoResponse;
    return errCode;
}

//发送握手命令（目的地址）
SerialLock::ErrorCode SerialLock::handshake()
{
    QByteArray dat;
    dat.append(m_dAddr);

    serial.write(dat);
    if(isDebug) debug16(dat, portName() + " Send handshake");

    if(!waitData.wait(&mutex, cmdWTime)) return NoResponse;
    return errCode;
}

//分析接收到的数据
SerialLock::ErrorCode SerialLock::analyzeCmd(QByteArray &dat)
{
    switch ((uchar)dat[3]) {//第4个为命令字
    case CMD_RESET:
        break;
    case CMD_QUERY:
        m_data.qDat1 = (uchar)dat[4];
        m_data.qEXState = ((dat[5]&0xFF)<<8) | (dat[6]&0xFF);
        m_data.qOpenTime = (uchar)dat[7];
        m_data.qReTime = (uchar)dat[8];
        if((uchar)dat[9] == 0x00) m_data.qTemper = (uchar)dat[10];
        else if((uchar)dat[9] == 0x01) return TemperExceeded;
        else if((uchar)dat[9] == 0x02) return TemperDevErr;
        break;
    case CMD_OPENLOCK:
        break;
    case CMD_SETOTIME:
        if((int)dat[4] == 1) return SetOTimeFailed;//参数无效
        break;
    case CMD_WARNING:
        break;
    case CMD_EXQUERY:
        m_data.exNo = (uchar)dat[4];
        m_data.gunState = (uchar)dat[5];
        m_data.doorState = (uchar)dat[6];
        break;
    case CMD_EXOLOCK:
        m_data.gunState = (uchar)dat[5];
        break;
    case CMD_EXLED:
        break;
    case CMD_EXLEDTK:
        break;
    case CMD_EXLEDSTATE:
        m_data.dat.clear();
        for(int i=0; i < dat[2]-1; i++) m_data.dat.append(dat[i+4]);
        break;
    case CMD_WEIGHT:
        m_data.exNo = (uchar)dat[4];
        m_data.weight = (dat[5]<<24) | (dat[6]<<16) | (dat[7]<<8) | dat[8];
        break;
    default:
        return UnkonwnCmd;//Get data 5: 00 01 01 fc fc
    }
    return Success;
}



/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void SerialLock::readSerial()
{
    static QByteArray datTemp;//串口数据缓存

    if(serial.bytesAvailable() <= 0) return;
    QByteArray dat = serial.readAll();

    //累加前面缓存的数据
    if(!datTemp.isEmpty()){
        dat = datTemp + dat; datTemp.clear();
    }

    if(isDebug) debug16(dat, portName() + " Get data");

    //接收到握手返回命令（源地址）
    if(dat.length() == 1 && (uchar)dat[0] == m_sAddr) errCode = Success;
    else if(dat.length() < 3){ datTemp = dat; return; } //防止 dat[2] 越界
    else if(dat[2]+4 == dat.length()){   //完整的命令串
        uchar sum = 0;
        for(int i = 0; i < dat.length()-1; i++) sum ^= dat[i]&0xFF;//校验
        if(sum == (uchar)dat[dat.length()-1]) errCode = analyzeCmd(dat);//分析命令串
        else { datTemp.clear(); errCode = DataError; }//校验位不相等，数据可能损坏
    }
    else { datTemp.clear(); errCode = DataError; }

    waitData.wakeOne(); //激活线程
}

//[1] 线程函数
void SerialLock::run()
{
    mutex.lock();

    //直接传递的外部数据
    m_data.type = cTask.type;//任务类型
    m_data.parent = cTask.parent;//任务创建者
    m_data.lockNo = cTask.lockNo;//锁编号
    m_data.exNo = cTask.exNo;

    //开始 测试串口任务 之前再设置串口端口
    if(cTask.type == TestPort) setPortName(cTask.portName);

    //串口未打开时，打开串口
    if(!serial.isOpen()){
        if(!openSerial()){ m_data.code = OpenSerialFailed; goto End; }
        //串口打开成功，则发送握手指令
        else if((m_data.code = handshake()) != Success) goto Close;
    }

    //! 根据任务，发送相应指令。
    //! 数据的接收在readSerial()函数中，数据处理在analyzeCmd()函数中
    //! 相应的任务，获得是数据可以参照analyzeCmd()函数
    switch (cTask.type) {
    case TestPort:
    case Reset:         m_data.code = SC_Reset(cmdWTime); break;
    case Query:         m_data.code = SC_Query(cmdWTime); break;
    case OpenLock:      m_data.code = SC_OpenLock(cTask.lockNo, cTask.time, cmdWTime); break;
    case SetOpenTime:   m_data.code = SC_SetOTime(cTask.time, cTask.time, cmdWTime); break;
    case Warning:       m_data.code = SC_Warning((uchar)cTask.warning, cmdWTime); break;
    case EXQuery:       m_data.code = SC_EXQuery(cTask.exNo, cmdWTime); break;
    case EXOpenLock:    m_data.code = SC_EXOLock(cTask.exNo, cTask.lockNo, cTask.time, cmdWTime); break;
    case EXLed:         m_data.code = SC_EXLed(cTask.dat, cmdWTime); break;
    case EXLedTwinkle:  m_data.code = SC_EXLedTK(cTask.dat, cmdWTime); break;
    case EXLedState:    m_data.code = SC_EXLedState(cmdWTime); break;
    case ReadWeight:    m_data.code = SC_Weight(cTask.lockNo, cmdWTime); break;
    case SetScale:      m_data.code = SC_SetScale(cTask.dat, cmdWTime); break;
    default: break;
    }

Close:
    if(taskQueue.isEmpty()) serial.close();//若一组任务完成后，关闭串口

End:
    emit finished(m_data);
    mutex.unlock();
}

//[2] 执行任务
void SerialLock::runTask()
{
    if(this->isRunning()) return;

    if( taskQueue.isEmpty() ){ taskTimer.stop(); return; }
    cTask = taskQueue.dequeue();
    this->start();
}

//[3] 添加任务
void SerialLock::addTask(TaskData data)
{
    taskQueue.enqueue(data);
    if( !taskTimer.isActive() ) taskTimer.start(300);
}
