#ifndef FINGERMANAGE_H
#define FINGERMANAGE_H

#include <QWidget>
#include <Device>
#include <Network>
#include "database.h"
#include "dataloop.h"
#include "serialrfid.h"
#include "config.h"

namespace Ui {
class FingerManage;
}

class FingerManage : public QWidget
{
    Q_OBJECT

public:
    explicit FingerManage(QWidget *parent = 0);
    ~FingerManage();

    void initData();

signals:
    void backWidget();

    void showMessage(QString text, int ms);

private:
    Ui::FingerManage *ui;
    Database *database;
    Network *network;
    DataLoop *dataLoop;
    UsbFinger *finger;
    SerialRfid *sRfid;
    Config *config;

    void updatePoliceList();

    DataModel::PoliceBioList bList;
    void updatePoliceBioList(DataModel::PoliceBioList &list);

    int delBioIndex;// save delete bio index
    DataModel::BioType cBioType;// current bio type
    bool haveFinger[16];// existence finger
    void initHaveFinger() { for( int i = 0; i < 16; i++ ) haveFinger[i] = false; }

    void initFingerIcon();
    void setFingerIcon(int bioType, int bioCheckType);

private slots:
    void policeListClicked(int index);
    void finished(const Network::DataCore &data);
    void finished(const UsbFinger::DevData data);
    void finished(const SerialRfid::DevData data);
    void finished(Database::DataCore data);
    void addAndDel(DataModel::BioType type);
    void on_left01_clicked();
    void on_left02_clicked();
    void on_left03_clicked();
    void on_left04_clicked();
    void on_left05_clicked();
    void on_right05_clicked();
    void on_right04_clicked();
    void on_right03_clicked();
    void on_right02_clicked();
    void on_right01_clicked();
    void on_addRfid_clicked();
    void on_delRfid_clicked();
    void on_searchButton_clicked();
};

#endif // FINGERMANAGE_H
