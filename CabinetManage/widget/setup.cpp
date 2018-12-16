#include "setup.h"
#include "ui_setup.h"

#include <QDateTime>
#include <QProcess>
#include "dialog/downdialog.h"
#include "dialog/infodialog.h"
#include "dialog/rfiddialog.h"
#include "common.h"

using namespace DataModel;

Setup::Setup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);

    ui->about->setText(ui->about->text() + " (" + BUILD_TIME_STR + ")");

    //0, 1000为输入的数字值范围
    ui->updateData->setValidator(new QIntValidator(0, 1000, this));
    ui->updateTemper->setValidator(new QIntValidator(0, 1000, this));
    ui->closeLcd->setValidator(new QIntValidator(0, 1000, this));

    ui->setBackight->setMinimum(0);
    ui->setBackight->setMaximum(252);

    config = Config::instance(this);

    if(config->isArm4418) arm4418 = Arm4418::instance(this);

    database = Database::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this);

    //初始化界面指针
    cWidget = ui->setupWidget;
    home = ui->setupWidget;
    setupLock = NULL;
    setupRfid = NULL;
    setupSerial = NULL;
    setupRs485 = NULL;
    setupMusic = NULL;

    //初始化按钮指针
    cButton = ui->welcome;
    setCurrentButton(ui->welcome);

    //初始化枪柜列表
    ui->roomList->setTitle(tr("Cabinet list"));
    ui->roomList->addColumn(ZDiy::Label, tr("No"));
    ui->roomList->addColumn(ZDiy::Label, tr("Type"));
    ui->roomList->addColumn(ZDiy::Label, "", 40);
    ui->roomList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"));
    ui->roomList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"));
    ui->roomList->setPageItemCount(8);
    connect(ui->roomList, SIGNAL(itemClicked(int)), this, SLOT(roomListClicked(int)));
}

Setup::~Setup()
{
    delete ui;
}

void Setup::init()
{
    //int index = ui->roomList->currentIndex();
    ui->roomList->clear();
    for( int i = 0; i < dataLoop->cabinets.length(); i++ )
    {
        ui->roomList->addItem();
        ui->roomList->setItemText(i, 0, dataLoop->cabinets[i].No);
        ui->roomList->setItemText(i, 1, cabinetType(dataLoop->cabinets[i].CabType));
        if( dataLoop->cabinets[i].Id == config->cabId ) roomListClicked(i);
    }
    //ui->roomList->sendClicked(index);

    ui->cBoxLocalVerify->setChecked(config->localVerify);
    ui->cBoxVManager->setChecked((bool)(config->vManagerNum-1));//双人验证
    ui->haveUSBFinger->setChecked(config->haveUSBFinger);
    ui->haveRFID->setChecked(config->haveRFID);
    ui->serverIp->setText(config->serverIp);
    ui->roomId->setText(config->roomId);
    ui->updateData->setText(QString::number(config->updateData/1000));
    ui->updateTemper->setText(QString::number(config->updateTemper/1000));
    ui->closeLcd->setText(QString::number(config->closeLcd/1000));
    ui->setBackight->setValue(252 - config->lcdBackight);

    on_updateTime_clicked();
}





/**
 ********************************************************************
 * private:
 ********************************************************************
*/

//获取渐变css代码
QString Setup::blackWhite(int start, int stop, bool border)
{
    QString code;
    code = "background-color: qlineargradient(" \
           "spread:pad, x1:0, y1:0, x2:1, y2:0, ";
    code+= QString("stop:0 rgba(%1, %1, %1, 255), ").arg(start);
    code+= QString("stop:1 rgba(%1, %1, %1, 255));").arg(stop);
    if(border) code+= "border-top: 1px solid; border-bottom: 1px solid;";
    return code;
}

//设置左边按钮是否禁用
void Setup::setButtonEnabled(bool enable)
{
    ui->initDatabase->setEnabled(enable);
    ui->setOpenAllCard->setEnabled(enable);

    ui->setupMusic->setEnabled(enable);
    ui->lockTest->setEnabled(enable);
    ui->rfidTest->setEnabled(enable);
    ui->serialTest->setEnabled(enable);
    ui->rs485Test->setEnabled(enable);
}





/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void Setup::roomListClicked(int index)
{
    if( index < 0 || index >= ui->roomList->length() ) return ;

    for( int i = 0; i < dataLoop->cabinets.length(); i++ )
    {
        if( i == index ){
            ui->roomList->setItemIcon(i, 2, QIcon(":/img/misc/pitchno.png"));
            config->setCabId(dataLoop->cabinets[i].Id);
        } else
            ui->roomList->setItemIcon(i, 2, QIcon());
    }
}

