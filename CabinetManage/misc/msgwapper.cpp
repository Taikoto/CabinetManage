#include "msgwapper.h"

#include <QMetaType>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "common.h"

void static msgHandlerFunction(QtMsgType type, const char *msg)
{
    QMetaObject::invokeMethod(MsgWapper::instance(), "message"
                              , Q_ARG(QtMsgType, type)
                              , Q_ARG(QString, QString::fromLatin1(msg)));
}

MsgWapper * MsgWapper::m_instance = 0;

MsgWapper * MsgWapper::instance()
{
    if(m_instance == NULL) m_instance = new MsgWapper;
    return m_instance;
}

MsgWapper::MsgWapper() :
    QObject(qApp)
{
#if (QT_VERSION == QT_VERSION_CHECK(4, 8, 6))
    qRegisterMetaType<QtMsgType>("QtMsgType");
    connect(this, SIGNAL(message(QtMsgType,QString)), this, SLOT(outLogFile(QtMsgType,QString)));

    qInstallMsgHandler(msgHandlerFunction);   //Qt4.8.6

    QFile file(DEBUG_LOG_PATH);
    file.remove(); // 删除需要时间，不能在删除的同时debug
#endif
}

void MsgWapper::outLogFile(QtMsgType type, const QString &msg)
{
    Q_UNUSED(type);
    QFile file(DEBUG_LOG_PATH);

    if (file.size() > 10485760) file.remove(); // 10M

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
        qWarning() << "Open debug.log file failed!";
        return ;
    }

    file.write(msg.toUtf8() + "\n");
    file.close();
}
