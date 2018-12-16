#ifndef INFODIALOG_H
#define INFODIALOG_H

#include "dialog.h"
#include <QMessageBox>

namespace Ui {
class InfoDialog;
}

class InfoDialog : public Dialog
{
    Q_OBJECT

public:
    explicit InfoDialog(QWidget *parent = 0);
    ~InfoDialog();

    static InfoDialog* instance(QWidget *parent = 0);

    void setTitle(QString text);
    void setText(QString text);
    void setOkHide();
    void setOkShow();
    void setCancelHide();
    void setCancelShow();

    static int message(QWidget *parent, QString title, QString text,
                       QMessageBox::StandardButton button = QMessageBox::Ok);
    static int information(QWidget *parent, QString text, QMessageBox::StandardButton button = QMessageBox::Ok);
    static int error(QWidget *parent, QString text, QMessageBox::StandardButton button = QMessageBox::Ok);

    static void mAccept(){ if(m_dialog != NULL) m_dialog->accept(); }
    static void mReject(){ if(m_dialog != NULL) m_dialog->reject(); }

private:
    Ui::InfoDialog *ui;
    static InfoDialog *m_dialog;
};

#endif // INFODIALOG_H
