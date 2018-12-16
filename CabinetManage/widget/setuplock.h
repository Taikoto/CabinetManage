#ifndef SETUPLOCK_H
#define SETUPLOCK_H

#include <QWidget>
#include "seriallock.h"

namespace Ui {
class SetupLock;
}

class SetupLock : public QWidget
{
    Q_OBJECT

public:
    explicit SetupLock(QWidget *parent = 0);
    ~SetupLock();

private slots:
    void oMessage(QtMsgType type, const QString &msg);

    void finished(const SerialLock::DevData data);

    void on_Reset_clicked();

    void on_TestPort_clicked();

    void on_Query_clicked();

    void on_OpenLock_clicked();

    void on_SetOpenTime_clicked();

    void on_Warning_clicked();

    void on_EXQuery_clicked();

    void on_EXOpenLock_clicked();

    void on_EXLed_clicked();

    void on_EXLedTwinkle_clicked();

    void on_EXLedState_clicked();

    void on_ReadWeight_clicked();

    void on_allLock_clicked();

    void on_setPort_clicked();

    void on_setScale_clicked();

private:
    Ui::SetupLock *ui;
    SerialLock *lock;
};

#endif // SETUPLOCK_H
