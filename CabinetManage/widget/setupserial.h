#ifndef SETUPSERIAL_H
#define SETUPSERIAL_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <Device>

namespace Ui {
class SetupSerial;
}

class SetupSerial : public QWidget, public SerialBase
{
    Q_OBJECT

public:
    explicit SetupSerial(QWidget *parent = 0);
    ~SetupSerial();

protected:
    bool eventFilter(QObject *obj, QEvent *event);//监听事件

private:
    Ui::SetupSerial *ui;

    QTimer *myTimer;

    QTimer *myReadTimer;//定时读取串口数据
    QTimer *mySendTimer;//定时发送串口数据

    int SendCount;//发送数据计数
    int ReceiveCount;//接收数据计数
    bool IsDebug;//是否启用调试,接收到数据后模拟发送数据
    bool IsAutoClear;//是否自动清空
    bool IsHexSend;//是否16进制数据发送
    bool IsHexReceive;//是否16进制数据接收

    QStringList SendDataList;//转发数据链表

    void ChangeEnable(bool b);//改变状态

    QString byteArrayToHexStr(QByteArray data);//字节数组转16进制字符串
    QByteArray hexStrToByteArray(QString str);//16进制字符串转字节数组
    char convertHexChar(char ch);

private slots:
    void on_cboxSend_currentIndexChanged(int index);
    void on_btnSend_clicked();
    void on_btnClearAll_clicked();
    void on_btnClearReceive_clicked();
    void on_btnClearSend_clicked();
    void on_ckHexSend_stateChanged(int arg1);
    void on_ckHexReceive_stateChanged(int arg1);
    void on_ckIsAutoSend_stateChanged(int arg1);
    void on_ckIsAutoClear_stateChanged(int arg1);
    void on_ckIsDebug_stateChanged(int arg1);
    void on_btnOpen_clicked();

    void SetTime();//动态显示时间
    void ReadMyCom();//读取串口数据
    void WriteMyCom();//写串口数据
};

#endif // SETUPSERIAL_H
