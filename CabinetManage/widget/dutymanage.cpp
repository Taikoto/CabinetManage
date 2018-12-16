#include "dutymanage.h"
#include "ui_dutymanage.h"

#include <QDebug>
#include "dialog/setdutydialog.h"
#include "dialog/verifydialog.h"
#include "dialog/infodialog.h"

using namespace DataModel;

DutyManage::DutyManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DutyManage)
{
    ui->setupUi(this);

    ui->dutyManagerList->setTitle(tr("Current duty manager list"));
    ui->dutyManagerList->addColumn(ZDiy::Label, tr("Name"));
    ui->dutyManagerList->addColumn(ZDiy::Label, tr("No"));
    ui->dutyManagerList->addColumn(ZDiy::Label, tr("Phone"));
    ui->dutyManagerList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"), 24, 24);
    ui->dutyManagerList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"), 24, 24);
    ui->dutyManagerList->setPageItemCount(3);

    ui->policeInfo->setTitle(tr("Current duty leader"));

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(init()));

    config = Config::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this);
}

DutyManage::~DutyManage()
{
    delete ui;
}

void DutyManage::init()
{
    ui->dutyManagerList->clear();
    network->getCurrentManage(config->roomId);
    network->getCurrentLead(config->roomId);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void DutyManage::finished(const Network::DataCore &data)
{
    if( isHidden() ) return ;

    if( data.code == Network::Success ){
        switch ( data.type ) {
        case Network::GetCurrentManage:
            ui->dutyManagerList->clear();
            for( int i = 0; i < data.policeList.length(); i++ )
            {
                ui->dutyManagerList->addItem();
                ui->dutyManagerList->setItemText(i, 0, data.policeList[i].Name);
                ui->dutyManagerList->setItemText(i, 1, data.policeList[i].No);
                ui->dutyManagerList->setItemText(i, 2, data.policeList[i].Phone);
            }
            updateTimer.stop();
            break;
        case Network::GetCurrentLead:
            ui->policeInfo->setData(data.police);
            updateTimer.stop();
            break;
        default:
            break;
        }
    } else {

    }
}

void DutyManage::on_setDutyLeader_clicked()
{
    SetDutyDialog dialog(this);
    dialog.exec();

    updateTimer.start(1000);
}

void DutyManage::on_hands_clicked()
{
    Police oldPolice = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( oldPolice.Id.isEmpty() ) return ;

    //InfoDialog::information(this, tr("当前值班管理员验证通过，请接班管理员验证身份。"));

    Police newPolice = VerifyDialog::getPoliceData(this, VerifyDialog::VManager);
    if( newPolice.Id.isEmpty() ) return ;

    DataModel::SetCurrent model;
    model.RoomId = config->roomId;
    model.CurrentId = oldPolice.Id;
    model.NewId = newPolice.Id;
    network->postRoomManage(model);
    qDebug() << "[值班交接] RoomId: " << model.RoomId << ", "
             << "Current id: " << model.CurrentId << ", "
             << "Nen id: " << model.NewId;

    updateTimer.start(1000);
}

void DutyManage::on_onDuty_clicked()
{
    if( ui->dutyManagerList->length() >= 3 ){
        InfoDialog::information(this, tr("Duty gun manager is full!"));
        return ;
    }

    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VManager);
    if( police.Id.isEmpty() ) return ;

    DataModel::SetCurrent model;
    model.RoomId = config->roomId;
    model.NewId = police.Id;
    network->postRoomManage(model);

    updateTimer.start(1000);
}

void DutyManage::on_offDuty_clicked()
{
    if( ui->dutyManagerList->length() <= 1 ){
        InfoDialog::information(this, tr("Must have gun manager on duty!"));
        return ;
    }

    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( police.Id.isEmpty() ) return ;

    DataModel::SetCurrent model;
    model.RoomId = config->roomId;
    model.CurrentId = police.Id;
    network->postRoomManage(model);

    updateTimer.start(1000);
}
