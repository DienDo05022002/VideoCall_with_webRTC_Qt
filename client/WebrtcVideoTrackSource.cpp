//#include "WebrtcVideoTrackSource.h"
//
//#include "modules/video_capture/video_capture.h"
//#include "modules/video_capture/video_capture_factory.h"
//
//#include "test/vcm_capturer.h"
//rtc::scoped_refptr<WebrtcVideoTrackSource> WebrtcVideoTrackSource::create()
//{
//    const size_t kWidth = 640;
//    const size_t kHeight = 480;
//    const size_t kFps = 30;
//    std::unique_ptr<webrtc::test::VcmCapturer> capturer;
//    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
//        webrtc::VideoCaptureFactory::CreateDeviceInfo());
//    if (!info) {
//        return nullptr;
//    }
//    int num_devices = info->NumberOfDevices();
//    for (int i = 0; i < num_devices; ++i) {
//        capturer = absl::WrapUnique(
//            webrtc::test::VcmCapturer::Create(kWidth, kHeight, kFps, i));
//        if (capturer) {
//            return new rtc::RefCountedObject<WebrtcVideoTrackSource>(std::move(capturer));
//            //return rtc::make_ref_counted<WebrtcVideoTrackSource>(std::move(capturer));
//        }
//    }
//
//    return nullptr;
//}
