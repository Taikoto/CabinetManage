#ifndef INPUTINTDIALOG_H
#define INPUTINTDIALOG_H

#include "dialog.h"

namespace Ui {
class InputIntDialog;
}

class InputIntDialog : public Dialog
{
    Q_OBJECT

public:
    explicit InputIntDialog(QWidget *parent = 0);
    ~InputIntDialog();

    static int getInteger(QWidget *parent = 0,
                          QString text = 0,
                          int value = 0,
                          int min = -2147483647,
                          int max = 2147483647);

    int value(){ return m_value; }

public slots:
    void setValue(int value);
    void setMaxValue(int value){ max = value; }
    void setMinValue(int value){ min = value; }
    void setText(QString text);

private slots:
    void on_subtract_clicked();

    void on_add_clicked();

    void on_ok_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::InputIntDialog *ui;
    int m_value;
    int max;
    int min;
};

#endif // INPUTINTDIALOG_H
