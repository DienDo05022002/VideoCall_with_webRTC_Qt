#pragma once

//#undef emit
#include "api/peer_connection_interface.h"
//#define emit
#include "WebrtcVideoSinkInterface.h"
#include "api/scoped_refptr.h"
#include <QJsonObject>
#include <QObject>

class WebrtcPeerConnectionObserver : public QObject,
    public webrtc::PeerConnectionObserver
{
    Q_OBJECT
public:
    explicit WebrtcPeerConnectionObserver(
        bool& loopback,
        rtc::scoped_refptr<webrtc::PeerConnectionInterface>& peer_connection,
        QObject* parent = nullptr);

Q_SIGNALS:
    //    void error(const QString errorString, const QString detailString = QString());
    void addTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver);
    //Signaling, sending a message to the peer
    void sendMessageToPeer(const QString message);
protected:

    // ! [1] ::: PeerConnectionObserver implementation.
    void OnSignalingChange(
        webrtc::PeerConnectionInterface::SignalingState new_state) override {}

    void OnDataChannel(
        rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}

    void OnIceGatheringChange(
        webrtc::PeerConnectionInterface::IceGatheringState new_state) override {}

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;
    void OnAddTrack(
        rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
        const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
        streams) override;
    void OnRemoveTrack(
        rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override;

    // ! [1] ::: End

    // ! [2] webrtc::PeerConnectionObserver is an abstract class and needs to implement all of its remaining pure virtual methods.
    //  ->Located inside "#include "api / peer_connection_interface.h""
    void OnRenegotiationNeeded() override {}
    void OnIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}

    // ! [2] ::: End
private:
    bool& m_loopback;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface>& m_peer_connection;
};
