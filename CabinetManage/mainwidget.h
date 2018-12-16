#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QQueue>

#include "widget/home.h"
#include "widget/setup.h"
#include "config.h"
#include "network.h"
#include "dataloop.h"
#include "server.h"
#include "database.h"
#include "seriallock.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

    typedef struct {
        QWidget* cWidget; // current show widget
    }DataCore;

    DataCore m_data;

    //! 界面类的声明
    Home *home;
    Setup *setup;

    //! 操作类的声明
    Config *config;
    Network *network;
    DataLoop *dataLoop;
    Server *server;
    Database *database;
    SerialLock *lock;
    Arm4418 *arm4418;

    QTimer updateTime;// update time timer
    QTimer updateData;// update duty manager and duty leader timer
    QTimer updateTemper;// update temprature timer
    QTimer updateMessage;// update message timer

    void paintEvent(QPaintEvent *);

private slots:
    void showMessage(QString text, int ms);
    void updateTimeSlot();
    void updateDataSlot();
    void updateTemperSlot();
    void updateMessageSlot();
    void setCurrentWidget(QWidget* widget, bool isNew);
    void finished(const Network::DataCore &data);
    void finished(const SerialLock::DevData data);
    void on_backButton_clicked();
    void on_setupButton_clicked();
    void on_alcohol_clicked();
};

#endif // MAINWIDGET_H
