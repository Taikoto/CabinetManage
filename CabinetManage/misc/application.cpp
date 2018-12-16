#include "application.h"

#include <QDebug>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    //实例化配置类
    config = Config::instance(this);
    config->readConfig();

    if( config->isArm4418 ){
        //实例化ARM4418类
        arm4418 = Arm4418::instance(this);

        connect(&closeLcdTimer, SIGNAL(timeout()), this, SLOT(closeLcd()));
        if( config->closeLcd > 0 ) closeLcdTimer.start(config->closeLcd);
    }
}

bool Application::notify(QObject *obj, QEvent *e)
{
    switch (e->type()) {
    case QEvent::MouseButtonPress: openLcd(); break;
    default:
        break;
    }
    return QApplication::notify(obj,e);
}


//打开LCD背光
void Application::openLcd()
{
    if( !config->isArm4418 ) return ;

    arm4418->setBackight(config->lcdBackight);
    closeLcdTimer.stop();
    if( config->closeLcd > 0 ) closeLcdTimer.start(config->closeLcd);
}

//关闭LCD背光
void Application::closeLcd()
{
    closeLcdTimer.stop();
    arm4418->setBackight(255);
}
