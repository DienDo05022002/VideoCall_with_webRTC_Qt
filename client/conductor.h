#pragma once

#undef emit
#include "api/peer_connection_interface.h"
#define emit
#include "rtc_base/thread.h"
#include "api/media_stream_interface.h"

#include <QObject>
#include <QMetaType>
#include <QImage>
#include <QtMultimedia/QVideoFrame>
#include <QtMultimedia/QAbstractVideoSurface>
#include "WsClient.h"

//! [#] From Qt5.15 and below, QML can only read some types such as QString, int, QMap, QPointer,...And avoid using custom return type in QML.
//to work around this error, use Q_DECLARE_METATYPE(Type), then register the data in the constructor constructor(qRegisterMetaType<WsClient*>("WsClient*")).
class WsClient;
Q_DECLARE_METATYPE(WsClient*)
//![#] END

class WebrtcVideoSinkInterface;
class WebrtcPeerConnectionObserver;
class WebrtcCreateSessionDescriptionObserver;
class WebrtcSetSessionDescriptionObserver;

class Conductor : public QObject {
    Q_OBJECT
public:
    explicit Conductor(QObject* parent = nullptr);
    ~Conductor();

    Q_INVOKABLE void startLogin(QString ip, int port);
    Q_INVOKABLE void signOut();

    Q_INVOKABLE WsClient* getWsClient();
    Q_INVOKABLE void connectToPeer(int peerid);

    bool InitializePeerConnection();
    bool CreatePeerConnection();
    void DeletePeerConnection();
    void AddTracks();


Q_SIGNALS:
    void freshPeerList();
    void newLocalVideoFrame(const QImage image);
    void switchToStreamingUi();
public Q_SLOTS:
    void onPeerConnectedOrDisconnected();
    void setMyId(int myid);
    void sendToPeer(const QString& message);
    void onMessageFromPeer(const QJsonObject& jsonObj);
    void onAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver);

private:
    bool loopback_ = false;
    std::unique_ptr<rtc::Thread> signaling_thread_;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;

    WebrtcPeerConnectionObserver* m_webrtcPeerConnectionObserver = nullptr;
    WebrtcCreateSessionDescriptionObserver* m_webrtcCreateSessionDescriptionObserver = nullptr;

    std::unique_ptr<WebrtcVideoSinkInterface> local_renderer_;
    std::unique_ptr<WebrtcVideoSinkInterface> remote_renderer_;

    QAbstractVideoSurface* m_localVideo = nullptr;
    QAbstractVideoSurface* m_remoteVideo = nullptr;

    WsClient* m_client;
    QString m_serverIp;
    int m_port;
    int m_otherid;
    int m_myid;

};

