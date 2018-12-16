#include "fingermanage.h"
#include "ui_fingermanage.h"

#include <QFile>
#include <QList>
#include <QCoreApplication>
#include <QDebug>

#include "dialog/addidcarddialog.h"
#include "dialog/infodialog.h"
#include "dialog/verifydialog.h"

using namespace DataModel;

FingerManage::FingerManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FingerManage)
{
    ui->setupUi(this);

    ui->policeList->setTitle(tr("police list"));
    ui->policeList->addColumn(ZDiy::Label, tr("Name"));
    ui->policeList->addColumn(ZDiy::Label, tr("No"));
    ui->policeList->addColumn(ZDiy::Label, tr("PoliceType"));
    ui->policeList->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"), 32, 32);
    ui->policeList->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"), 32, 32);
    ui->policeList->setPageItemCount(10);
    connect(ui->policeList, SIGNAL(itemClicked(int)), this, SLOT(policeListClicked(int)));

    database = Database::instance(this);
    connect(database, SIGNAL(finished(Database::DataCore)), this, SLOT(finished(Database::DataCore)));

    config = Config::instance(this);

    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance(this);

    finger = UsbFinger::instance(this);
    connect(finger, SIGNAL(finished(UsbFinger::DevData)), this, SLOT(finished(UsbFinger::DevData)));

    sRfid = SerialRfid::instance(this);
    connect(sRfid, SIGNAL(finished(SerialRfid::DevData)), this, SLOT(finished(SerialRfid::DevData)));
    sRfid->setPortName(config->rfidPort);
}

FingerManage::~FingerManage()
{
    delete ui;
}

void FingerManage::initData()
{
    ui->policeInfo->init();
    ui->rfidCardNo->clear();
    initFingerIcon();
    initHaveFinger();//初始化 有指纹 标志位

    dataLoop->updatePoliceList();
    updatePoliceList();
}




/**
 ********************************************************************
 * private:
 ********************************************************************
*/

//更新警员列表
void FingerManage::updatePoliceList()
{
    ui->policeInfo->init();
    ui->rfidCardNo->clear();
    initFingerIcon();

    int index = ui->policeList->currentIndex();
    ui->policeList->clear();
    for(int i=0; i < dataLoop->polices.count(); i++)
    {
        ui->policeList->addItem();
        ui->policeList->setItemText(i, 0, dataLoop->polices[i].Name);
        ui->policeList->setItemText(i, 1, dataLoop->polices[i].No);
        ui->policeList->setItemText(i, 2, policeType(dataLoop->polices[i].PoliceType));
        QCoreApplication::processEvents();
    }
    ui->policeList->sendClicked(index);
}

//更新指纹列表
void FingerManage::updatePoliceBioList(PoliceBioList &list)
{
    bList = list;
    delBioIndex = -1;

    initFingerIcon();
    initHaveFinger();//初始化 有指纹 标志位

    for( int i = 0; i < bList.count(); i++ )
    {
        setFingerIcon(bList[i].BioType, bList[i].BioCheckType);
        haveFinger[bList[i].BioType] = true;//此处有指纹
    }
}

void FingerManage::initFingerIcon()
{
    ui->left05->setIcon(QIcon(":/img/fingerprint/leftfinger/Finger5_1.png"));
    ui->left04->setIcon(QIcon(":/img/fingerprint/leftfinger/Finger4_1.png"));
    ui->left03->setIcon(QIcon(":/img/fingerprint/leftfinger/Finger3_1.png"));
    ui->left02->setIcon(QIcon(":/img/fingerprint/leftfinger/Finger2_1.png"));
    ui->left01->setIcon(QIcon(":/img/fingerprint/leftfinger/Finger1_1.png"));
    ui->right05->setIcon(QIcon(":/img/fingerprint/rightfinger/Finger5_1.png"));
    ui->right04->setIcon(QIcon(":/img/fingerprint/rightfinger/Finger4_1.png"));
    ui->right03->setIcon(QIcon(":/img/fingerprint/rightfinger/Finger3_1.png"));
    ui->right02->setIcon(QIcon(":/img/fingerprint/rightfinger/Finger2_1.png"));
    ui->right01->setIcon(QIcon(":/img/fingerprint/rightfinger/Finger1_1.png"));
}

