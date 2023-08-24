#pragma once

#undef emit
#include "api/peer_connection_interface.h"
#define emit

#include "api/jsep.h"

#include <QJsonObject>
#include <QObject>

class WebrtcCreateSessionDescriptionObserver : public QObject,
    public webrtc::CreateSessionDescriptionObserver
{
    Q_OBJECT
public:
    explicit WebrtcCreateSessionDescriptionObserver(
        bool& loopback,
        rtc::scoped_refptr<webrtc::PeerConnectionInterface>& peer_connection,
        QObject* parent = nullptr);

Q_SIGNALS:
    //    void error(const QString errorString, const QString detailString = QString());

        //Signaling, sending a message to the peer
    void sendMessageToPeer(const QString& message);
protected:
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
    void OnFailure(webrtc::RTCError error) override;

    void AddRef() const override {}
    rtc::RefCountReleaseStatus Release() const override {
        return rtc::RefCountReleaseStatus::kDroppedLastRef;
    }

private:
    bool& m_loopback;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface>& m_peer_connection;
};
