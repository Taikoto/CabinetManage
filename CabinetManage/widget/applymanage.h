#ifndef APPLYMANAGE_H
#define APPLYMANAGE_H

#include <QWidget>
#include "network.h"
#include "dataloop.h"

namespace Ui {
class ApplyManage;
}

class ApplyManage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyManage(QWidget *parent = 0);
    ~ApplyManage();

public slots:
    void init(DataModel::Police police);

    void setInfoBoxEnabled(bool enable);//设置左边信息GroupBox是否禁用

signals:
    void backWidget();

    void showMessage(QString text, int ms);//显示提示信息

protected:

private:
    Ui::ApplyManage *ui;
    Config *config;
    Network *network;
    DataLoop *dataLoop;

    DataModel::Task task;
    DataModel::TaskItemList gunItemList, ammoItems, clipItems;
    DataModel::Police taskPolice;

    void updateItemList();//更新任务清单列表

    //设置ItemList数据
    void setItemList(const int &index, const DataModel::TaskItem &item, const QString &name);

private slots:
    void finished(const Network::DataCore data);

    void textChanged(int itemNo, int widgetNo);

    void on_startTimeButton_clicked();

    void on_endTimeButton_clicked();

    void on_addPolice_clicked();
    void on_delPolice_clicked();
    void on_ok_clicked();
};

#endif // APPLYMANAGE_H
