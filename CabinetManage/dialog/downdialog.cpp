#include "downdialog.h"
#include "ui_downdialog.h"

#include <QFile>
#include <QPainter>
#include <QDebug>

using namespace DataModel;

DownDialog::DownDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::DownDialog)
{
    ui->setupUi(this);

    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条不可见
    ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条不可见

    initAll();

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    network = Network::instance();
    connect(network, SIGNAL(finished(Network::DataCore)), this, SLOT(finished(Network::DataCore)));

    dataLoop = DataLoop::instance();

    database = Database::instance();
    connect(database, SIGNAL(finished(Database::DataCore)), this, SLOT(finished(Database::DataCore)));

    uFinger = UsbFinger::instance();
    connect(uFinger, SIGNAL(finished(UsbFinger::DevData)), this, SLOT(finished(UsbFinger::DevData)));
}

DownDialog::~DownDialog()
{
    delete ui;
}

void DownDialog::initAll()
{
    pBar01Value = 0;
    pBar02Value = 0;

    ui->textBrowser->clear();
    ui->progressBar01->setValue(0);
    ui->progressBar02->setValue(0);

    ui->labelTime->setText("00:00:00");
}

int DownDialog::start(QString title)
{
    if(!title.isEmpty()) ui->title->setText(title);

    dataLoop->updateData();

    initAll();
    usedTime.restart();//重置计时器
    updateTimer.start(20);

    return exec();
}





/**
 ********************************************************************
 * public slots:
 ********************************************************************
*/

void DownDialog::setPBar01Value(int value)
{
    pBar01Value = value;
    ui->progressBar01->setValue(value);
}

void DownDialog::setPBar02Value(int value)
{
    pBar02Value = value;
    ui->progressBar02->setValue(value);
}





/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

//更新时间
void DownDialog::updateTime()
{
    int ms = usedTime.elapsed();
    ui->labelTime->setText(QString("%1:%2:%3").arg(ms/60000,2,10,QLatin1Char('0'))
                           .arg(ms%60000/1000,2,10,QLatin1Char('0'))
                           .arg(ms%1000/10,2,10,QLatin1Char('0')));

    //检查是否下载完成
    if(pBar01Value >= 100 && pBar02Value >= 100){
        updateTimer.stop();
        ui->textBrowser->append(tr("Download finish!"));
    }
}

void DownDialog::finished(const Network::DataCore &data)
{
    if(this->isHidden()) return;

    if(data.code == Network::Success){
        if(data.type == Network::GetPoliceList){

            policeNum = 0;
            policeCount = data.policeList.length();
            fingerNum = 0;//已下载到指纹模块的总数
            fingerCount = 0;//初始化指纹总数

            ui->textBrowser->append(tr("Connect server success!"));

            database->clrPolice();
            database->clrPoliceBio();
            uFinger->clearFinger();

            int biosLen;//一个警员的指纹总数
            for( int i = 0; i < policeCount; i++ )
            {
                database->addPolice(data.policeList[i], this);

                //更新指纹仪
                biosLen = data.policeList[i].PoliceBios.length();
                fingerCount += biosLen;
                for(int j = 0; j < biosLen; j++){
                    database->addPoliceBio(data.policeList[i].PoliceBios[j], this);
                    QByteArray key = QByteArray::fromBase64(data.policeList[i].PoliceBios[j].Key.toLatin1());
                    ushort fingerId = data.policeList[i].PoliceBios[j].FingerprintId;
                    uFinger->downFinger(key, fingerId, this);
                }
            }
            QFile file(":/img/Finger.dat");
            if(file.open(QIODevice::ReadOnly)){
                QByteArray dat = file.readAll();
                uFinger->downFinger(dat, 999, this);
                file.close();
            } else qDebug() << "Open Finger.dat failed!";
            //服务器上没有数据
            if(policeCount == 0) setPBar01Value(100);
            if(fingerCount == 0) setPBar02Value(100);
        }
    } else {
        ui->textBrowser->append(network->getErrorString(data.code));
        setPBar01Value(100);
        setPBar02Value(100);
    }
}

void DownDialog::finished(const UsbFinger::DevData data)
{
    if( data.parent != this ) return ;

    if( fingerCount != 0 ){
        fingerNum++;
        setPBar02Value(((double)fingerNum)/fingerCount*100);
    }

    if(data.code != PS_OK)
        ui->textBrowser->append(uFinger->getErrorString(data.code));
}

void DownDialog::finished(Database::DataCore data)
{
    if( data.parent != this ) return ;

    switch ( data.taskType ) {
    case Database::AddPolice:
        if( data.success ){
            policeNum++;
            setPBar01Value(((double)policeNum)/policeCount*100);
        } else {
            ui->textBrowser->append(tr("Save police data failed!"));
        }
        break;
    default:
        break;
    }
}