//根据BioType设置相应手指的图片
void FingerManage::setFingerIcon(int bioType, int bioCheckType)
{
    int num = 1;
    if(bioCheckType == (int)BioNormal) num = 3;
    else if(bioCheckType == (int)BioAbnormal) num = 2;

    QString head(":/img/fingerprint/");
    switch ((BioType)bioType) {
    case LeftThumb:     ui->left05->setIcon(QIcon(head+QString("leftfinger/Finger5_%1.png").arg(num))); break;
    case LeftFore:      ui->left04->setIcon(QIcon(head+QString("leftfinger/Finger4_%1.png").arg(num))); break;
    case LeftMedius:    ui->left03->setIcon(QIcon(head+QString("leftfinger/Finger3_%1.png").arg(num))); break;
    case LeftRing:      ui->left02->setIcon(QIcon(head+QString("leftfinger/Finger2_%1.png").arg(num))); break;
    case LeftPinky:     ui->left01->setIcon(QIcon(head+QString("leftfinger/Finger1_%1.png").arg(num))); break;
    case RightThumb:    ui->right05->setIcon(QIcon(head+QString("rightfinger/Finger5_%1.png").arg(num))); break;
    case RightFore:     ui->right04->setIcon(QIcon(head+QString("rightfinger/Finger4_%1.png").arg(num))); break;
    case RightMedius:   ui->right03->setIcon(QIcon(head+QString("rightfinger/Finger3_%1.png").arg(num))); break;
    case RightRing:     ui->right02->setIcon(QIcon(head+QString("rightfinger/Finger2_%1.png").arg(num))); break;
    case RightPinky:    ui->right01->setIcon(QIcon(head+QString("rightfinger/Finger1_%1.png").arg(num))); break;
    default: break;
    }
}

/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void FingerManage::policeListClicked(int index)
{
    ui->policeInfo->setData(dataLoop->polices[index]);
    updatePoliceBioList(dataLoop->polices[index].PoliceBios);//更新指纹列表

    //RFID信息显示
    ui->rfidCardNo->setText(dataLoop->polices[index].reserved);
}

void FingerManage::finished(const Network::DataCore &data)
{
    if(this->isHidden()) return;

    if(data.code == Network::Success){
        if(data.type == Network::GetPoliceList){
            //updatePoliceList(data.policeList);//更新警员列表
        }
        else if(data.type == Network::PostPoliceBio){
            updatePoliceBioList(bList);//更新指纹列表
            InfoDialog::information(this, data.message.Body);//上传指纹数据成功
        }
        else if(data.type == Network::GetDelPoliceBio){
            //服务器删除指纹成功，则开始删除硬件指纹数据
            finger->deleteFinger(bList[delBioIndex].FingerprintId, this);//删除设备指纹
            database->delPoliceBio(bList[delBioIndex].Id);//删除数据库中的数据
            bList.removeAt(delBioIndex);
            dataLoop->polices[ui->policeList->currentIndex()].PoliceBios.removeAt(delBioIndex);
            updatePoliceBioList(bList);//更新指纹列表
            InfoDialog::information(this, tr("Delete fingerprint success!"));
        }
        else if(data.type == Network::PostPoliceIdCard){
            database->delPolice(dataLoop->polices[ui->policeList->currentIndex()].Id, this);
            database->addPolice(dataLoop->polices[ui->policeList->currentIndex()], this);
            ui->rfidCardNo->setText(dataLoop->polices[ui->policeList->currentIndex()].reserved);
            InfoDialog::information(this, tr("Police data update success!"));
        }
    }
    else {
        InfoDialog::error(this, network->getErrorString(data.code));
        emit backWidget();
    }
}

void FingerManage::finished(const UsbFinger::DevData data)
{
    if( isHidden() ) return ;

    if(data.code == PS_USER_CANCEL) return ;
    if(data.code != PS_OK){
        InfoDialog::error(this, finger->getErrorString(data.code));
        return ;
    }
    if(data.type == UsbFinger::Enroll){

        PoliceBio bio;
        bio.Id = DataModel::uuid();
        bio.PoliceId = dataLoop->polices[ui->policeList->currentIndex()].Id;
        bio.BioType = (int)cBioType;
        if(ui->typeNormal->isChecked()) bio.BioCheckType = (int)BioNormal;
        else bio.BioCheckType = (int)BioAbnormal;
        bio.FingerprintId = data.fingerId;
        bio.Key = data.templet.toBase64();
        bio.TemplateType = "LD9900";

        network->postPoliceBio(bio);//上传指纹数据
        database->addPoliceBio(bio, this);//写入数据库
        dataLoop->polices[ui->policeList->currentIndex()].PoliceBios.append(bio);
        bList.append(bio);

        InfoDialog::information(this, tr("Uploading data..."));//正在上传数据

    } else if(data.type == UsbFinger::DeleteFinger){
        //指纹删除流程完成，只在乎服务器中是否删除成功
    }
}

