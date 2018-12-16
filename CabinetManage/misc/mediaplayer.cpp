#include "mediaplayer.h"

#include <QApplication>
#include <QStringList>
#include <QDebug>

MediaPlayer* MediaPlayer::m_instance = NULL;

MediaPlayer* MediaPlayer::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new MediaPlayer;
    m_instance->setParent(parent);
    return m_instance;
}

MediaPlayer::MediaPlayer(QObject *parent) :
    QProcess(parent)
{

}

MediaPlayer::~MediaPlayer()
{
    write("stop\n");
}





void MediaPlayer::play(QString path)
{
    QStringList args;

    args << "-slave" << "-quiet" << path;

    start("mplayer", args);
}

void MediaPlayer::play(MusicType type)
{
    QString root = QApplication::applicationDirPath();
    switch (type) {
    case VPolice:           play(root + "/music/VPolice.mp3"); break;
    case VManger:           play(root + "/music/VManger.mp3"); break;
    case VDutyManager:      play(root + "/music/VDutyManager.mp3"); break;
    case VLeader:           play(root + "/music/VLeader.mp3"); break;
    case WaitVPolice:       play(root + "/music/WaitVPolice.mp3"); break;
    case OtherVManager:     play(root + "/music/OtherVManager.mp3"); break;
    case ReVerify:          play(root + "/music/ReVerify.mp3"); break;
    case VerifyFailed:      play(root + "/music/VerifyFailed.mp3"); break;
    case Warn:              play(root + "/music/warn.mp3"); break;
    case NoOutTask:         play(root + "/music/NoOutTask.mp3"); break;
    case NoTempTask:        play(root + "/music/NoTempTask.mp3"); break;
    case NoRepairTask:      play(root + "/music/NoRepairTask.mp3"); break;
    case NoThrowTask:       play(root + "/music/NoThrowTask.mp3"); break;
    case NoInTask:          play(root + "/music/NoInTask.mp3"); break;
    case NotManager:        play(root + "/music/NotManager.mp3"); break;
    case NotDutyManager:    play(root + "/music/NotDutyManager.mp3"); break;
    case NotLeader:         play(root + "/music/NotLeader.mp3"); break;
    case NotPolice:         play(root + "/music/NotPolice.mp3"); break;
    case PressFinger:       play(root + "/music/PressFinger.mp3"); break;
    default: break;
    }
}
