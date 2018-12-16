#include "dataloop.h"

using namespace DataModel;

DataLoop* DataLoop::m_instance = NULL;

DataLoop* DataLoop::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new DataLoop(parent);
    return m_instance;
}

DataLoop::DataLoop(QObject *parent) :
    QObject(parent)
{
    config = Config::instance(this);
    network = Network::instance(this);
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    updateData();
}




/**
 ********************************************************************
 * public:
 ********************************************************************
*/

ObjectType DataLoop::findObjType(int typeId)
{
    if( m_instance == NULL ) return ObjectType();

    return objectType(m_instance->types, typeId);
}

Police DataLoop::findPolice(const QString &id)
{
    if( m_instance == NULL ) return Police();

    for( int i = 0; i < m_instance->polices.length(); i++ )
    {
        if(id == m_instance->polices[i].Id) return m_instance->polices[i];
    }
    return Police();
}

Police DataLoop::findPolice(const int &fingerId)
{
    if( m_instance == NULL ) return Police();

    for( int i = 0; i < m_instance->polices.length(); i++ )
    {
        PoliceBioList &bios = m_instance->polices[i].PoliceBios;
        for( int j = 0; j < bios.length(); j++ )
        {
            if( fingerId == bios[j].FingerprintId ) return m_instance->polices[i];
        }
    }
    return Police();
}

Police DataLoop::findPoliceByRfid(const QString &rfid)
{
    if( m_instance == NULL ) return Police();

    for( int i = 0; i < m_instance->polices.length(); i++ )
    {
        if(m_instance->polices[i].reserved == rfid) return m_instance->polices[i];
    }
    return Police();
}

PoliceBioList DataLoop::findAllBio()
{
    PoliceBioList bios;
    if( m_instance == NULL ) return bios;

    for( int i = 0; i < m_instance->polices.length(); i++ )
    {
        bios.append(m_instance->polices[i].PoliceBios);
    }
    return bios;
}




/**
 ********************************************************************
 * public slots:
 ********************************************************************
*/

void DataLoop::updateData()
{
    network->getTypeList();
    updateCabinet();
    network->getCabList(config->roomId);
    network->getPoliceList(config->roomId);
    network->getCurrentLead(config->roomId);
    network->getCurrentManage(config->roomId);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void DataLoop::finished(const Network::DataCore &data)
{
    if( data.code == Network::Success )
    {
        switch(data.type){
        case Network::GetTypeList:      types = data.typeList;   break;
        case Network::GetCabById:       cabinet = data.cabinet; break;
        case Network::GetCabList:       cabinets = data.cabinetList; break;
        case Network::GetPoliceList:    polices = data.policeList; break;
        case Network::GetCurrentLead:   cLeader = data.police; break;
        case Network::GetCurrentManage: cManagers = data.policeList;  break;
        default: break;
        }
        emit finished(data.type);
    }
}

