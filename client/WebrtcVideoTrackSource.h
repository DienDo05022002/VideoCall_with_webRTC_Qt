#pragma once

#include "pc/video_track_source.h"
#include "test/vcm_capturer.h"
#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_factory.h"

#include <QObject>
#include <QDebug>

//class WebrtcVideoTrackSource : public QObject,
//    public webrtc::VideoTrackSource
//{
//    Q_OBJECT
//public:
//    explicit WebrtcVideoTrackSource(QObject* parent = nullptr) = delete;
//
//    static rtc::scoped_refptr<WebrtcVideoTrackSource> create();
//    /*{
//        const size_t kWidth = 640;
//        const size_t kHeight = 480;
//        const size_t kFps = 30;
//        std::unique_ptr<webrtc::test::VcmCapturer> capturer;
//        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
//            webrtc::VideoCaptureFactory::CreateDeviceInfo());
//        if (!info) {
//            return nullptr;
//        }
//        int num_devices = info->NumberOfDevices();
//        for (int i = 0; i < num_devices; ++i) {
//            capturer = absl::WrapUnique(
//                webrtc::test::VcmCapturer::Create(kWidth, kHeight, kFps, i));
//            if (capturer) {
//                return rtc::make_ref_counted<WebrtcVideoTrackSource>(std::move(capturer));
//            }
//        }
//
//        return nullptr;
//    };*/
//
//    //signals:
//protected:
//    explicit WebrtcVideoTrackSource(
//        std::unique_ptr<webrtc::test::VcmCapturer> capturer)
//        : VideoTrackSource(/*remote=*/false), capturer_(std::move(capturer)) {
//        qDebug() << "WebrtcVideoTrackSource";
//    }
//private:
//    rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
//        return capturer_.get();
//    }
//    std::unique_ptr<webrtc::test::VcmCapturer> capturer_;
//};

class WebrtcVideoTrackSource : public webrtc::VideoTrackSource {
public:
    static rtc::scoped_refptr<WebrtcVideoTrackSource> create() {
        const size_t kWidth = 640;
        const size_t kHeight = 480;
        const size_t kFps = 30;
        std::unique_ptr<webrtc::test::VcmCapturer> capturer;
        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
            webrtc::VideoCaptureFactory::CreateDeviceInfo());
        if (!info) {
            return nullptr;
        }

        int num_devices = info->NumberOfDevices();
        for (int i = 0; i < num_devices; ++i) {
            capturer = absl::WrapUnique(
                webrtc::test::VcmCapturer::Create(kWidth, kHeight, kFps, i));
            if (capturer) {
                return new rtc::RefCountedObject<WebrtcVideoTrackSource>(
                    std::move(capturer));
            }
        }

        return nullptr;
    }

protected:
    explicit WebrtcVideoTrackSource(
        std::unique_ptr<webrtc::test::VcmCapturer> capturer)
        : VideoTrackSource(/*remote=*/false), capturer_(std::move(capturer)) {}

private:
    rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
        return capturer_.get();
    }
    std::unique_ptr<webrtc::test::VcmCapturer> capturer_;
};