#ifndef MSGWAPPER_H
#define MSGWAPPER_H

#include <QObject>

class MsgWapper : public QObject
{
    Q_OBJECT
public:
    static MsgWapper * instance();

signals:
    void message(QtMsgType type, const QString &msg);

private slots:
    void outLogFile(QtMsgType type, const QString &msg);

private:
    MsgWapper();
    static MsgWapper * m_instance;
};

#endif // MSGWAPPER_H
