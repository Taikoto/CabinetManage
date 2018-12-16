#ifndef MYINPUTPANELCONTEXT_H
#define MYINPUTPANELCONTEXT_H

#include <QInputContext>
#include <QDebug>

#include "myinputpanel.h"


class MyInputPanel;

//! [0]

class MyInputPanelContext:public QInputContext
{
    Q_OBJECT


public:
    MyInputPanelContext();
    ~MyInputPanelContext();

    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;

    void reset();

signals:
    void setImObjName(QString);

private slots:
    void sendCharacter(QChar character);
    void backspace();
    void key_up();
    void key_down();
    void key_left();
    void key_right();

private:
    MyInputPanel *inputPanel;
     void updatePosition();
};

//! [0]

#endif // MYINPUTPANELCONTEXT_H
