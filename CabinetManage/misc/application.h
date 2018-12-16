#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QTimer>
#include <QObject>
#include "config.h"
#include "arm4418.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);

private:
    bool notify(QObject *, QEvent *);

    Config *config;
    Arm4418 *arm4418;
    QTimer closeLcdTimer;

private slots:
    void openLcd();
    void closeLcd();
};

#endif // APPLICATION_H
