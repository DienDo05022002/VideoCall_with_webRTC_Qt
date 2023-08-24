#include "WebrtcPeerConnectionObserver.h"
#include "WebrtcVideoSinkInterface.h"

#include <QDebug>
#include <QJsonDocument>
#include <QImage>
//#include <QtMultimedia/QVideoFrameFormat>
#include <QtMultimedia/QVideoFrame>
WebrtcPeerConnectionObserver::WebrtcPeerConnectionObserver(
    bool& loopback,
    rtc::scoped_refptr<webrtc::PeerConnectionInterface>& peer_connection,
    QObject* parent)
    : QObject{ parent }, m_loopback(loopback), m_peer_connection(peer_connection)
{

}

void WebrtcPeerConnectionObserver::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__;
#endif
    //    RTC_LOG(LS_INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
        // For loopback test. To save some connecting delay.
    if (m_loopback) {
        if (!m_peer_connection->AddIceCandidate(candidate)) {
            qDebug() << "Failed to apply the received candidate";
            //        Q_EMIT error(tr("处理接收的candidate失败!"));
        }
        return;
    }

    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        //      RTC_LOG(LS_ERROR) << "Failed to serialize candidate";
        //      Q_EMIT error(tr("接收的candidate,序列化失败!"));
        return;
    }
    QJsonObject messageJsonObject;
    messageJsonObject["sdpMid"] = QString::fromStdString(candidate->sdp_mid());
    messageJsonObject["sdpMLineIndex"] = candidate->sdp_mline_index();
    messageJsonObject["candidate"] = QString::fromStdString(sdp);
    qDebug() << "messageJsonObject:" << QJsonDocument(messageJsonObject);
    //不清楚还要如何处理？？？？应该发给对端处理？ ？？？？？？？？
    qDebug() << "OnIceCandidate ! candidate" << messageJsonObject;
    QJsonDocument doc(messageJsonObject);
    Q_EMIT sendMessageToPeer(doc.toJson(QJsonDocument::Compact));
}

void WebrtcPeerConnectionObserver::OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface> >& streams)
{
    qDebug() << "WebrtcPeerConnectionObserver onAddTrack";
    receiver->track().release();
    Q_EMIT addTrack(receiver);
}

void WebrtcPeerConnectionObserver::OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver)
{
    auto* track = reinterpret_cast<webrtc::MediaStreamTrackInterface*>(receiver->track().release());
    track->Release();
}