void FingerManage::finished(const SerialRfid::DevData data)
{
    if( data.parent != this || isHidden() ) return ;

    //检错
    if(data.code == SerialRfid::UserCanceled) return ;
    if( data.code != SerialRfid::Success ){
        InfoDialog::error(this, sRfid->getErrorString(data.code));
        return ;
    }

    if( data.type == SerialRfid::GetCardId ){

        for( int i = 0; i < dataLoop->polices.length(); i++ )
        {
            if(dataLoop->polices[i].reserved == data.cardId){
                InfoDialog::error(this, tr("This rfid card has been bound!"));
                return ;
            }
        }

        AddIdCard addIdCard;
        addIdCard.PoliceId  = dataLoop->polices[ui->policeList->currentIndex()].Id;
        addIdCard.Name      = dataLoop->polices[ui->policeList->currentIndex()].Name;
        //addIdCard.Sex       = "man";
        //addIdCard.Nation    = "han";
        //addIdCard.Address   = "dizhi";
        //addIdCard.IDCardNo  = "123";
        //addIdCard.Born      = "19700101";
        //addIdCard.GrantDept = "jiguan";
        //addIdCard.UserLifeBegin = "19700101";
        //addIdCard.UserLifeEnd = "19700101";
        addIdCard.reserved  = data.cardId;
        addIdCard.PhotoFileName = getPhotoBase64(":/img/photo/headPhoto.jpg");

        network->postPoliceIdCard(addIdCard);

        dataLoop->polices[ui->policeList->currentIndex()].reserved = data.cardId;
    }
}

void FingerManage::finished(Database::DataCore data)
{
    if( data.parent != this ) return ;

    if( data.success ){
        switch (data.taskType) {
        case Database::FindBiosByPId:
            updatePoliceBioList(data.bioList);//更新指纹列表
            break;
        case Database::AddPoliceBio:
            break;
        default:
            break;
        }
    } else {
        qDebug() << "Database task error, type =" << data.taskType;
    }
}

void FingerManage::addAndDel(BioType type)
{
    if( haveFinger[(int)type] ){
        //删除
        for( int i = 0; i < bList.length(); i++ )
        {
            if(bList[i].BioType == (int)type){
                delBioIndex = i;
                network->getDelPoliceBio(bList[i].Id);//发送删除指纹请求
            }
        }
        InfoDialog::information(this, tr("Deleting fingerprint..."));//正在删除指纹
    } else {
        //增加
        cBioType = type;
        VerifyDialog::enrollFinger(this);
    }
}

void FingerManage::on_left01_clicked()
{
    addAndDel(LeftPinky);
}

void FingerManage::on_left02_clicked()
{
    addAndDel(LeftRing);
}

void FingerManage::on_left03_clicked()
{
    addAndDel(LeftMedius);
}

void FingerManage::on_left04_clicked()
{
    addAndDel(LeftFore);
}

void FingerManage::on_left05_clicked()
{
    addAndDel(LeftThumb);
}

void FingerManage::on_right05_clicked()
{
    addAndDel(RightThumb);
}

void FingerManage::on_right04_clicked()
{
    addAndDel(RightFore);
}

void FingerManage::on_right03_clicked()
{
    addAndDel(RightMedius);
}

void FingerManage::on_right02_clicked()
{
    addAndDel(RightRing);
}

void FingerManage::on_right01_clicked()
{
    addAndDel(RightPinky);
}

void FingerManage::on_addRfid_clicked()
{
    int index = ui->policeList->currentIndex();
    if( index < 0 || index >= dataLoop->polices.length() ) return ;

    if( !config->haveRFID ){
        InfoDialog::information(this, tr("RFID device not open!"));
        return ;
    }

    sRfid->getCardId(SerialRfid::ReadOnly, this);
    InfoDialog::information(this, tr("Please brush RFID card"));
}

void FingerManage::on_delRfid_clicked()
{
    int index = ui->policeList->currentIndex();
    if( index < 0 || index >= dataLoop->polices.length() ) return ;

    AddIdCard addIdCard;
    addIdCard.PoliceId  = dataLoop->polices[index].Id;
    addIdCard.Name      = dataLoop->polices[index].Name;
    //addIdCard.Sex       = "man";
    //addIdCard.Nation    = "han";
    //addIdCard.Address   = "dizhi";
    //addIdCard.IDCardNo  = "123";
    //addIdCard.Born      = "19700101";
    //addIdCard.GrantDept = "jiguan";
    //addIdCard.UserLifeBegin = "19700101";
    //addIdCard.UserLifeEnd = "19700101";
    //addIdCard.reserved  = data.cardId;
    addIdCard.PhotoFileName = getPhotoBase64(":/img/photo/headPhoto.jpg");
    dataLoop->polices[index].reserved.clear();

    network->postPoliceIdCard(addIdCard);
}

void FingerManage::on_searchButton_clicked()
{
    for(int i = 0; i < dataLoop->polices.count(); i++)
    {
        if(ui->policeNo->text() == dataLoop->polices[i].No){
            ui->policeList->sendClicked(i);
        }
    }
}
