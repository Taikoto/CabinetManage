#ifndef DOWNDIALOG_H
#define DOWNDIALOG_H

#include <QTime>
#include <QTimer>

#include "network.h"
#include "dataloop.h"
#include "database.h"
#include "usbfinger.h"
#include "dialog.h"

namespace Ui {
class DownDialog;
}

class DownDialog : public Dialog
{
    Q_OBJECT
public:
    explicit DownDialog(QWidget *parent = 0);
    ~DownDialog();

    void initAll();
    int start(QString title = 0);

public slots:
    void setPBar01Value(int value);
    void setPBar02Value(int value);

private slots:
    void updateTime();

    void finished(const Network::DataCore &data);

    void finished(const UsbFinger::DevData data);

    void finished(Database::DataCore data);

private:
    Ui::DownDialog *ui;

    int pBar01Value;
    int pBar02Value;

    int policeNum;//已保存的警员数量
    int policeCount;
    int fingerNum;//已下载到指纹模块的总数
    int fingerCount;//指纹总数

    QTime usedTime;//使用时间计时器
    QTimer updateTimer;//更新现实时间定时器

    Network *network;
    DataLoop *dataLoop;
    Database *database;
    UsbFinger *uFinger;
};

#endif // DownDialog_H
