#ifndef SETUP_H
#define SETUP_H

#include <QWidget>
#include "config.h"
#include "database.h"
#include "network.h"
#include "dataloop.h"
#include "widget/setuplock.h"
#include "widget/setuprfid.h"
#include "widget/setupserial.h"
#include "widget/setuprs485.h"
#include "widget/setupmusic.h"
#include "arm4418.h"

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT

public:
    explicit Setup(QWidget *parent = 0);
    ~Setup();

    void init();

signals:
    void showMessage(QString text, int ms);//显示提示信息

private:
    Ui::Setup *ui;

    Arm4418 *arm4418;
    Config *config;
    Database *database;
    Network *network;
    DataLoop *dataLoop;

    QWidget *cButton;//保存当前按钮指针，用于改变css

    //界面指针
    QWidget* cWidget;//保存当前显示的界面
    QWidget* home;//通用设置界面
    SetupLock* setupLock;//锁控测试界面
    SetupRfid* setupRfid;//RFID测试界面
    SetupSerial* setupSerial;//串口测试界面
    SetupRs485* setupRs485;//485测试界面
    SetupMusic* setupMusic;//音乐界面

    QString blackWhite(int start, int stop, bool border = false);//返回由黑到白的渐变css
    void setButtonEnabled(bool enable);//设置左边按钮是否禁用

private slots:
    void roomListClicked(int index);//Room list 项目点击槽

    void finished(const Network::DataCore &data);

    void setCurrentWidget(QWidget* widget, bool isNew);//改变当前界面指针
    void setCurrentButton(QWidget* button);//改变当前按钮

    void on_welcome_clicked();

    void on_lockTest_clicked();
    void on_rfidTest_clicked();
    void on_serialTest_clicked();
    void on_rs485Test_clicked();

    void on_updatePolice_clicked();
    void on_initConfig_clicked();
    void on_updateTime_clicked();
    void on_initDatabase_clicked();
    void on_haveUSBFinger_clicked();
    void on_haveRFID_clicked();
    void on_setupMusic_clicked();
    void on_setBackight_valueChanged(int value);
    void on_cBoxLocalVerify_clicked();
    void on_cBoxVManager_clicked();
    void on_saveButton_clicked();
    void on_openDebug_clicked();
    void on_setOpenAllCard_clicked();
};

#endif // SETUP_H
