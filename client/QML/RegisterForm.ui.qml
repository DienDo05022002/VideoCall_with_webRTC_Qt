import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: page
    property alias usernameinput: usernameinput
    property alias usrinputbtn: usrinputbtn
    Rectangle {
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCente
        RowLayout {
            id: rowLayout
            anchors.centerIn: parent
            spacing: 20
            Label {
                text: qsTr("Please choose a\nunique name for yourself first:")
                font.pixelSize: 18
                color: "#343b74"
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 6
            }
            TextInput {
                id: usernameinput
                text: qsTr("feng")
                clip: true
                font.pixelSize: 18
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 6
            }
            Button {
                id: usrinputbtn
                text: qsTr("Sure")
                font.pixelSize: 20
//                Keys.onEnterPressed: usrinputbtn.clicked
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 15
            }
        }
    }
}
