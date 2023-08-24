#include "Tool_Websocket.h"


Tool_Websocket::Tool_Websocket(QObject* parent)
{
    init();
}

Tool_Websocket::~Tool_Websocket()
{
}

void Tool_Websocket::connectToServer(const QUrl url)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__ << url;
#endif
    m_webSocket->open(url);
}

void Tool_Websocket::sendTextToServer(const QString message)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__ << message;
#endif
    int bytes = m_webSocket->sendTextMessage(message);
    //    m_webSocket->flush();
    qDebug() << "Tool_Websocket(websocketclient) sended byte counts:" << bytes;
}

void Tool_Websocket::sendBinaryToServer(const QByteArray message)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__ << message;
#endif
    m_webSocket->sendBinaryMessage(message);
}

void Tool_Websocket::init()
{
    m_webSocket = new QWebSocket();
    connect(m_webSocket, &QWebSocket::connected, this, &Tool_Websocket::onConnected);
    connect(m_webSocket, &QWebSocket::connected, this, &Tool_Websocket::connected);
    connect(m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
        this, &Tool_Websocket::onSslErrors);
}

void Tool_Websocket::onConnected()
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__;
#endif
    qDebug() << "WebSocket connected";

    connect(m_webSocket, &QWebSocket::textMessageReceived,
        this, &Tool_Websocket::newTextMessageReceived, Qt::QueuedConnection);
    connect(m_webSocket, &QWebSocket::binaryMessageReceived,
        this, &Tool_Websocket::newBinaryMessageReceived, Qt::QueuedConnection);
}

void Tool_Websocket::onSslErrors(const QList<QSslError>& errors)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__;
#endif
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_webSocket->ignoreSslErrors();
}
