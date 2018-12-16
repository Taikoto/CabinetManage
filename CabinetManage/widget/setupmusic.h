#ifndef SETUPMUSIC_H
#define SETUPMUSIC_H

#include <QWidget>
#include "mediaplayer.h"

namespace Ui {
class SetupMusic;
}

class SetupMusic : public QWidget
{
    Q_OBJECT

public:
    explicit SetupMusic(QWidget *parent = 0);
    ~SetupMusic();

public slots:
    void init();

private:
    Ui::SetupMusic *ui;
    MediaPlayer *player;
    QString path;
    int cIndex;
    int volume;//音量
    bool isButton;

private slots:
    void itemClicked(int index);
    void readOutput();
    void playFinished(int exitCode);
    void on_addVolume_clicked();
    void on_subVolume_clicked();
    void on_stop_clicked();
    void on_setVolume_clicked();
};

#endif // SETUPMUSIC_H
