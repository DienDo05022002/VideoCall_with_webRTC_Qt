import QtMultimedia 5.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: page
    property alias videoOutput_local: videoOutput_local
    property alias videoOutput_remote: videoOutput_remote
    ColumnLayout {
        anchors.fill: parent
        VideoOutput {
            id: videoOutput_local
            Layout.preferredHeight: page.height/2
            Layout.preferredWidth: page.width
            source: conductor.localVideo
            Label {
                font.pixelSize: 15
                text: qsTr("Local screen")
                anchors.left: parent.left
                anchors.top: parent.top
            }
        }
        VideoOutput {
            id: videoOutput_remote
            Layout.preferredHeight: page.height/2
            Layout.preferredWidth: page.width
            source: conductor.remoteVideo
            Label {
                font.pixelSize: 15
                text: qsTr("Remote screen")
                anchors.left: parent.left
                anchors.top: parent.top
            }
        }
    }
}
