#ifndef DATALOOP_H
#define DATALOOP_H

#include <QObject>
#include <Network>
#include "datamodel.h"
#include "config.h"

class DataLoop : public QObject
{
    Q_OBJECT
public:
    static DataLoop* instance(QObject *parent = 0);
    explicit DataLoop(QObject *parent = 0);

    static DataModel::ObjectType findObjType(int typeId);
    static DataModel::Police findPolice(const QString &id);
    static DataModel::Police findPolice(const int &fingerId);
    static DataModel::Police findPoliceByRfid(const QString &rfid);
    static DataModel::PoliceBioList findAllBio();

    DataModel::ObjectTypeList types; // ammos type list
    DataModel::PoliceList polices; // police list
    DataModel::Cabinet cabinet; // current cabinet
    DataModel::CabinetList cabinets;
    DataModel::Police cLeader; // current duty leader 
    DataModel::PoliceList cManagers; // current duty manager list

signals:
    void finished(Network::TaskType);

public slots:
    void updateData();
    void updateTypeList(){ network->getTypeList(); }
    void updateCabinet(){ network->getCabById(config->cabId); }
    void updateCabList(){ network->getCabList(config->roomId); }
    void updatePoliceList(){ network->getPoliceList(config->roomId); }
    void updateCurrentLead(){ network->getCurrentLead(config->roomId); }
    void updateCurrentManage(){ network->getCurrentManage(config->roomId); }

private slots:
    void finished(const Network::DataCore &data);

private:
    Network *network;
    Config *config;
    static DataLoop* m_instance;
};

#endif // DATALOOP_H
