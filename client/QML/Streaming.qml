import QtQuick 2.15

StreamingForm {
    Component.onCompleted: {
        //conductor.setLocalVideoSink(videoOutput_local.videoSink);
        //conductor.setRemoteVideoSink(videoOutput_remote.videoSink);
        conductor.setLocalVideo(videoOutput_local);
        conductor.setRemoteVideo(videoOutput_remote);
    }
    Component.onDestruction: {
        //conductor.resetLocalVideoSink();
        //conductor.resetRemoteVideoSink();
        conductor.resetLocalVideo();
        conductor.resetRemoteVideo();
    }
}
