#ifndef SETUPRS485_H
#define SETUPRS485_H

#include <QWidget>
#include <QTimer>
#include <Device>
#include "config.h"

namespace Ui {
class SetupRs485;
}

class SetupRs485 : public QWidget
{
    Q_OBJECT

public:
    explicit SetupRs485(QWidget *parent = 0);
    ~SetupRs485();

private slots:
    void oMessage(QtMsgType type, const QString &msg);

    void finished(Arm4418::DataCore data);

    void loopSend();

    void on_state_clicked();

    void on_openLock_clicked();

    void on_changeAddr_clicked();

    void on_send458_clicked();

    void on_received_clicked();

    void on_state485_clicked();

    void on_loopSend_clicked();

    void on_addLoop_clicked();

    void on_delLoop_clicked();

    void on_cboxBaudRate_currentIndexChanged(int index);

    void on_sAddress_textEdited(const QString &arg1);

    void on_dAddress_textEdited(const QString &arg1);

    void on_openLcdButton_clicked();

    void on_closeLcdButton_clicked();

    void on_setOLTButton_clicked();

    void on_setDWButton_clicked();

    void on_setWWButton_clicked();

    void on_setUnitWButton_clicked();

    void on_readWeight_clicked();

    void on_readNumber_clicked();

    void on_allOpen_clicked();

private:
    Ui::SetupRs485 *ui;
    Config *config;
    Arm4418 *arm4418;
    QTimer timer;
};

#endif // SETUPRS485_H
