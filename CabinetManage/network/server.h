#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <Network>
#include "config.h"
#include "datamodel.h"

class Server : public QObject
{
    Q_OBJECT
public:
    static Server* instance(QObject *parent = 0);
private:
    static Server* m_instance;

public:
    explicit Server(QObject *parent = 0);
    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port=0);
    bool listen(quint16 port);

signals:
    void message(QString text, int ms);

public slots:

private slots:
    void onRequest(HttpRequest* req, HttpResponse* resp);

private:
    HttpServer server;
    Config *config;
    QByteArray apiTime();
    QByteArray apiMessage();
    QByteArray apiGetPoliceId();
};

#endif // SERVER_H
