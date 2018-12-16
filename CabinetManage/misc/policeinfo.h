#ifndef POLICEINFO_H
#define POLICEINFO_H

#include <QGroupBox>
#include "datamodel.h"

namespace Ui {
class PoliceInfo;
}

class PoliceInfo : public QGroupBox
{
    Q_OBJECT

public:
    explicit PoliceInfo(QWidget *parent = 0);
    ~PoliceInfo();

public slots:
    void init();
    void setTitle(QString text);
    void setData(DataModel::Police police);

private:
    Ui::PoliceInfo *ui;
};

#endif // POLICEINFO_H
