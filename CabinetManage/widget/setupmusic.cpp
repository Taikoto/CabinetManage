#include "setupmusic.h"
#include "ui_setupmusic.h"

#include <QDir>
#include <QFileInfoList>
#include <QDebug>

SetupMusic::SetupMusic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupMusic)
{
    ui->setupUi(this);

    player = MediaPlayer::instance(this);
    connect(player, SIGNAL(finished(int)), this, SLOT(playFinished(int)));
    connect(player, SIGNAL(readyRead()), this, SLOT(readOutput()));

    ui->groupBox->setTitle(tr("Music list"));
    ui->groupBox->addColumn(ZDiy::Label, tr("No"), 25);
    ui->groupBox->addColumn(ZDiy::Label, tr("Music"));
    ui->groupBox->setPreviousButtonIcon(QIcon(":/img/misc/prewhite.png"));
    ui->groupBox->setNextButtonIcon(QIcon(":/img/misc/nextwhite.png"));
    ui->groupBox->setPageItemCount(10);
    connect(ui->groupBox, SIGNAL(itemClicked(int)), this, SLOT(itemClicked(int)));

    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条不可见
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条不可见

#ifdef Q_OS_WIN32
    path = "D:/KuGou/";
#endif
#ifdef Q_OS_LINUX
    path = "/home/mm/";
#endif
    cIndex = 0;
    volume = 80;
    isButton = false;
}

SetupMusic::~SetupMusic()
{
    delete ui;
}

void SetupMusic::init()
{
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    ui->groupBox->clear();
    for ( int i = 0; i < list.length(); i++ )
    {
        QFileInfo fileInfo = list.at(i);
        ui->groupBox->addItem();
        ui->groupBox->setItemText(i, 0, QString::number(i));
        ui->groupBox->setItemText(i, 1, fileInfo.fileName());
    }
}

void SetupMusic::itemClicked(int index)
{
    if( isHidden() || parentWidget()->isHidden() ) return ;
    if( index < 0 || index >= ui->groupBox->length() ) return ;

    QFileInfo fileInfo(path + ui->groupBox->itemText(index, 1));
    cIndex = index;

    player->play(fileInfo.filePath());
}

void SetupMusic::readOutput()
{
    QString text = player->readAll();
    ui->textEdit->append(text);
}

void SetupMusic::playFinished(int exitCode)
{
    if( isHidden() || parentWidget()->isHidden() ) return ;

    qDebug() << "Process finished, exit code =" << exitCode;
    if( exitCode != 0) return ;
    if(isButton){ isButton = false; return ; }

    if( cIndex + 1 >= ui->groupBox->length() ) cIndex = -1;
    ui->groupBox->sendClicked(cIndex + 1);
}

void SetupMusic::on_addVolume_clicked()
{
    player->write("volume +5\n");
}

void SetupMusic::on_subVolume_clicked()
{
    player->write("volume -5\n");
}

void SetupMusic::on_stop_clicked()
{
    isButton = true;
    player->write("stop\n");
}

void SetupMusic::on_setVolume_clicked()
{
    volume = ui->lineEdit->text().toInt();
    player->write("volume " + QByteArray::number(volume) + " 1\n");
}