void Setup::finished(const Network::DataCore &data)
{
    if(this->isHidden()) return;

    if(data.code == Network::Success){
        //设置时间
        if(data.type == Network::GetTime){
            QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(data.time.Body);
            QString temp = dateTime.toString("yyyy-MM-dd hh:mm:ss");
            QProcess::execute("date -s \""+temp+"\"");
            QProcess::execute("hwclock -w");
            QProcess::execute("sync");
            emit showMessage(tr("Synchronization time success."), 2000);
        }
    }
}

void Setup::setCurrentWidget(QWidget *widget, bool isNew)
{
    //添加进布局
    if( isNew ) ui->mainLayout->addWidget(widget);

    cWidget->hide();//隐藏上一个界面
    cWidget = widget;//设置新的窗体
    cWidget->show();
}

void Setup::setCurrentButton(QWidget *button)
{
    if(button == NULL) return ;

    cButton->setEnabled(true);
    cButton->setStyleSheet(blackWhite(50, 100));
    cButton = button;
    cButton->setEnabled(false);
    cButton->setStyleSheet(blackWhite(170, 230, true));
}

void Setup::on_welcome_clicked()
{
    setCurrentButton(ui->welcome);
    setCurrentWidget(home, false);
}

void Setup::on_lockTest_clicked()
{
    setCurrentButton(ui->lockTest);

    if(setupLock == NULL){
        setupLock = new SetupLock(this);
        setCurrentWidget(setupLock, true);
    } else {
        setCurrentWidget(setupLock, false);
    }
}

void Setup::on_rfidTest_clicked()
{
    setCurrentButton(ui->rfidTest);

    if(setupRfid == NULL){
        setupRfid = new SetupRfid(this);
        setCurrentWidget(setupRfid, true);
    } else {
        setCurrentWidget(setupRfid, false);
    }
}

void Setup::on_serialTest_clicked()
{
    setCurrentButton(ui->serialTest);

    if(setupSerial == NULL){
        setupSerial = new SetupSerial(this);
        setCurrentWidget(setupSerial, true);
    } else {
        setCurrentWidget(setupSerial, false);
    }
}

void Setup::on_rs485Test_clicked()
{
    setCurrentButton(ui->rs485Test);

    if(setupRs485 == NULL){
        setupRs485 = new SetupRs485(this);
        setCurrentWidget(setupRs485, true);
    } else {
        setCurrentWidget(setupRs485, false);
    }
}

void Setup::on_updatePolice_clicked()
{
    DownDialog dialog(this);
    dialog.start(tr("Download"));
}

void Setup::on_initConfig_clicked()
{
    config->createFile();
    InfoDialog::information(this, tr("Initialization configuration complete."));
}

void Setup::on_updateTime_clicked()
{
    network->getTime();
    emit showMessage(tr("Start getting server time..."), 2000);
}

void Setup::on_initDatabase_clicked()
{
    if(database->createAllTable())
        InfoDialog::information(this, tr("Initialization database success!"));
    else
        InfoDialog::information(this, tr("Initialization database failed!"));
}

void Setup::on_haveUSBFinger_clicked()
{
    config->setHaveUSBFinger(ui->haveUSBFinger->isChecked());
}

void Setup::on_haveRFID_clicked()
{
    config->setHaveRFID(ui->haveRFID->isChecked());
}

void Setup::on_setupMusic_clicked()
{
    setCurrentButton(ui->setupMusic);

    if(setupMusic == NULL){
        setupMusic = new SetupMusic(this);
        setCurrentWidget(setupMusic, true);
        setupMusic->init();
    } else {
        setCurrentWidget(setupMusic, false);
    }
}

void Setup::on_setBackight_valueChanged(int value)
{
    if(value == config->lcdBackight) return ;
    if(!config->isArm4418) return ;

    value = 252 - value;
    arm4418->setBackight(value);
    config->setLcdBackight(value);
}

void Setup::on_cBoxLocalVerify_clicked()
{
    config->setLocalVerify(ui->cBoxLocalVerify->isChecked());
}

void Setup::on_cBoxVManager_clicked()
{
    config->setVManagerNum((int)(ui->cBoxVManager->isChecked())+1);
}

void Setup::on_saveButton_clicked()
{
    config->setServerIp(ui->serverIp->text());
    config->setRoomId(ui->roomId->text());
    config->setUpdateData(ui->updateData->text().toInt());
    config->setUpdateTemper(ui->updateTemper->text().toInt());
    config->setCloseLcd(ui->closeLcd->text().toInt());
    InfoDialog::information(this, tr("Save success!"));
}

void Setup::on_openDebug_clicked()
{
    if( ui->setupMusic->isEnabled() ){
        ui->openDebug->setText(tr("Open Debug"));
        setButtonEnabled(false);
    } else {
        if( ui->password->text() != "2073" ) return ;
        ui->openDebug->setText(tr("Close Debug"));
        setButtonEnabled(true);
    }
}

void Setup::on_setOpenAllCard_clicked()
{
    QString cardId = RfidDialog::getCardId(this);
    if (cardId.isEmpty()) return ;
    config->setOpenAllCard(cardId);
}
