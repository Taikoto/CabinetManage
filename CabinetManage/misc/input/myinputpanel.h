#ifndef MYINPUTPANEL_H
#define MYINPUTPANEL_H

#include <QtGui>
#include <QtCore>

#include "ui_myinputpanelform.h"

//! [0]

class MyInputPanel : public QWidget
{
    Q_OBJECT
friend class MyInputPanelContext;


public:
    MyInputPanel();
    ~MyInputPanel();
    void setMapper();
    void connectMapper();

signals:
    void characterGenerated(QChar character);
    void delter();
    void up_move();
    void down_move();
    void left_move();
    void right_move();

protected:
    bool event(QEvent *e);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QWidget *w);

    void on_toolButton_backspace_clicked();

    void on_toolButton_left_clicked();

    void on_toolButton_right_clicked();

    void on_toolButton_up_clicked();

    void on_toolButton_down_clicked();

    void onHorizontalSliderValueChanged(int value);

    void on_CapsLock_clicked();

private:
    Ui::MyInputPanelForm form;
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;
    bool capsLock;

    bool shouldMove;//看是否应该改变窗口的位置
    QPoint formerMousePos;//离窗体左上角的距离
};

//! [0]

#endif // MYINPUTPANEL_H
