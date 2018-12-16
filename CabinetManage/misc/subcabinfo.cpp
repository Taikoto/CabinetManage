#include "subcabinfo.h"
#include "ui_subcabinfo.h"

#include <QDebug>

using namespace DataModel;

SubCabInfo::SubCabInfo(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::SubCabInfo)
{
    ui->setupUi(this);

    dataLoop = DataLoop::instance(this);

    subCabinet = NULL;

    isEnabled = true;

    //设置选中图片
    QPixmap pixMap(":/img/misc/pitchno.png");
    checkLabel.setParent(this);
    checkLabel.setPixmap(pixMap.scaled(80, 80));
    checkLabel.hide();
}

SubCabInfo::~SubCabInfo()
{
    delete ui;
}




/**
 ********************************************************************
 * public:
 ********************************************************************
*/

void SubCabInfo::setData(Cabinet *cab, int post)
{
    init();
    subCabinet = &(cab->SubCabs[post]);//保存子柜信息，用于点击时，修改是否被选中
    this->post = post;
    this->show();

    //设置编号
    //int high = post/8+1, wight = post%8+1;
    //ui->lb_post->setText(QString("%1 - %2").arg(high).arg(wight));
    ui->lb_post->setText(QString::number(post+1));

    showCheckedPhoto(subCabinet->isChecked);//显示选中图片

    if (!subCabinet->Guns.isEmpty()) setGunData(subCabinet->Guns.last());
    else if (!subCabinet->Ammos.isEmpty()) setAmmoData(subCabinet->Ammos.last());
    else {
        ui->lb_state->setText(tr("Void"));//空位
        ui->photo->setIcon(QIcon());
    }
}

void SubCabInfo::setEnabled(const TaskItem &item)
{
    if (subCabinet == NULL) return ;
    if (item.Id.isEmpty()) { setEnabled(true); return ; }

    //使能选中模式
    if(enableChecked){
        if(subCabinet->isChecked) setEnabled(true);
        else setEnabled(false);
        return ;
    }

    if (!subCabinet->Guns.isEmpty()) {
        Gun &gun = subCabinet->Guns.last();
        setObjectEnabled(item, gun.ObjectStatus, gun.ObjectTypeId);
    }
    else if (!subCabinet->Ammos.isEmpty()) {
        Ammo &ammo = subCabinet->Ammos.last();
        setObjectEnabled(item, ammo.ObjectStatus, ammo.ObjectTypeId);
    }
    else this->setEnabled(false); // 空位
}




/**
 ********************************************************************
 * public slots:
 ********************************************************************
*/

void SubCabInfo::init()
{
    subCabinet = NULL;
    post = -1;
    enableChecked = false;

    ui->lb_state->clear();
    ui->lb_post->clear();
    ui->photo->setIcon(QIcon());
    ui->lb_Info01->clear();
    ui->lb_info02->clear();
    ui->lb_info03->clear();

    //hide();
}

void SubCabInfo::show()
{
    this->setEnabled(true);

    ui->lb_state->show();
    ui->lb_post->show();
    ui->photo->show();
    ui->lb_Info01->show();
    ui->lb_info02->show();
    ui->lb_info03->show();
    this->setStyleSheet("");

    checkLabel.hide();
}

void SubCabInfo::hide()
{
    this->setEnabled(false);

    ui->lb_state->hide();
    ui->lb_post->hide();
    ui->photo->hide();
    ui->lb_Info01->hide();
    ui->lb_info02->hide();
    ui->lb_info03->hide();
    this->setStyleSheet("QGroupBox{ border: 0px; }");

    checkLabel.hide();
}

void SubCabInfo::setEnabled(bool enable)
{
    ui->photo->setEnabled(enable);
    checkLabel.setEnabled(enable);
    isEnabled = enable;
}




/**
 ********************************************************************
 * private:
 ********************************************************************
*/

// 判断是否禁用BOX
void SubCabInfo::setObjectEnabled(const TaskItem &item,
                                  const DataModel::ObjectStatus &status,
                                  int objTypeId)
{
    if (item.ObjectTypeId != objTypeId) {
        setEnabled(false); // Object type id 不匹配直接禁用
        return ;
    }

    bool isInLib = isInLibrary(status); // 是否在库
    bool isGetObj = isGetObject(item.TaskItemStatus); // 是否是来领取的
    bool enable = false;

    switch (taskType) {
    case Emergency:
    case OutPolice:
    case Keep:
        enable = isGetObj ? isInLib : !isInLib;
        break;
    case Storage:
        if (status == ApplyStorage) enable = isGetObj ? !isInLib : isInLib;
        break;
    case Dumping:
        if (status == ApplyScrap) enable = isGetObj ? isInLib : !isInLib;
        break;
    case TempDeposit:
        // 来存放 只能 存到申请临时存放的位置上
        if (isGetObj && status == ApplyTemporary) enable = true;
        else if (!isGetObj && status == InLibrary) enable = true;
        break;
    case OtherTaskType:
    default: break;
    }
    setEnabled(enable);
}

//! 设置枪支数据
void SubCabInfo::setGunData(const Gun &gun)
{
    ui->lb_state->setText(objectStatus(gun.ObjectStatus)); // 显示状态信息
    if (isInLibrary(gun.ObjectStatus)) {
        ui->lb_info03->setText(tr("Take"));//领取
    } else {
        ui->lb_info03->setText(tr("Deposit")); // 存放
    }

    //设置枪支弹药类型名称
    ui->lb_Info01->setText(DataLoop::findObjType(gun.ObjectTypeId).Name);

    setPhoto(gun.ObjectTypeId);//设置图片
    ui->lb_info02->setText(tr("No: ") + gun.No);//枪支编号
}

//! 设置弹药数据
void SubCabInfo::setAmmoData(const Ammo &ammo)
{
    ui->lb_state->setText(objectStatus(ammo.ObjectStatus)); // 显示状态信息
    if (isInLibrary(ammo.ObjectStatus)) {
        ui->lb_info03->setText(tr("Take"));//领取
    } else {
        ui->lb_info03->setText(tr("Deposit"));
    }

    //设置枪支弹药类型名称
    ui->lb_Info01->setText(DataLoop::findObjType(ammo.ObjectTypeId).Name);

    setPhoto(ammo.ObjectTypeId);//设置图片
    ui->lb_info02->setText(tr("Number: ") + QString::number(ammo.ObjectNumber));//弹药数量
}

//! 设置类型图片
void SubCabInfo::setPhoto(int typeId, int w, int h)
{
    QString root = QApplication::applicationDirPath();//应用程序路径
    ui->photo->setIcon(QIcon(root+"/image/objecttype/"+QString::number(typeId)+".png"));

    QSize size;
    if( w == 0 ) w = ui->photo->width() - 10;
    if( h == 0 ) h = ui->photo->height() - 10;
    size.setWidth(w);
    size.setHeight(h);
    ui->photo->setIconSize(size);
}

//! 显示选中图片
void SubCabInfo::showCheckedPhoto(bool enable)
{
    if(enable){
        checkLabel.setGeometry(width()-70, height()-70, 80, 80);
        checkLabel.show();
    } else {
        checkLabel.hide();
    }
}

//! 鼠标释放事件
void SubCabInfo::mouseReleaseEvent(QMouseEvent *)
{
    if( isEnabled ) on_photo_clicked();
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void SubCabInfo::on_photo_clicked()
{
    if(subCabinet == NULL) return;

    emit checked(post);

    subCabinet->isChecked = !subCabinet->isChecked;
    showCheckedPhoto(subCabinet->isChecked);//显示选中图片
}
