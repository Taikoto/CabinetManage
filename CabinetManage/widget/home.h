#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QTimer>
#include "fingermanage.h"
#include "taskcabinfo.h"
#include "dutymanage.h"
#include "applymanage.h"
#include "query.h"
#include "network.h"

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = 0);
    ~Home();

signals:
    void backWidget();

    void widgetChange(QWidget* widget, bool isNew);

    void showMessage(QString text, int ms);

private:
    Ui::Home *ui;
    FingerManage *fingerManage;
    TaskCabInfo *cabinet;
    DutyManage *dutyManage;
    ApplyManage *applyManage;
    Query *query;

    Config *config;
    Network *network;

    QTimer timer; // 用来检测CurrentTask

    void initTaskCabInfo();
    void initTaskCabInfo(const DataModel::Police &police, const DataModel::TaskType &type);

private slots:
    void finished(const Network::DataCore &data);

    void onCheckCurrentTask();

    void on_fingerManage_clicked();

    void on_outPolice_clicked();

    void on_urgent_clicked();

    void on_keep_clicked();

    void on_dutyManage_clicked();

    void on_apply_clicked();

    void on_inStorage_clicked();

    void on_infoCheck_clicked();

    void on_dumping_clicked();

    void on_tempDeposit_clicked();
};

#endif // HOME_H
