#include "server.h"

#include "datamodel.h"
#include "dialog/verifydialog.h"

#include <QDateTime>
#include <QStringList>
#include <QTextCodec>
#include <QRegExp>
#include <QDebug>

using namespace DataModel;

Server* Server::m_instance = NULL;

Server* Server::instance(QObject *parent)
{
    if(m_instance == NULL) m_instance = new Server(parent);
    return m_instance;
}

Server::Server(QObject *parent)
    : QObject(parent)
{
    connect(&server,SIGNAL(newRequest(HttpRequest*,HttpResponse*)),
            this,SLOT(onRequest(HttpRequest*,HttpResponse*)));
    config = Config::instance();
}

bool Server::listen(const QHostAddress &address, quint16 port)
{
    return server.listen(address, port);
}

bool Server::listen(quint16 port)
{
    return server.listen(port);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

//新请求
void Server::onRequest(HttpRequest* req, HttpResponse* resp)
{
    QString path = req->path();
    QByteArray body;
    QString value = path.split("/").last();//取出path中最后一个参数(/为分割)
    QString notValue = path;
    notValue.replace(value, "");//除去path中的最后一个参数
    qDebug() << tr("New request:") << req->method() << path;//显示完整路径

    if(path == config->apiGetTime) body = apiTime();
    else if(path == config->apiGetQPoliceId) body = apiGetPoliceId();
    else body = apiMessage();

    //本地数据库编码是UTF-8，故不用转换
    //qDebug() << QString::fromUtf8(body);

    resp->setHeader("Content-Length", QString::number(body.size()));
    resp->setHeader("Content-Type", "application/json; charset=UTF-8");
    resp->setStatus(200);
    resp->end(body);//end会断开连接
}




//  api


QByteArray Server::apiTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    DataModel::Time time;
    time.Success = true;
    time.Body = dateTime.toMSecsSinceEpoch();
    return DataModel::serialize(time);
}

QByteArray Server::apiMessage()
{
    DataModel::Message msg;
    msg.Success = false;
    msg.Body = "No data.";
    return DataModel::serialize(msg);
}

//获取警员ID
QByteArray Server::apiGetPoliceId()
{
    Police police = VerifyDialog::getPoliceData(NULL, VerifyDialog::VPolice);

    QByteArray body;
    if( police.Id.isEmpty() ){
        DataModel::Message msg;
        msg.Success = true;
        msg.Body = tr("Get police data failed!");
        body = serialize(msg);
    } else {
        body = addShell(serialize(police));
    }

    return body;
}
