#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QProcess>

class MediaPlayer : public QProcess
{
    Q_OBJECT
public:
    static MediaPlayer* instance(QObject *parent = 0);
private:
    static MediaPlayer* m_instance;

public:
    explicit MediaPlayer(QObject *parent = 0);
    ~MediaPlayer();

    enum MusicType{
        None = 0
        , VPolice           //请警员验证身份
        , VManger           //请管理员验证身份
        , VDutyManager      //请值班管理员验证身份
        , VLeader           //请领导验证身份
        , WaitVPolice       //等待警员验证指纹
        , OtherVManager     //请另一位值班管理员验证身份
        , ReVerify          //请重新验证
        , VerifyFailed      //验证失败
        , Warn              //报警
        , NoOutTask         //无出警任务
        , NoTempTask        //无临时存放任务
        , NoRepairTask      //无保养任务
        , NoThrowTask       //无枪弹报废任务
        , NoInTask          //无入库任务
        , NotManager        //您不是管理员
        , NotDutyManager    //您不是值班管理员
        , NotLeader         //您不是领导
        , NotPolice         //您不是普通警员
        , PressFinger       //请按指纹
    };

public slots:
    void play(QString path);
    void play(MusicType type);
};

#endif // MEDIAPLAYER_H
