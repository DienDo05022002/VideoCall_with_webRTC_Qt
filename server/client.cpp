#include "client.h"

Client::Client(QString username, int peerid, QWebSocket* client, QObject* parent)
    : QObject{ parent }, m_username(username), m_peerid(peerid), m_websocketClient(client)
{
    //    m_websocketClient=new QWebSocket();
}

void Client::setSocketClient(QWebSocket* client)
{
    m_websocketClient = client;
}

QWebSocket* Client::getSocketClient()
{
    return m_websocketClient;
}

void Client::setPeerId(int peerid)
{
    m_peerid = peerid;
}

int Client::getPeerId()
{
    return m_peerid;
}

QString Client::getUsername()
{
    return m_username;
}