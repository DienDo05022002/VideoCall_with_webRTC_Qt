#include "WebrtcVideoSinkInterface.h"

#include "api/video/i420_buffer.h"
#include "third_party/libyuv/include/libyuv.h"

//#include <synchapi.h>
#include <QMutexLocker>
#include <QDebug>

WebrtcVideoSinkInterface::WebrtcVideoSinkInterface(webrtc::VideoTrackInterface* track_to_render,
    QObject* parent)
    : QObject{ parent }, rendered_track_(track_to_render)
{
    //    ::InitializeCriticalSection(&buffer_lock_);
    //    ZeroMemory(&bmi_, sizeof(bmi_));
    //    bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    //    bmi_.bmiHeader.biPlanes = 1;
    //    bmi_.bmiHeader.biBitCount = 32;
    //    bmi_.bmiHeader.biCompression = BI_RGB;
    //    bmi_.bmiHeader.biWidth = width;
    //    bmi_.bmiHeader.biHeight = -height;
    //    bmi_.bmiHeader.biSizeImage =
    //        width * height * (bmi_.bmiHeader.biBitCount >> 3);
    QMutexLocker mutexLocker(&m_mutex);
    rendered_track_->AddOrUpdateSink(this, rtc::VideoSinkWants());
}

WebrtcVideoSinkInterface::~WebrtcVideoSinkInterface()
{
    QMutexLocker mutexLocker(&m_mutex);
    rendered_track_->RemoveSink(this);
    //    ::DeleteCriticalSection(&buffer_lock_);
}

void WebrtcVideoSinkInterface::OnFrame(const webrtc::VideoFrame& videoFrame)
{
    //#ifndef QT_NO_DEBUG
    //    qInfo()<<Q_FUNC_INFO<< __LINE__ ;
    //#endif
    QMutexLocker mutexLocker(&m_mutex);
    //    webrtc::MutexLock lock(&capture_lock_);
    //    int height = videoFrame.height();
    //    int width = videoFrame.width();

    //    static int incoming_frames_ = 0;
    //    incoming_frames_ ++;

    //    qDebug()<<"width"<<width<<"height"<<height<<"incoming_frames_"<<incoming_frames_;
    rtc::scoped_refptr<webrtc::I420BufferInterface> buffer(
        videoFrame.video_frame_buffer()->ToI420());
    if (videoFrame.rotation() != webrtc::kVideoRotation_0) {
        buffer = webrtc::I420Buffer::Rotate(*buffer, videoFrame.rotation());
    }

    //    setSize(buffer->width(), buffer->height());

    //    RTC_DCHECK(image_.get() != NULL);
    //    libyuv::I420ToARGB(buffer->DataY(), buffer->StrideY(), buffer->DataU(),
    //                       buffer->StrideU(), buffer->DataV(), buffer->StrideV(),
    //                       image_.get(),
    //                       bmi_.bmiHeader.biWidth * bmi_.bmiHeader.biBitCount / 8,
    //                       buffer->width(), buffer->height());

    const int length = buffer->width() * buffer->height() * 4;
    //    qDebug()<<"buffer->width()"<<buffer->width()<<"buffer->height()"<<buffer->height();
    uchar* imageBuffer = new uchar[length];


    libyuv::I420ToRGBA(buffer->DataY(), buffer->StrideY(), buffer->DataU(),
        buffer->StrideU(), buffer->DataV(), buffer->StrideV(),
        imageBuffer,
        buffer->width() * 4,
        buffer->width(), buffer->height());

    QImage image(imageBuffer, buffer->width(), buffer->height(), QImage::Format_RGBA8888);
    //    qDebug()<<image;//image.save("local.png");return;
    //    m_lastImage=image.copy();
    Q_EMIT newVideoFrame(image.copy());
    delete[] imageBuffer;
}

//void WebrtcVideoSinkInterface::setSize(int width, int height)
//{
////    AutoLock<WebrtcVideoSinkInterface> lock(this);

//    if (width == bmi_.bmiHeader.biWidth && height == bmi_.bmiHeader.biHeight) {
//      return;
//    }

//    bmi_.bmiHeader.biWidth = width;
//    bmi_.bmiHeader.biHeight = -height;
//    bmi_.bmiHeader.biSizeImage =
//        width * height * (bmi_.bmiHeader.biBitCount >> 3);
//    image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
//}

//// A little helper class to make sure we always to proper locking and
//// unlocking when working with VideoRenderer buffers.
//template <typename T>
//class AutoLock {
// public:
//  explicit AutoLock(T* obj) : obj_(obj) { obj_->Lock(); }
//  ~AutoLock() { obj_->Unlock(); }

// protected:
//  T* obj_;
//};
