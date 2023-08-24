import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "QML"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Register {}
    }
    Component {
        id: connectpage
        Connect {
            width: stackView.width
            height: stackView.height
        }
    }

    Component {
        id: peerpage
        PeerList {
            width: stackView.width
            height: stackView.height
        }
    }
    Component {
        id: streamingpage
        Streaming {
            width: stackView.width
            height: stackView.height}
    }
}
