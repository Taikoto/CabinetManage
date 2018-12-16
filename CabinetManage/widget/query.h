#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include "network.h"
#include "datamodel.h"
#include "dataloop.h"

namespace Ui {
class Query;
}

class Query : public QDialog
{
    Q_OBJECT

public:
    explicit Query(QWidget *parent = 0);
    ~Query();

private:
    Ui::Query *ui;
    Network *network;
    Config *config;
    DataLoop *dataLoop;
    QString gun;

    void updateRoom(const DataModel::Room &r);
    void selectGun(const DataModel::CabinetList &list);

public slots:
    void init();

private slots:
    void finished(const Network::DataCore &data);
    void on_ok_clicked();
};

#endif // QUERY_H
