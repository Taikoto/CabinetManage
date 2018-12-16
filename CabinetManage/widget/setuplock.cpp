#include "setuplock.h"
#include "ui_setuplock.h"

#include "misc/msgwapper.h"

SetupLock::SetupLock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupLock)
{
    ui->setupUi(this);

    lock = SerialLock::instance();
    connect(lock, SIGNAL(finished(SerialLock::DevData)), this, SLOT(finished(SerialLock::DevData)));

    connect(MsgWapper::instance(), SIGNAL(message(QtMsgType,QString)),
            this, SLOT(oMessage(QtMsgType,QString)));

    ui->textBrowser->document()->setMaximumBlockCount(1000);
}

SetupLock::~SetupLock()
{
    delete ui;
}


void SetupLock::oMessage(QtMsgType /*type*/, const QString &msg)
{
    ui->textBrowser->append(msg);
}

void SetupLock::finished(const SerialLock::DevData data)
{
    if(data.parent != this) return;

    ui->textBrowser->append(lock->getErrorString(data.code));

    if(data.code == SerialLock::Success && data.type == SerialLock::ReadWeight)
        ui->textBrowser->append("Read weight = " + QString::number(data.weight) + "g");

    if(data.type == SerialLock::Query)
        ui->textBrowser->append("dat1 = 0x" + QString::number(data.qDat1, 16) +
                                ", exState = 0x" + QString::number(data.qEXState, 16) +
                                ", Open time = 0x" + QString::number(data.qOpenTime, 16) +
                                ", ReTime = 0x" + QString::number(data.qReTime, 16) +
                                ", Temper = " + QString::number(data.qTemper) + ".");
}

void SetupLock::on_Reset_clicked()
{
    lock->reset(this);
}

void SetupLock::on_TestPort_clicked()
{
    lock->testPort(ui->lineEdit->text(), this);
}

void SetupLock::on_Query_clicked()
{
    lock->query(this);
}

void SetupLock::on_OpenLock_clicked()
{
    lock->openLock(ui->exBox->currentIndex(), ui->timeBox->currentIndex(), this);
}

void SetupLock::on_SetOpenTime_clicked()
{
    lock->setOpenTime(ui->timeBox->currentIndex(), ui->timeBox->currentIndex(), this);
}

void SetupLock::on_Warning_clicked()
{
    lock->warning(ui->warning->isChecked(), this);
}

void SetupLock::on_EXQuery_clicked()
{
    lock->exQuery(ui->exBox->currentIndex(), this);
}

void SetupLock::on_EXOpenLock_clicked()
{
    bool ok;
    uchar no = (uchar)ui->lockBox->currentText().toInt(&ok, 16);
    lock->exOpenLock(ui->exBox->currentIndex(), no, ui->timeBox->currentIndex(), this);
}

void SetupLock::on_EXLed_clicked()
{
    //lock->exLed();
}

void SetupLock::on_EXLedTwinkle_clicked()
{

}

void SetupLock::on_EXLedState_clicked()
{

}

void SetupLock::on_ReadWeight_clicked()
{
    lock->readWeight(ui->weightBox->currentIndex(), this);
}

void SetupLock::on_allLock_clicked()
{
    for(uchar i=0; i < 16; i++)
    {
        lock->exOpenLock(i, 0xFF, ui->timeBox->currentIndex(), this);
    }
}

void SetupLock::on_setPort_clicked()
{
    lock->setPortName(ui->lineEdit->text());
}

void SetupLock::on_setScale_clicked()
{
    uchar code[] = { 0x01, 0x00, 0x00, 0x18, 0xFF, 0xFF, 0xFF, 0xFF };
    code[0] = ui->weightBox->currentIndex();
    QByteArray dat;
    for(int i=0; i < 8; i++) dat.append(code[i]);
    lock->setScale(dat, this);
}
