#pragma once

#include "api/media_stream_interface.h"
#include "api/video/video_frame.h"

#include <QImage>
#include <QObject>
#include <QMutex>

class WebrtcVideoSinkInterface : public QObject,
    public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
    Q_OBJECT
public:
    explicit WebrtcVideoSinkInterface(webrtc::VideoTrackInterface* track_to_render,
        QObject* parent = nullptr);
    ~WebrtcVideoSinkInterface();

    //VideoSinkInterface implementation. Xử lý khung nhận được
    void OnFrame(const webrtc::VideoFrame& videoFrame) override;
Q_SIGNALS:
    void newVideoFrame(const QImage image);
protected:
    //    void setSize(int width, int height);
private:
    //    BITMAPINFO bmi_;
    //    std::unique_ptr<uint8_t[]> image_;
    //    CRITICAL_SECTION buffer_lock_;
    //    webrtc::Mutex capture_lock_;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> rendered_track_;

    QMutex m_mutex;
};


