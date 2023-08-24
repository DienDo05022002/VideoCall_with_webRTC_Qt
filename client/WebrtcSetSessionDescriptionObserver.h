#pragma once

#include "api/jsep.h"
#include "api/scoped_refptr.h"
#include <QObject>

class WebrtcSetSessionDescriptionObserver : public QObject,
    public webrtc::SetSessionDescriptionObserver
{
    Q_OBJECT
public:
    explicit WebrtcSetSessionDescriptionObserver(QObject* parent = nullptr);

    static rtc::scoped_refptr<WebrtcSetSessionDescriptionObserver> Create() {
        return rtc::scoped_refptr<WebrtcSetSessionDescriptionObserver>();
    }
    virtual void OnSuccess() { RTC_LOG(LS_INFO) << __FUNCTION__; }
    virtual void OnFailure(webrtc::RTCError error) {
        //      RTC_LOG(LS_INFO) << __FUNCTION__ << " " << ToString(error.type()) << ": "
        //                       << error.message();
    }

};
