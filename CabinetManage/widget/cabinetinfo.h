#ifndef CABINETINFO_H
#define CABINETINFO_H

#include <QGroupBox>
#include <QTimer>
#include <Device>

#include "config.h"
#include "network.h"
#include "dataloop.h"
#include "seriallock.h"
#include "subcabinfo.h"

namespace Ui {
class CabinetInfo;
}

class CabinetInfo : public QGroupBox
{
    Q_OBJECT

public:
    explicit CabinetInfo(QWidget *parent = 0);
    ~CabinetInfo();

signals:
    void openCabinet(bool); // 打开柜门信号，发送false

public slots:
    void init();

    void setData(const DataModel::TaskItem &item, DataModel::TaskType type, int operCount);

    void hideButton();

private:
    Ui::CabinetInfo *ui;

    Config *config;
    Network *network;
    DataLoop *dataLoop;
    SerialLock *sLock;
    Arm4418 *arm4418;

    QList<SubCabInfo *> boxs;

    DataModel::TaskItem taskItem; // 任务清单
    DataModel::TaskType taskType; // 任务类型
    int operCount; // 可操作总数
    int currentPage; // 当前显示的第几页
    int pageCount; // 总页数
    QTimer numberTimer; // 更新显示取弹数量定时器

    void showPage(int index);
    void setCabinetData(DataModel::Cabinet &cabinet);

    bool operChecked();//用户操作检查

    bool uploadGunOper(int index);//上传枪支操作记录
    bool uploadAmmoOper(int index);//上传弹药操作记录

private slots:
    void updateGetNumber();//更新显示取弹数量
    void finished(const Network::DataCore &data);
    void finished(const SerialLock::DevData data);
    void finished(Arm4418::DataCore data);
    void subCabinetChecked(int no);
    void on_pb_back_clicked();
    void on_pb_next_clicked();
    void on_pb_openCab_clicked();
    void on_openLock_clicked();
    void on_getNumber_clicked();
    void on_openAll_clicked();
};

#endif // CABINETINFO_H
