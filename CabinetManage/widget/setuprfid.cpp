#include "setuprfid.h"
#include "ui_setuprfid.h"

#include "misc/msgwapper.h"

SetupRfid::SetupRfid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupRfid)
{
    ui->setupUi(this);

    rfid = SerialRfid::instance();
    connect(rfid, SIGNAL(finished(SerialRfid::DevData)), this, SLOT(finished(SerialRfid::DevData)));

    connect(MsgWapper::instance(), SIGNAL(message(QtMsgType,QString)), this, SLOT(oMessage(QtMsgType,QString)));

    ui->textBrowser->document()->setMaximumBlockCount(1000);
}

SetupRfid::~SetupRfid()
{
    delete ui;
}


void SetupRfid::oMessage(QtMsgType /*type*/, const QString &msg)
{
    ui->textBrowser->append(msg);
}

void SetupRfid::finished(const SerialRfid::DevData data)
{
    if(data.parent != this) return;

    ui->textBrowser->append(rfid->getErrorString(data.code));

    if(data.code == SerialRfid::Success && data.type == SerialRfid::GetCardId)
        ui->textBrowser->append("Card ID:" + data.cardId);
}

void SetupRfid::on_setPort_clicked()
{
    rfid->setPortName(ui->lineEdit->text());

    ui->textBrowser->append("Set port : " + rfid->portName());
}

void SetupRfid::on_getCardId_clicked()
{
    rfid->getCardId(SerialRfid::ReadWrite, this);
}

void SetupRfid::on_TestPort_clicked()
{
    rfid->testPort(ui->lineEdit->text(), this);
}

void SetupRfid::on_setBaud_clicked()
{
    switch (ui->baudrate->text().toInt()) {
    case 9600:  rfid->setBaudrate(BAUD9600); break;
    case 19200: rfid->setBaudrate(BAUD19200); break;
    case 38400: rfid->setBaudrate(BAUD38400); break;
    case 57600: rfid->setBaudrate(BAUD57600); break;
    case 115200:rfid->setBaudrate(BAUD115200); break;
    default:    rfid->setBaudrate(BAUD9600); break;
    }

    ui->textBrowser->append("Set baudrate : " + QString::number(rfid->baudrate()));
}

void SetupRfid::on_setDevBaud_clicked()
{
    rfid->setDevBaudrate(ui->comboBox->currentIndex(), this);
}

void SetupRfid::on_getCardIdOnly_clicked()
{
    rfid->getCardId(SerialRfid::ReadOnly, this);
}
