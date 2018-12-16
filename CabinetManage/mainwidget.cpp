#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QDebug>

#include "dialog/verifydialog.h"
#include "dialog/alcoholdialog.h"
#include "misc/msgwapper.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    config = Config::instance(this); // 实例化配置类
    config->readConfig();

    network = Network::instance(this); // 实例化网络类
    network->setServerIp(config->serverIp);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this); // 数据类

    database = Database::instance(this); // 实例化数据库类
    database->connectDb(QApplication::applicationDirPath() + "/cabManage.db");

    ui->setupUi(this);

    if (config->isArm4418) {
        //实例化ARM4418类
        arm4418 = Arm4418::instance(this);
        arm4418->updatePortName(config->arm4418Port);
        arm4418->setBackight(config->lcdBackight);
    } else {
        //实例化串口锁控类
        lock = SerialLock::instance(this);
        lock->setPortName(config->lockPort);
        connect(lock, SIGNAL(finished(SerialLock::DevData)), this, SLOT(finished(SerialLock::DevData)));
    }

    //设置全屏
    if(config->fullScreen) this->setWindowState(Qt::WindowFullScreen);

    //隐藏鼠标
    if(config->hideMouse) this->setCursor(Qt::BlankCursor);

    //服务器
    if(config->isQServer){
        server = Server::instance(this);
        connect(server, SIGNAL(message(QString,int)), this, SLOT(showMessage(QString,int)));
        server->listen(config->qPost);
    }

    //初始化数据中心
    m_data.cWidget = NULL;

    //设置当前界面为home
    home = new Home(this);
    connect(home, SIGNAL(backWidget()), this, SLOT(on_backButton_clicked()));
    connect(home, SIGNAL(widgetChange(QWidget*,bool)), this, SLOT(setCurrentWidget(QWidget*,bool)));
    connect(home, SIGNAL(showMessage(QString,int)), this, SLOT(showMessage(QString,int)));
    setCurrentWidget(home, true);
    setup = NULL;

    //绑定更新时间槽
    connect(&updateTime, SIGNAL(timeout()), this, SLOT(updateTimeSlot()));
    updateTime.start(1000);

    //绑定更新信息槽
    connect(&updateData, SIGNAL(timeout()), this, SLOT(updateDataSlot()));
    if(config->updateData > 0) updateData.start(config->updateData);

    //更新硬件状态
    connect(&updateTemper, SIGNAL(timeout()), this, SLOT(updateTemperSlot()));
    if(config->updateTemper > 0) updateTemper.start(config->updateTemper);

    //绑定更新提示信息槽
    connect(&updateMessage, SIGNAL(timeout()), this, SLOT(updateMessageSlot()));

    MsgWapper::instance();
    if (!config->isArm4418) ui->alcohol->hide();
}

MainWidget::~MainWidget()
{
    delete ui;
}




/**
 ********************************************************************
 * private:
 ********************************************************************
*/

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/img/background.png"));
    //QWidget::paintEvent();
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

//显示提示信息
void MainWidget::showMessage(QString text, int ms)
{
    ui->messageLabel->setText(text);
    updateMessage.start(ms);
}

//更新时间
void MainWidget::updateTimeSlot()
{
    QDateTime time = QDateTime::currentDateTime();
    ui->timeLabel->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}

//更新信息
void MainWidget::updateDataSlot()
{
    updateData.stop();
    if( config->updateData > 0 ) updateData.start(config->updateData);

    network->getCurrentLead(config->roomId);
    network->getCurrentManage(config->roomId);
}

//更新硬件状态槽
void MainWidget::updateTemperSlot()
{
    updateTemper.stop();
    if( config->updateTemper > 0 ) updateTemper.start(config->updateTemper);

    if( config->isArm4418 ){
        QString temper = QString::number(arm4418->getTemper(), 'f', 0);
        ui->temperature->setText(tr("Temper:") + temper + tr("C"));
    } else {
        lock->query(this);
    }
}

//更新提示信息
void MainWidget::updateMessageSlot()
{
    updateMessage.stop();
    ui->messageLabel->clear();
}

//设置当前界面指针
void MainWidget::setCurrentWidget(QWidget *widget, bool isNew)
{
    //如果显示的不是主页，则显示返回按钮
    if( widget == home || widget == NULL ) {
        ui->backButton->hide();
        config->readConfig(); // 每次显示主界面时重新读取配置文件
    } else ui->backButton->show();

    //添加进布局
    if( isNew ) ui->mainLayout->addWidget(widget);

    //隐藏上一个界面
    if( m_data.cWidget == NULL ) ui->defaultHome->hide();
    else m_data.cWidget->hide();

    m_data.cWidget = widget;//设置新的窗体
    m_data.cWidget->show();
}

//网络信号槽
void MainWidget::finished(const Network::DataCore &data)
{
    switch (data.code) {
    case Network::Success:
        ui->netStatusLabel->setText(tr("Online"));
        ui->neteorkPhoto->setPixmap(QPixmap(":/img/misc/networkState.png"));
        break;
    case Network::NoNetwork:
    case Network::ConnectTimeout:
    case Network::OtherNetError:
        ui->netStatusLabel->setText(tr("NoNetwork"));
        ui->neteorkPhoto->setPixmap(QPixmap(":/img/misc/nonetwork.png"));
        showMessage(network->getErrorString(data.code), 2000);
        break;
    case Network::NoData: break;
    case Network::GetCurrentManage:
        foreach (DataModel::Police police, data.policeList) {
            qDebug() << "Current Manager:" << police.Name;
        }
    default:
        showMessage(network->getErrorString(data.code), 2000);
        break;
    }
}

//串口锁控槽
void MainWidget::finished(const SerialLock::DevData data)
{
    if( data.parent != this ) return ;

    if(data.code == SerialLock::Success){
        if(data.type == SerialLock::Query){
            QString temper = QString::number(data.qTemper);
            ui->temperature->setText(tr("Temper:") + temper + tr("C"));
        }
    } else {
        //showMessage(lock->getErrString(data.code), 2000);
    }
}

void MainWidget::on_backButton_clicked()
{
    setCurrentWidget(home, false);
}

void MainWidget::on_setupButton_clicked()
{
    //if(VerifyDialog::verifyId(this, VerifyDialog::VLeader) != QDialog::Accepted) return ;

    if(setup == NULL){
        setup = new Setup(this);
        connect(setup, SIGNAL(showMessage(QString,int)), this, SLOT(showMessage(QString,int)));
        setCurrentWidget(setup, true);
    } else {
        setCurrentWidget(setup, false);
    }
    setup->init();
}

void MainWidget::on_alcohol_clicked()
{
    AlcoholDialog dialog(this);
    dialog.exec();
}
