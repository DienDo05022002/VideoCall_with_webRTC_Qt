#include "WsClient.h"
#include <QDebug.h>
#include <QJsonDocument>

WsClient::WsClient(QObject* parent)
{
    m_webSocket = new Tool_Websocket();
    m_myid = -1;
    connect(m_webSocket, &Tool_Websocket::newTextMessageReceived, this, &WsClient::onTextMessageReceived);
}

bool WsClient::isConnected()
{
    return m_myid != -1;
}

void WsClient::setUsername(QString username)
{
    m_username = username;
}
QString WsClient::getUsername()
{
    qDebug() << "WsClient::getUsername::: " + m_username;
    return m_username;
}

void WsClient::startLogin(QString serverIp, int port)
{
    qDebug() << "serverIp: " << serverIp << "||" << " PORT:" << port;
    QUrl url(QString("ws://").append(serverIp).append(QString(":")).append(QString::number(port)));
    //    QUrl url("ws://127.0.0.1:4567");
    m_webSocket->connectToServer(url);
    connect(m_webSocket, &Tool_Websocket::connected, this, &WsClient::sendLoginText);
}

void WsClient::signOut()
{
    m_webSocket->disconnect();
}

Q_INVOKABLE QJsonArray WsClient::getPeerList()
{
    QJsonArray arr;
    for (auto it = m_peerlist.constBegin(); it != m_peerlist.constEnd(); ++it)
    {
        QJsonObject obj;
        obj.insert("id", it.key());
        obj.insert("name", it.value());
        arr.append(obj);
    }
    return arr;
}

void WsClient::sendToPeer(const QString& message)
{
    m_webSocket->sendTextToServer(message);
}

void WsClient::sendLoginText()
{
    QJsonObject loginJsonObj;
    loginJsonObj.insert("type", "signin");
    loginJsonObj.insert("username", m_username);
    QJsonDocument doc(loginJsonObj);
    qDebug() << doc;
    m_webSocket->sendTextToServer(doc.toJson(QJsonDocument::Compact));
    Q_EMIT loginSuccess();
}

void WsClient::onTextMessageReceived(const QString& message)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8(), &error);
    if (error.error || !jsonDocument.isObject()) {
        qDebug() << "Receive a message which can not be converted to JSON format";
        return;
    }
    QJsonObject jsonObj = jsonDocument.object();
    QString type = jsonObj.value("type").toString();
    if (type == "client")
    {
        QString username = jsonObj.value("username").toString();
        int peerid = jsonObj.value("peerid").toInt();
        if (!m_peerlist[peerid].isEmpty()) {
            qDebug() << "There is a new client ,but it's id duplicate with an id in the list.Didn't not process it.";
            return;
        }
        m_peerlist[peerid] = username;
        Q_EMIT peerConnectedOrDisconnected();
    }
    else if (type == "out") {
        int peerid = jsonObj.value("peerid").toInt();
        if (m_peerlist[peerid].isEmpty()) {
            qDebug() << "There is a  client sign out,but its id isn't in the list.Didn't not process it.";
            return;
        }
        m_peerlist.remove(peerid);
        Q_EMIT peerConnectedOrDisconnected();
    }
    else if (type == "loginresponse") {
        int myid = jsonObj.value("peerid").toInt();
        m_myid = myid;
        Q_EMIT sendId(m_myid);
    }
    else if (type == "message") {
        Q_EMIT messageFromPeer(jsonObj);
    }
}
