#include "server.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
Server::Server(QObject* parent)
    : QObject{ parent }
{
    m_server = new QWebSocketServer("signaling server", QWebSocketServer::NonSecureMode, this);
    m_count = 0;
    connect(m_server, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
    if (m_server->listen(QHostAddress::Any, 4567)) {
        qDebug() << "Server:: Start successfully!!! listening on port 4567.....";
    }
    else {
        qDebug() << "Server start fail!";
    }
}

void Server::onNewConnection()
{
    qDebug() << "Server:: new Connection!";
    QWebSocket* client = m_server->nextPendingConnection();
    connect(client, &QWebSocket::textMessageReceived, this, &Server::onTextMessageReceived);
    connect(client, &QWebSocket::disconnected, this, &Server::onDisconnection);
}

void Server::onTextMessageReceived(const QString& message)
{
    qDebug() << "Server:: text received from client:" << message;
    QWebSocket* sendClient = qobject_cast<QWebSocket*>(sender());
    if (sendClient) {
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8(), &error);
        if (error.error || !jsonDocument.isObject()) {
            qDebug() << "Server:: Receive a message which can not be converted to JSON format";
            return;
        }
        QJsonObject jsonObj = jsonDocument.object();
        QString type = jsonObj.value("type").toString();
        if (type == "signin") {
            qDebug() << "Server:: signin";
            QString username = jsonObj.value("username").toString();
            Client* client = new Client(username, m_count, sendClient);

            qDebug() << "Server:: User" << username << "signin. Server assign an id:" << m_count << "for him(her)";
            QJsonObject responseObj;

            responseObj.insert("type", "loginresponse");
            responseObj.insert("peerid", m_count);
            responseObj.insert("msg", "login success!");
            QJsonDocument doc3(responseObj);
            sendClient->sendTextMessage(doc3.toJson(QJsonDocument::Compact));
            QJsonObject infoObj1;
            infoObj1.insert("type", "client");
            infoObj1.insert("username", client->getUsername());
            infoObj1.insert("peerid", client->getPeerId());
            for (auto it = m_clients.constBegin(); it != m_clients.constEnd(); ++it)
            {
                QJsonObject infoObj2;
                infoObj2.insert("type", "client");
                infoObj2.insert("username", it.value()->getUsername());
                infoObj2.insert("peerid", it.value()->getPeerId());
                QJsonDocument doc(infoObj1);
                QJsonDocument doc2(infoObj2);
                it.value()->getSocketClient()->sendTextMessage(doc.toJson(QJsonDocument::Compact));
                client->getSocketClient()->sendTextMessage(doc2.toJson(QJsonDocument::Compact));
            }
            m_clients[m_count] = client;
            m_count++;
        }if (type == "signout") {
            qDebug() << "signout";
            int peerid = jsonObj.value("peerid").toInt();
            m_clients.remove(peerid);
            sendClient->close();
            foreach(Client * theclient, m_clients) {
                QJsonObject infoObj;
                infoObj.insert("type", "out");
                infoObj.insert("peerid", peerid);
                QJsonDocument doc(infoObj);
                theclient->getSocketClient()->sendTextMessage(doc.toJson(QJsonDocument::Compact));
            }
        }if (type == "message") {
            // ~~~           for(auto it=m_clients.constBegin();it!=m_clients.constEnd();++it)
            //            {
            //                qDebug()<<"number "<<it.key()<<" client:"<<it.value()->getUsername();
            //            }

            qDebug() << "message";
            int to_peerid = jsonObj.value("to").toInt();
            //  ~~~          qDebug()<<"ttttttttttttttoId"<<to_peerid;
            Client* client = m_clients[to_peerid];
            QJsonObject messagejsonObj = jsonObj.value("message").toObject();
            QJsonObject sendObj;
            sendObj.insert("type", "message");
            sendObj.insert("from", jsonObj.value("from").toInt());
            sendObj.insert("message", messagejsonObj);

            QJsonDocument doc(sendObj);
            client->getSocketClient()->sendTextMessage(doc.toJson(QJsonDocument::Compact));
            //            QString message=jsonObj.value("message").toString();
            qDebug() << message;
            //             client->getSocketClient()->sendTextMessage(message);
        }
    }
}

void Server::onDisconnection()
{
    QWebSocket* websocketClient = qobject_cast<QWebSocket*>(sender());
    if (websocketClient) {
        int i = -1;
        for (auto it = m_clients.constBegin(); it != m_clients.constEnd(); ++it)
        {
            if (it.value()->getSocketClient() == websocketClient)
            {
                i = it.key();
                break;
            }
        }
        if (i != -1)
        {
            qDebug() << "Server:: Client disconnected:" << "username:" << m_clients.value(i)->getUsername() << "peerid:" << m_clients.value(i)->getPeerId();
            m_clients.remove(i);

            foreach(Client * theclient, m_clients) {
                QJsonObject infoObj;
                infoObj.insert("type", "out");
                infoObj.insert("peerid", i);
                QJsonDocument doc(infoObj);
                theclient->getSocketClient()->sendTextMessage(doc.toJson(QJsonDocument::Compact));
            }
        }
        websocketClient->deleteLater();
    }
}
