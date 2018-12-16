#ifndef TASKCABINFO_H
#define TASKCABINFO_H

#include <QWidget>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

#include "config.h"
#include "network.h"
#include "dataloop.h"

namespace Ui {
class TaskCabInfo;
}

class TaskCabInfo : public QWidget
{
    Q_OBJECT

public:
    explicit TaskCabInfo(QWidget *parent = 0);
    ~TaskCabInfo();

    void init();

public slots:
    void init(DataModel::Police taskPolice, const DataModel::TaskType &type);
    void init(QString taskId);

signals:
    void backWidget();

    void showMessage(QString text, int ms);

private:
    Ui::TaskCabInfo *ui;
    Config *config;
    Network *network;

    DataModel::TaskType taskType;
    DataModel::Task task;
    DataModel::TaskItemList *cTaskItems;// current police task item list

    void showTaskInfo();

    bool checkTaskStatus(const DataModel::TaskStatus &status);
    bool checkTaskType(const DataModel::TaskType &type);

private slots:
    void finished(const Network::DataCore &data);
    void on_taskItemType_currentIndexChanged(int index);
    void on_taskPolices_currentIndexChanged(int index);
};

#endif // TASKCABINFO_H
