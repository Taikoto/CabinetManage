#ifndef SETUPRFID_H
#define SETUPRFID_H

#include <QWidget>
#include "serialrfid.h"

namespace Ui {
class SetupRfid;
}

class SetupRfid : public QWidget
{
    Q_OBJECT

public:
    explicit SetupRfid(QWidget *parent = 0);
    ~SetupRfid();

private slots:
    void oMessage(QtMsgType type, const QString &msg);

    void finished(const SerialRfid::DevData data);

    void on_setPort_clicked();

    void on_getCardId_clicked();

    void on_TestPort_clicked();

    void on_setBaud_clicked();

    void on_setDevBaud_clicked();

    void on_getCardIdOnly_clicked();

private:
    Ui::SetupRfid *ui;
    SerialRfid *rfid;
};

#endif // SETUPRFID_H
