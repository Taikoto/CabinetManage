#include "policeinfo.h"
#include "ui_policeinfo.h"

using namespace DataModel;

PoliceInfo::PoliceInfo(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PoliceInfo)
{
    ui->setupUi(this);
}

PoliceInfo::~PoliceInfo()
{
    delete ui;
}

void PoliceInfo::init()
{
    ui->photo->setIcon(QIcon(":/img/photo/headPhoto.jpg"));
    ui->name->clear();
    ui->policeType->clear();
    ui->no->clear();
    ui->departy->clear();
    ui->phone->clear();
}

void PoliceInfo::setTitle(QString text)
{
    ui->title->setText(text);
}

void PoliceInfo::setData(DataModel::Police police)
{
    if(!police.PhotoBase64.isEmpty()){
        QPixmap photo;
        QByteArray dat = QByteArray::fromBase64(police.PhotoBase64.toLatin1());
        photo.loadFromData(dat);
        ui->photo->setIcon(QIcon(photo));
    }
    ui->name->setText(police.Name);
    ui->policeType->setText(policeType(police.PoliceType));
    ui->no->setText(police.No);
    ui->departy->setText(departy(police.Departy));
    ui->phone->setText(police.Phone);
}
