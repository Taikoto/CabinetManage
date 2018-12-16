#include "setupserial.h"
#include "ui_setupserial.h"

#include <QTime>
#include <QMessageBox>
#include <QTextStream>

SetupSerial::SetupSerial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupSerial)
{
    ui->setupUi(this);

    ReceiveCount=0;
    SendCount=0;
    IsAutoClear=false;
    IsHexSend=true;
    IsHexReceive=true;
    IsDebug=false;

    QStringList comList;//串口号
    QStringList baudList;//波特率

#ifdef Q_OS_WIN//如果是windows系统
    comList<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"
          <<"COM7"<<"COM8"<<"COM9"<<"COM10"<<"COM11"<<"COM12"
         <<"COM13"<<"COM14"<<"COM15";
#else//如果是unix或者其他系统
    comList<<"ttyAMA1"<<"ttyAMA2"<<"ttyAMA3"<<"ttyGS0"<<"ttyGS1"<<"ttyGS2"
          <<"ttyGS3"<<"ttyXRUSB0"<<"ttyXRUSB1"<<"ttyXRUSB2"<<"ttyXRUSB3";
#endif

    ui->cboxPortName->addItems(comList);
    ui->cboxPortName->setCurrentIndex(2);

    baudList<<"50"<<"75"<<"110"<<"134"<<"150"<<"200"<<"300"
           <<"600"<<"1200"<<"1800"<<"2400"<<"4800"<<"9600"
          <<"14400"<<"19200"<<"38400"<<"56000"<<"57600"
         <<"76800"<<"115200"<<"128000"<<"256000";

    ui->cboxBaudRate->addItems(baudList);
    ui->cboxBaudRate->setCurrentIndex(12);

    //读取数据(采用定时器读取数据，不采用事件，方便移植到linux)
    myReadTimer=new QTimer(this);
    myReadTimer->setInterval(100);
    connect(myReadTimer,SIGNAL(timeout()),this,SLOT(ReadMyCom()));

    //发送数据
    mySendTimer=new QTimer(this);
    mySendTimer->setInterval(5000);
    connect(mySendTimer,SIGNAL(timeout()),this,SLOT(WriteMyCom()));

    //显示日期时间
    myTimer=new QTimer(this);
    myTimer->start(1000);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(SetTime()));

    QDate dateNow= QDate::currentDate();
    ui->labDate->setText(tr("Date:") + dateNow.toString("yyyy-MM-dd dddd"));

    for ( int i = 1; i <= 30; i++ )
        ui->cboxSend->addItem(QString::number(i) + tr("sec"));
    ui->cboxSend->setCurrentIndex(2);

    this->ChangeEnable(false);

    ui->txtSend->installEventFilter(this);//安装监听器监听发送数据框回车响应
}

SetupSerial::~SetupSerial()
{
    delete ui;
}

