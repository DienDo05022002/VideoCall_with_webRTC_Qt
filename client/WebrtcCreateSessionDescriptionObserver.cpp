#include "WebrtcCreateSessionDescriptionObserver.h"
#include "WebrtcSetSessionDescriptionObserver.h"
#include <QDebug>
#include <QJsonDocument>

WebrtcCreateSessionDescriptionObserver::WebrtcCreateSessionDescriptionObserver(
    bool& loopback,
    rtc::scoped_refptr<webrtc::PeerConnectionInterface>& peer_connection,
    QObject* parent)
    : QObject{ parent }, m_loopback(loopback), m_peer_connection(peer_connection)
{

}

void WebrtcCreateSessionDescriptionObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__;
#endif
    m_peer_connection->SetLocalDescription(
        WebrtcSetSessionDescriptionObserver::Create().get(), desc);

    std::string sdp;
    desc->ToString(&sdp);

    // For loopback test. To save some connecting delay.
    if (m_loopback) {
        // Replace message type from "offer" to "answer"
        std::unique_ptr<webrtc::SessionDescriptionInterface> session_description =
            webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdp);
        m_peer_connection->SetRemoteDescription(
            WebrtcSetSessionDescriptionObserver::Create().get(),
            session_description.release());
        return;
    }

    QJsonObject messageJsonObject;
    messageJsonObject["kind"] = QString(webrtc::SdpTypeToString(desc->GetType()));
    messageJsonObject["sdp"] = QString::fromStdString(sdp);
    QJsonDocument doc(messageJsonObject);
    Q_EMIT sendMessageToPeer(doc.toJson(QJsonDocument::Compact));
}

void WebrtcCreateSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
#ifndef QT_NO_DEBUG
    qInfo() << Q_FUNC_INFO << __LINE__;
#endif
    //    RTC_LOG(LS_ERROR) << ToString(error.type()) << ": " << error.message();

    //    Q_EMIT this->error(tr("获取会话描述失败!"), error.message());
}