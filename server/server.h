#pragma once

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include "client.h"
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);

signals:

public slots:
    void onNewConnection();
    void onTextMessageReceived(const QString& message);
    void onDisconnection();
private:
    QWebSocketServer* m_server;
    QMap<int, Client*> m_clients;
    int m_count;
};

