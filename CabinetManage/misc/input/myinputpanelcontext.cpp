#include <QtCore>

#include "myinputpanelcontext.h"



MyInputPanelContext::MyInputPanelContext()
{
    inputPanel = new MyInputPanel;

    //接受的键盘的输入，就将键盘上的字符发送到输入框
    connect(inputPanel, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));

    connect(inputPanel, SIGNAL(delter()), SLOT(backspace()));   //删除键
    connect(inputPanel, SIGNAL(up_move()), SLOT(key_up()));     //光标上移
    connect(inputPanel, SIGNAL(down_move()), SLOT(key_down())); //光标下移
    connect(inputPanel, SIGNAL(left_move()), SLOT(key_left())); //光标左移
    connect(inputPanel, SIGNAL(right_move()), SLOT(key_right()));//光标右移
}

MyInputPanelContext::~MyInputPanelContext()
{
    delete inputPanel;
}

/*名称：输入法事件过滤器
*功能：鼠标点击输入框的时候会产生一个事件，该函数能够捕捉这个事件，然后激活输入法
*
*/
bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel) {
        //updatePosition();                   //更新输入法的位置
        inputPanel->show();
        //inputPanel->move(100,300);          //移动输入法坐标
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel) {
        //inputPanel->hide();
        return true;
    }
    return false;
}


//返回输入法名称
QString MyInputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void MyInputPanelContext::reset()
{
}

//保证输入法是否成功输入
bool MyInputPanelContext::isComposing() const
{
    return false;
}

//输入法语言
QString MyInputPanelContext::language()
{
    return "en_US";
}


/*
*名称：发送字符到输入框
*
*/
void MyInputPanelContext::sendCharacter(QChar character)
{
    QPointer<QWidget> w = focusWidget();
    if (!w) return;

    QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
}

/*
*名称：更新输入法键盘的位置
*/
void MyInputPanelContext::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
    inputPanel->move(panelPos);
}


/*//发送一个按键事件，删除字符
*
*/
void MyInputPanelContext::backspace()
{
    QPointer<QWidget> w = focusWidget();    //检测当前窗体是不是焦点窗体

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

//发送一个按键事件， 使光标上移
void MyInputPanelContext::key_up()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}


//发送一个按键事件， 使光标下移
void MyInputPanelContext::key_down()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

//发送一个按键事件， 使光标左移
void MyInputPanelContext::key_left()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

//发送一个按键事件， 使光标右移
void MyInputPanelContext::key_right()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

