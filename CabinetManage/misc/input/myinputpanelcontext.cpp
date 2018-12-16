#include <QtCore>

#include "myinputpanelcontext.h"



MyInputPanelContext::MyInputPanelContext()
{
    inputPanel = new MyInputPanel;

    //���ܵļ��̵����룬�ͽ������ϵ��ַ����͵������
    connect(inputPanel, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));

    connect(inputPanel, SIGNAL(delter()), SLOT(backspace()));   //ɾ����
    connect(inputPanel, SIGNAL(up_move()), SLOT(key_up()));     //�������
    connect(inputPanel, SIGNAL(down_move()), SLOT(key_down())); //�������
    connect(inputPanel, SIGNAL(left_move()), SLOT(key_left())); //�������
    connect(inputPanel, SIGNAL(right_move()), SLOT(key_right()));//�������
}

MyInputPanelContext::~MyInputPanelContext()
{
    delete inputPanel;
}

/*���ƣ����뷨�¼�������
*���ܣ������������ʱ������һ���¼����ú����ܹ���׽����¼���Ȼ�󼤻����뷨
*
*/
bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel) {
        //updatePosition();                   //�������뷨��λ��
        inputPanel->show();
        //inputPanel->move(100,300);          //�ƶ����뷨����
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel) {
        //inputPanel->hide();
        return true;
    }
    return false;
}


//�������뷨����
QString MyInputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void MyInputPanelContext::reset()
{
}

//��֤���뷨�Ƿ�ɹ�����
bool MyInputPanelContext::isComposing() const
{
    return false;
}

//���뷨����
QString MyInputPanelContext::language()
{
    return "en_US";
}


/*
*���ƣ������ַ��������
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
*���ƣ��������뷨���̵�λ��
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


/*//����һ�������¼���ɾ���ַ�
*
*/
void MyInputPanelContext::backspace()
{
    QPointer<QWidget> w = focusWidget();    //��⵱ǰ�����ǲ��ǽ��㴰��

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

//����һ�������¼��� ʹ�������
void MyInputPanelContext::key_up()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}


//����һ�������¼��� ʹ�������
void MyInputPanelContext::key_down()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

//����һ�������¼��� ʹ�������
void MyInputPanelContext::key_left()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

//����һ�������¼��� ʹ�������
void MyInputPanelContext::key_right()
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    QApplication::sendEvent(w, &keyPress);
}

