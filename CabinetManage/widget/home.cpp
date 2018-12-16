#include "home.h"
#include "ui_home.h"

#include <QDateTime>
#include "dialog/verifydialog.h"

using namespace DataModel;

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    fingerManage = NULL;
    cabinet = NULL;
    dutyManage = NULL;
    applyManage = NULL;
    query = NULL;

    config = Config::instance();

    network = Network::instance();
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    connect(&timer, SIGNAL(timeout()), this, SLOT(onCheckCurrentTask()));
    timer.start(5000);
}

Home::~Home()
{
    delete ui;
}

void Home::initTaskCabInfo()
{
    if(cabinet == NULL){
        cabinet = new TaskCabInfo(this);
        connect(cabinet, SIGNAL(backWidget()), this, SIGNAL(backWidget()));
        connect(cabinet, SIGNAL(showMessage(QString,int)), this, SIGNAL(showMessage(QString,int)));
        emit widgetChange(cabinet, true);
    } else {
        emit widgetChange(cabinet, false);
    }
}

void Home::initTaskCabInfo(const Police &police, const TaskType &type)
{
    initTaskCabInfo();
    cabinet->init(police, type);
}



void Home::finished(const Network::DataCore &data)
{
    if (isHidden() || data.code != Network::Success) return ;

    if (data.type == Network::GetRoomCTaskItem) {
        initTaskCabInfo();
        foreach (CurrentTaskItem item, data.cTaskItems) {
            if (!item.TaskId.isEmpty()) {
                if(VerifyDialog::verifyId(this, VerifyDialog::VDutyManager) != QDialog::Accepted) return ;
                cabinet->init(item.TaskId);
                break;
            }
        }
    }
}

void Home::onCheckCurrentTask()
{
    if (isHidden()) return ;
    network->getRoomCTaskItem(config->roomId);
}

void Home::on_fingerManage_clicked()
{
    if(VerifyDialog::verifyId(this, VerifyDialog::VLeader) != QDialog::Accepted) return ;

    if(fingerManage == NULL){
        fingerManage = new FingerManage(this);
        connect(fingerManage, SIGNAL(backWidget()), this, SIGNAL(backWidget()));
        connect(fingerManage, SIGNAL(showMessage(QString,int)), this, SIGNAL(showMessage(QString,int)));
        emit widgetChange(fingerManage, true);
    } else {
        emit widgetChange(fingerManage, false);
    }
    fingerManage->initData();
}

void Home::on_outPolice_clicked()
{
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VTask);
    if( police.Id.isEmpty() ) return ;

    initTaskCabInfo(police, OutPolice);
}

void Home::on_urgent_clicked()
{
    if(VerifyDialog::verifyId(this, VerifyDialog::VLeader) != QDialog::Accepted) return ;
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( police.Id.isEmpty() ) return ;

    initTaskCabInfo(police, Emergency);
}

void Home::on_keep_clicked()
{
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( police.Id.isEmpty() ) return ;

    initTaskCabInfo(police, Keep);
}

void Home::on_dutyManage_clicked()
{
    if(VerifyDialog::verifyId(this, VerifyDialog::VManager) != QDialog::Accepted) return ;

    if(dutyManage == NULL){
        dutyManage = new DutyManage(this);
        emit widgetChange(dutyManage, true);
    } else {
        emit widgetChange(dutyManage, false);
    }
    dutyManage->init();
}

void Home::on_apply_clicked()
{
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VPolice);
    if( police.Id.isEmpty() ) return ;

    if( applyManage == NULL ){
        applyManage = new ApplyManage(this);
        emit widgetChange(applyManage, true);
    } else {
        emit widgetChange(applyManage, false);
    }
    applyManage->init(police);
}

void Home::on_inStorage_clicked()
{
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( police.Id.isEmpty() ) return ;

    initTaskCabInfo(police, Storage);
}

void Home::on_infoCheck_clicked()
{
    if( query == NULL ){
        query = new Query(this);
        emit widgetChange(query, true);
    } else {
        emit widgetChange(query, false);
    }
    query->init();
}

void Home::on_dumping_clicked()
{
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( police.Id.isEmpty() ) return ;

    initTaskCabInfo(police, Dumping);
}

void Home::on_tempDeposit_clicked()
{
    Police police = VerifyDialog::getPoliceData(this, VerifyDialog::VDutyManager);
    if( police.Id.isEmpty() ) return ;

    initTaskCabInfo(police, TempDeposit);
}
