import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    property alias idlabel: idlabel
    property alias namelabel: namelabel
    property alias mousearea: mousearea
    border.width: 2
    border.color:"#FFE4B5"
    color:"#ADD8E6"
    RowLayout {
        anchors.fill: parent
        Label {
            id: idlabel
            text: qsTr("id")
            font.pixelSize: 18
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/2
        }
        Label {
            id: namelabel
            text: qsTr("name")
            font.pixelSize: 18
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/2
        }
    }
    MouseArea {
        id: mousearea
        anchors.fill: parent

    }
}
