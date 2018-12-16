#ifndef SubCabInfo_H
#define SubCabInfo_H

#include <QGroupBox>
#include <QLabel>
#include "datamodel.h"
#include "dataloop.h"

namespace Ui {
class SubCabInfo;
}

class SubCabInfo : public QGroupBox
{
    Q_OBJECT

public:
    explicit SubCabInfo(QWidget *parent = 0);
    ~SubCabInfo();

    void setData(DataModel::Cabinet *cab, int post);
    void setEnabled(const DataModel::TaskItem &item);
    void setTaskType(DataModel::TaskType taskType){ this->taskType = taskType; }

public slots:
    void init();//初始化配置
    void show();
    void hide();
    void setEnabled(bool enable);
    void setEnabledChecked(bool enable){ enableChecked = enable; }

signals:
    void checked(int no);

private:
    Ui::SubCabInfo *ui;
    DataLoop *dataLoop;
    QLabel checkLabel;//用来显示选中图片
    DataModel::SubCabinet *subCabinet;//保存这个子柜模型
    DataModel::TaskType taskType;
    int post;
    bool isEnabled;
    bool enableChecked;//使能选中模式

    void setObjectEnabled(const DataModel::TaskItem &item,
                          const DataModel::ObjectStatus &status, int objTypeId);

    void setGunData(const DataModel::Gun &gun);//! 设置枪支数据
    void setAmmoData(const DataModel::Ammo &ammo);//! 设置弹药数据

    void setPhoto(int typeId, int w = 0, int h = 0);//设置类型图片
    void showCheckedPhoto(bool enable);//显示选中图片

    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_photo_clicked();
};

#endif // SubCabInfo_H
