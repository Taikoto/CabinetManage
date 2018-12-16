#include "setuprs485.h"
#include "ui_setuprs485.h"

#include <QFile>
#include <QString>
#include <QTextStream>

#include "misc/msgwapper.h"

SetupRs485::SetupRs485(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupRs485)
{
    ui->setupUi(this);

    ui->sAddress->setValidator(new QIntValidator(0, 255, this));
    ui->dAddress->setValidator(new QIntValidator(0, 255, this));
    ui->address->setValidator(new QIntValidator(0, 255, this));

    config = Config::instance(this);
    config->readConfig();

    arm4418 = Arm4418::instance(this);
    arm4418->setPortName(config->arm4418Port);
    connect(arm4418, SIGNAL(finished(Arm4418::DataCore)), this, SLOT(finished(Arm4418::DataCore)));

    QStringList baudList;//波特率
    baudList<<"50"<<"75"<<"110"<<"134"<<"150"<<"200"<<"300"
           <<"600"<<"1200"<<"1800"<<"2400"<<"4800"<<"9600"
          <<"14400"<<"19200"<<"38400"<<"56000"<<"57600"
         <<"76800"<<"115200"<<"128000"<<"256000";

    ui->cboxBaudRate->addItems(baudList);
    ui->cboxBaudRate->setCurrentIndex(12);

    connect(&timer, SIGNAL(timeout()), this, SLOT(loopSend()));

    //绑定重定向QDebug()输出
    connect(MsgWapper::instance(), SIGNAL(message(QtMsgType,QString)),
            this, SLOT(oMessage(QtMsgType,QString)));

    ui->loopList->setTitle(tr("Loop list"));
    ui->loopList->addColumn(ZDiy::Label, tr("Lock address"));
    ui->loopList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"), 32, 32);
    ui->loopList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"), 32, 32);
    ui->loopList->setPageItemCount(5);
}

SetupRs485::~SetupRs485()
{
    delete ui;
}

void SetupRs485::oMessage(QtMsgType /*type*/, const QString &msg)
{
    ui->textEdit->append(msg);
}

void SetupRs485::finished(Arm4418::DataCore data)
{
    if( data.parent != this ) return ;

    if( data.code == Arm4418::Success ){
        QString text;
        QTextStream stream(&text);
        switch (data.type) {
        case Arm4418::SetOpenLockTime:
            stream << "OpenLockTime: " << data.openLockTime << "\n";
            break;
        case Arm4418::SetDefWeight:
            stream << "DefWeight: " << data.defWeight << "\n";
            break;
        case Arm4418::SetWratio:
            stream << "Wratio: " << data.wratio << "\n";
            break;
        case Arm4418::ReadWeight:
            stream << "Weight: " << data.weight << "\n";
            break;
        case Arm4418::ReadNumber:
            stream << "Number: " << data.number << "\n";
            break;
        default:
            break;
        }
        ui->textEdit->append(text);
    } else {
        ui->textEdit->append(arm4418->getErrorString(data.code));
    }
}


void SetupRs485::loopSend()
{
    uchar dAddr;
    for( int i = 0; i < ui->loopList->length(); i++ )
    {
        dAddr = ui->loopList->itemText(i, 0).toInt(0, 16);
        arm4418->openLock(this, dAddr);
    }
}

void SetupRs485::on_state_clicked()
{
    arm4418->readState(this, arm4418->dAddress());
}

void SetupRs485::on_openLock_clicked()
{
    arm4418->openLock(this, arm4418->dAddress());
}

void SetupRs485::on_changeAddr_clicked()
{
    uchar addr = (uchar)ui->address->text().toInt(0, 16);
    arm4418->setAddress(this, arm4418->dAddress(), addr);
}

void SetupRs485::on_send458_clicked()
{
    arm4418->setSendState();
}

void SetupRs485::on_received_clicked()
{
    arm4418->setReceiveState();
}

void SetupRs485::on_state485_clicked()
{
    ui->textEdit->setText(arm4418->rs485State());
}

void SetupRs485::on_loopSend_clicked()
{
    if( timer.isActive() )
        timer.stop();
    else
        timer.start(ui->msec->text().toInt());
}

void SetupRs485::on_addLoop_clicked()
{
    ui->loopList->addItem();
    ui->loopList->setItemText(ui->loopList->length() - 1, 0, ui->dAddress->text());
}

void SetupRs485::on_delLoop_clicked()
{
    ui->loopList->delItem(ui->loopList->currentIndex());
}

void SetupRs485::on_cboxBaudRate_currentIndexChanged(int index)
{
    if( index > 0 ){
        arm4418->setBaudrate((BaudRateType)index);
        arm4418->updatePortName(arm4418->portName());
    }
}

void SetupRs485::on_sAddress_textEdited(const QString &arg1)
{
    arm4418->setSAddress(arg1.toInt(0, 16));
}

void SetupRs485::on_dAddress_textEdited(const QString &arg1)
{
    arm4418->setDAddress(arg1.toInt(0, 16));
}

void SetupRs485::on_openLcdButton_clicked()
{
    arm4418->openLed(this, arm4418->dAddress());
}

void SetupRs485::on_closeLcdButton_clicked()
{
    arm4418->closeLed(this, arm4418->dAddress());
}

void SetupRs485::on_setOLTButton_clicked()
{
    arm4418->setOpenLockTime(this, arm4418->dAddress(), ui->msec->text().toInt());
}

void SetupRs485::on_setDWButton_clicked()
{
    arm4418->setDefWeight(this, arm4418->dAddress());
}

void SetupRs485::on_setWWButton_clicked()
{
    arm4418->setWratio(this, arm4418->dAddress(), ui->wratioEdit->text().toFloat());
}

void SetupRs485::on_setUnitWButton_clicked()
{
    arm4418->setUnitWeight(this, arm4418->dAddress());
}

void SetupRs485::on_readWeight_clicked()
{
    arm4418->readWeight(this, arm4418->dAddress());
}

void SetupRs485::on_readNumber_clicked()
{
    arm4418->readNumber(this, arm4418->dAddress());
}

void SetupRs485::on_allOpen_clicked()
{
    for( int i = 0; i < 255; i++ )
    {
        arm4418->openLock(this, i);
    }
}
