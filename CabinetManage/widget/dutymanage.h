#ifndef DUTYMANAGE_H
#define DUTYMANAGE_H

#include <QWidget>
#include <QTimer>
#include "network.h"
#include "dataloop.h"

namespace Ui {
class DutyManage;
}

class DutyManage : public QWidget
{
    Q_OBJECT

public:
    explicit DutyManage(QWidget *parent = 0);
    ~DutyManage();

public slots:
    void init();

private:
    Ui::DutyManage *ui;
    Config *config;
    Network *network;
    DataLoop *dataLoop;
    QTimer updateTimer;

private slots:
    void finished(const Network::DataCore &data);
    void on_setDutyLeader_clicked();
    void on_hands_clicked();
    void on_onDuty_clicked();
    void on_offDuty_clicked();
};

#endif // DUTYMANAGE_H
