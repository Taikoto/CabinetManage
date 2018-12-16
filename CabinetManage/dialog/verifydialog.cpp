#include "verifydialog.h"
#include "ui_verifydialog.h"

#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QDebug>

#include "verify_p.h"

using namespace DataModel;


VerifyDialog::VerifyDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::VerifyDialog)
{
    ui->setupUi(this);

    taskMode = NULL;
    policeMode = NULL;
    managerMode = NULL;
    dutyManagerMode = NULL;
    leaderMode = NULL;
    m_EnrollFinger = NULL;

    //关闭窗口定时器
    connect(&rejectTimer, SIGNAL(timeout()), &rejectTimer, SLOT(stop()));
    connect(&rejectTimer, SIGNAL(timeout()), this, SLOT(reject()));
}

VerifyDialog::~VerifyDialog()
{
    delete ui;
}




/**
 ********************************************************************
 * public:
 ********************************************************************
*/

int VerifyDialog::start(VerifyType type)
{
    //ui->photo->setIcon(QIcon(":/img/fingerprint/fingerprint.png"));

    switch (type) {
    case VTask:
        taskMode = new TaskVerify(this);
        connect(taskMode, SIGNAL(message(QString)), ui->label, SLOT(setText(QString)));
        connect(taskMode, SIGNAL(finished(bool,DataModel::Police)),
                this, SLOT(finished(bool,DataModel::Police)));
        taskMode->start();
        break;
    case VPolice:
        policeMode = new PoliceVerify(this);
        connect(policeMode, SIGNAL(message(QString)), ui->label, SLOT(setText(QString)));
        connect(policeMode, SIGNAL(finished(bool,DataModel::Police)),
                this, SLOT(finished(bool,DataModel::Police)));
        policeMode->start();
        break;
    case VManager:
        managerMode = new ManagerVerify(this);
        connect(managerMode, SIGNAL(message(QString)), ui->label, SLOT(setText(QString)));
        connect(managerMode, SIGNAL(finished(bool,DataModel::Police)),
                this, SLOT(finished(bool,DataModel::Police)));
        managerMode->start();
        break;
    case VDutyManager:
        dutyManagerMode = new DutyManagerVerify(this);
        connect(dutyManagerMode, SIGNAL(message(QString)), ui->label, SLOT(setText(QString)));
        connect(dutyManagerMode, SIGNAL(finished(bool,DataModel::Police)),
                this, SLOT(finished(bool,DataModel::Police)));
        dutyManagerMode->start();
        break;
    case VLeader:
        leaderMode = new LeaderVerify(this);
        connect(leaderMode, SIGNAL(message(QString)), ui->label, SLOT(setText(QString)));
        connect(leaderMode, SIGNAL(finished(bool,DataModel::Police)),
                this, SLOT(finished(bool,DataModel::Police)));
        leaderMode->start();
        break;
    case EFinger:
        m_EnrollFinger = new EnrollFinger(this);
        connect(m_EnrollFinger, SIGNAL(message(QString)), ui->label, SLOT(setText(QString)));
        connect(m_EnrollFinger, SIGNAL(finished(bool,DataModel::Police)),
                this, SLOT(finished(bool,DataModel::Police)));
        m_EnrollFinger->start();
        break;
    default: break;
    }
    return this->exec();
}

int VerifyDialog::verifyId(QWidget *parent, VerifyType type)
{
    VerifyDialog dialog(parent);
    return dialog.start(type);
}

Police VerifyDialog::getPoliceData(QWidget *parent, VerifyType type)
{
    VerifyDialog dialog(parent);
    dialog.start(type);
    return dialog.police;
}

int VerifyDialog::enrollFinger(QWidget *parent)
{
    VerifyDialog dialog(parent);
    return dialog.start(EFinger);
}



/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void VerifyDialog::finished(bool success, Police police)
{
    if(success){
        this->police = police;
        this->accept();
    }
    else rejectTimer.start(2000);
}
