#pragma once

#include <QObject>
#include <QSslError>
#include <QWebSocket>

class Tool_Websocket : public QObject
{
	Q_OBJECT
public:
    explicit Tool_Websocket(QObject* parent = nullptr);
    ~Tool_Websocket();
    void connectToServer(const QUrl url);
    void sendTextToServer(const QString message);
    void sendBinaryToServer(const QByteArray message);

Q_SIGNALS:
    void error(const QString errorString, const QString detailString = QString());
    void connected();

    void newTextMessageReceived(QString message);
    void newBinaryMessageReceived(QByteArray message);
private:
    void init();

    void onConnected();
    void onSslErrors(const QList<QSslError>& errors);

    QWebSocket* m_webSocket;
    QString m_url = "ws://127.0.0.1:4567";
};

//![111]