#ifndef VERIFYDIALOG_H
#define VERIFYDIALOG_H

#include <QTimer>

#include "datamodel.h"
#include "dialog.h"

namespace Ui {
class VerifyDialog;
}

class TaskVerify;
class PoliceVerify;
class ManagerVerify;
class DutyManagerVerify;
class LeaderVerify;
class EnrollFinger;

class VerifyDialog : public Dialog
{
    Q_OBJECT

public:
    explicit VerifyDialog(QWidget *parent = 0);
    ~VerifyDialog();

    //验证类型
    enum VerifyType {
        None
        , VPolice       //验证普通警员
        , VManager      //验证管理员
        , VDutyManager  //验证值班枪管员
        , VLeader       //验证领导
        , VTask         //任务验证流程
        , EFinger       //录入指纹
    };

    int start(VerifyType type);

    //验证身份
    static int verifyId(QWidget *parent = 0, VerifyType type = None);

    //验证身份，并返回验证所得的警员数据
    static DataModel::Police getPoliceData(QWidget *parent = 0, VerifyType type = None);

    //录指纹
    static int enrollFinger(QWidget *parent = 0);

private:
    Ui::VerifyDialog *ui;

    TaskVerify *taskMode;
    PoliceVerify *policeMode;
    ManagerVerify *managerMode;
    DutyManagerVerify *dutyManagerMode;
    LeaderVerify *leaderMode;
    EnrollFinger *m_EnrollFinger;

    QTimer rejectTimer;//关闭对话框定时器

    DataModel::Police police;

private slots:
    void finished(bool success, DataModel::Police police);
};

#endif // VERIFYDIALOG_H
