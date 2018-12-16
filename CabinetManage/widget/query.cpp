#include "query.h"
#include "ui_query.h"

using namespace DataModel;

Query::Query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Query)
{
    ui->setupUi(this);

    config = Config::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)),this,SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this);
}

Query::~Query()
{
    delete ui;
}

void Query::init()
{
    ui->cabinetInfo->init();
    ui->cabinetInfo->hideButton();

    network->getCabById(config->cabId);//刷新CabinetInfo
    network->getRoomById(config->roomId);
}

void Query::updateRoom(const DataModel::Room &r)
{
    ui->unitName->setText(r.Name);
    ui->unitNum->setText(r.No);

    network->getCurrentLead(r.Id);
    network->getCurrentManage(r.Id);
}

void Query::selectGun(const DataModel::CabinetList &list)
{
    for(int i = 0; i < list.length(); i++)
    {
        for(int j = 0; j < list[i].SubCabs.length(); j++)
        {
            const GunList &guns = list[i].SubCabs[j].Guns;
            if(!guns.isEmpty() && gun == guns.last().No){
                ui->status->setText(objectStatus(guns.last().ObjectStatus));
                return ;
            }
        }
    }
}

//******private slots******
void Query::finished(const Network::DataCore &data)
{
    if(data.code == Network::Success)
    {
        switch(data.type){
        case Network::GetCabById:
            //ui->cabinetInfo里面更新了
            break;
        case Network::GetRoomById:
            updateRoom(data.room);
            break;
        case Network::GetCurrentLead:
            ui->leadName->setText(data.police.Name);
            ui->leadNum->setText(data.police.No);
            break;
        case Network::GetCurrentManage:
            break;
        case Network::GetPoliceById:
            ui->manageName->setText(data.police.Name);
            ui->manageNum->setText(data.police.No);
            break;
        case Network::GetCabList:
            selectGun(data.cabinetList);
            break;
        default: break;
        }
    }
}

void Query::on_ok_clicked()
{
    gun = ui->gunId->text();
    network->getCabList(config->roomId);
    ui->status->clear();
}
