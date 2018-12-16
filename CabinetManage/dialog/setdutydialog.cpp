#include "setdutydialog.h"
#include "ui_setdutydialog.h"

#include <QPainter>
#include "config.h"

using namespace DataModel;

SetDutyDialog::SetDutyDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::SetDutyDialog)
{
    ui->setupUi(this);

    ui->policeList->setTitle(tr("Leader list"));
    ui->policeList->addColumn(ZDiy::Label, tr("Name"));
    ui->policeList->addColumn(ZDiy::Label, tr("No"));
    ui->policeList->addColumn(ZDiy::Label, "", 40);
    ui->policeList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"), 24, 24);
    ui->policeList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"), 24, 24);
    ui->policeList->setPageItemCount(5);
    connect(ui->policeList, SIGNAL(itemClicked(int)), this, SLOT(listChecked(int)));

    config = Config::instance();

    network = Network::instance();
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    network->getPoliceList(config->roomId);
    network->getCurrentLead(config->roomId);
}

SetDutyDialog::~SetDutyDialog()
{
    delete ui;
}






/**
 ********************************************************************
 * private:
 ********************************************************************
*/

void SetDutyDialog::updatePoliceList(const PoliceList &list)
{
    leaderList.clear();
    ui->policeList->clear();
    int num = 0;//Police list counter
    for( int i = 0; i < list.length(); i++ )
    {
        if( list[i].PoliceType == Leader ){
            leaderList.append(list[i]);
            ui->policeList->addItem();
            ui->policeList->setItemText(num, 0, list[i].Name);
            ui->policeList->setItemText(num, 1, list[i].No);
            num++;
        }
    }
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void SetDutyDialog::listChecked(int index)
{
    if( index < 0 ) return ;

    for( int i = 0; i < ui->policeList->length(); i++ )
    {
        if( i == index )
            ui->policeList->setItemIcon(i, 2, QIcon(":/img/misc/pitchno.png"));
        else
            ui->policeList->setItemIcon(i, 2, QIcon());
    }
}

void SetDutyDialog::finished(const Network::DataCore &data)
{
    if( this->isHidden() ) return ;

    if(data.code == Network::Success){
        switch (data.type) {
        case Network::GetPoliceList:    updatePoliceList(data.policeList); break;
        case Network::GetCurrentLead:   cLeaderId = data.police.Id; break;
        default: break;
        }
        for( int i = 0; i < leaderList.length(); i++ )
        {
            if( cLeaderId == leaderList[i].Id ){
                ui->policeList->setItemIcon(i, 2, QIcon(":/img/misc/pitchno.png"));
                break;
            }
        }
    } else {

    }
}

void SetDutyDialog::on_ok_clicked()
{
    int index = ui->policeList->currentIndex();
    if( index < 0 || index >= ui->policeList->length() ) return ;

    DataModel::SetCurrent model;

    model.RoomId = config->roomId;
    //model.CurrentId
    model.NewId = leaderList[index].Id;

    network->postRoomLead(model);
    this->accept();
}
