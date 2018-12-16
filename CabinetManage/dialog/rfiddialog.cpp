#include "rfiddialog.h"
#include "ui_rfiddialog.h"

RfidDialog::RfidDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::RfidDialog)
{
    ui->setupUi(this);

    sRfid = SerialRfid::instance(this);
    connect(sRfid, SIGNAL(finished(SerialRfid::DevData)), this, SLOT(finished(SerialRfid::DevData)));

    sRfid->getCardId(SerialRfid::ReadOnly, this);
    ui->label->setText(tr("Please brush RFID card"));
}

RfidDialog::~RfidDialog()
{
    delete ui;
}

QString RfidDialog::getCardId(QWidget *parent)
{
    RfidDialog dialog(parent);
    dialog.exec();
    return dialog.id;
}

void RfidDialog::finished(const SerialRfid::DevData data)
{
    if( data.parent != this || isHidden() ) return ;

    if(data.code == SerialRfid::UserCanceled) return ;
    if( data.code != SerialRfid::Success ){
        ui->label->setText(sRfid->getErrorString(data.code));
        return ;
    }

    if (data.type == SerialRfid::GetCardId) id = data.cardId;
}

void RfidDialog::on_cancel_clicked()
{
    sRfid->stopTask();
    reject();
}
