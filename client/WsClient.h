#pragma once
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Tool_Websocket.h"

class WsClient : public QObject {
    Q_OBJECT
public:
    explicit WsClient(QObject* parent = nullptr);

    bool isConnected();

    Q_INVOKABLE void setUsername(QString username);
    Q_INVOKABLE QString getUsername();
    //Q_INVOKABLE void setMyId(int peerid);
    //Q_INVOKABLE int getMyId();

    Q_INVOKABLE void startLogin(QString serverIp, int port);
    Q_INVOKABLE void signOut();
    Q_INVOKABLE QJsonArray getPeerList();

    void sendToPeer(const QString& message);

public Q_SLOTS:
    void sendLoginText();
    void onTextMessageReceived(const QString& message);

Q_SIGNALS:
    void sendId(int myid);
    void login();
    void peerConnectedOrDisconnected();
    void loginSuccess();
    void messageFromPeer(QJsonObject jsonObj);

private:
    QString m_username;
    int m_myid;
    QMap<int, QString> m_peerlist;

    Tool_Websocket* m_webSocket;
};
