#pragma once

#include <QObject>
#include <QWebSocket>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString username, int peerid, QWebSocket* client, QObject* parent = nullptr);
    void setSocketClient(QWebSocket* client);
    QWebSocket* getSocketClient();
    void setPeerId(int peerid);
    int getPeerId();
    QString getUsername();
signals:


private:
    QString m_username;
    int m_peerid;
    QWebSocket* m_websocketClient;
};