//监听回车键
bool SetupSerial::eventFilter(QObject *obj, QEvent *event)
{
    if (obj==ui->txtSend)
    {
        if (event->type()==QEvent::KeyPress)
        {
            QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
            if (keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
            {
                WriteMyCom();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj,event);
}

void SetupSerial::ChangeEnable(bool b)
{
    ui->cboxBaudRate->setEnabled(!b);
    ui->cboxPortName->setEnabled(!b);
    ui->btnOpen->setEnabled(true);

    ui->btnSend->setEnabled(b);
}

//字节数组转16进制字符串
QString SetupSerial::byteArrayToHexStr(QByteArray data)
{
    QString temp="";
    QString hex=data.toHex();
    for (int i=0;i<hex.length();i=i+2)
    {
        temp+=hex.mid(i,2)+" ";
    }
    return temp.trimmed().toUpper();
}

//16进制字符串转字节数组
QByteArray SetupSerial::hexStrToByteArray(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertHexChar(hstr);
        lowhexdata = convertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

char SetupSerial::convertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void SetupSerial::SetTime()
{
    QTime timeNow = QTime::currentTime();
    ui->labTime->setText(tr("time:") + timeNow.toString());
}

void SetupSerial::ReadMyCom()
{
    //这个判断尤为重要,否则的话直接延时再接收数据,空闲时和会出现高内存占用
    if ( serial.bytesAvailable() <= 0 ) return ;

    QByteArray buffer= serial.readAll();

    if (IsHexReceive)
    {
        QString tempDataHex = byteArrayToHexStr(buffer);
        ui->txtDataHex->append(tr("Received:") + tempDataHex + " " + tr("time:")
                               + QTime::currentTime().toString("HH:mm:ss"));

        if (IsDebug)//2013-8-6增加接收数据后转发数据，模拟设备
        {
            foreach(QString tempData,SendDataList)
            {
                QStringList temp=tempData.split(';');
                if (tempDataHex==temp[0])
                {
                    //这里没有跳出循环，有可能一条数据会对应多条数据需要转发
                    serial.write(hexStrToByteArray(temp[1]));
                }
            }
        }
    }
    else
    {
        ui->txtDataHex->append(tr("Received:") + buffer + " " + tr("time:")
                               + QTime::currentTime().toString("HH:mm:ss"));

        if (IsDebug)//2013-8-6增加接收数据后转发数据，模拟设备
        {
            foreach(QString tempData,SendDataList)
            {
                QStringList temp=tempData.split(';');
                if (buffer == temp[0])
                {
                    //这里没有跳出循环，有可能一条数据会对应多条数据需要转发
                    serial.write(temp[1].toLatin1());
                }
            }
        }
    }

    ReceiveCount=ReceiveCount+buffer.size();
    ui->labReceive->setText(tr("Received:") + QString::number(ReceiveCount) + tr("Byte"));
}

void SetupSerial::WriteMyCom()
{
    QString str=ui->txtSend->currentText();
    if (str==""){ui->txtSend->setFocus();return;}//发送数据为空
    if (!serial.isOpen()) { return; }//串口没有打开

    QByteArray outData=str.toLatin1();
    int size=outData.size();

    if (IsHexSend)//转化为16进制发送
    {
        outData = hexStrToByteArray(str);
        size=outData.size();
        serial.write(outData);
    }
    else
    {
        size=outData.size();
        serial.write(outData);
    }

    ui->txtDataHex->append(tr("Send:") + str + " " + tr("time:")
                           + QTime::currentTime().toString("HH:mm:ss"));

    SendCount=SendCount+size;
    ui->labSend->setText(tr("Send:") + QString::number(SendCount) + tr("Byte"));

    if (IsAutoClear)
    {
        ui->txtSend->setCurrentIndex(-1);
        ui->txtSend->setFocus();
    }
}

void SetupSerial::on_btnOpen_clicked()
{
    if (!serial.isOpen())
    {
        QString portName=ui->cboxPortName->currentText();

#ifdef Q_OS_WIN//如果是windows系统
        serial.setPortName(portName);
#else
        serial.setPortName("/dev/" + portName);
#endif
        serial.setBaudRate((BaudRateType)ui->cboxBaudRate->currentIndex());

        if (openSerial()){
            this->ChangeEnable(true);
            ui->btnOpen->setText(tr("Close"));//关闭串口
            ui->labIsOpen->setText(tr("State: Open"));//串口状态：打开
            this->myReadTimer->start();
        }
    } else {
        serial.close();
        this->ChangeEnable(false);
        ui->btnOpen->setText(tr("Open"));//打开串口
        ui->labIsOpen->setText(tr("State: Close"));//串口状态：关闭
        this->myReadTimer->stop();

        //这样的话保证每次关闭串口后,自动发送和自动保存定时器不会空转
        ui->ckIsAutoSend->setChecked(false);
    }
}

void SetupSerial::on_ckHexSend_stateChanged(int arg1)
{
    IsHexSend = (bool)arg1;
}

void SetupSerial::on_ckHexReceive_stateChanged(int arg1)
{
    IsHexReceive = (bool)arg1;
}

void SetupSerial::on_ckIsAutoSend_stateChanged(int arg1)
{
    bool IsAutoSend = (bool)arg1;
    if (IsAutoSend) mySendTimer->start();
    else mySendTimer->stop();
}

void SetupSerial::on_ckIsAutoClear_stateChanged(int arg1)
{
    IsAutoClear = (bool)arg1;
}

void SetupSerial::on_ckIsDebug_stateChanged(int arg1)
{
    IsDebug = (bool)arg1;
}

void SetupSerial::on_btnClearSend_clicked()
{
    SendCount=0;
    ui->labSend->setText(tr("Send:") + QString::number(0) + tr("Byte"));
}

void SetupSerial::on_btnClearReceive_clicked()
{
    ReceiveCount=0;
    ui->labReceive->setText(tr("Received:") + QString::number(0) + tr("Byte"));
}

void SetupSerial::on_cboxSend_currentIndexChanged(int index)
{
    mySendTimer->setInterval((index+1)*1000);
}

void SetupSerial::on_btnClearAll_clicked()
{
    ui->txtDataHex->clear();
}

void SetupSerial::on_btnSend_clicked()
{
    this->WriteMyCom();
}
