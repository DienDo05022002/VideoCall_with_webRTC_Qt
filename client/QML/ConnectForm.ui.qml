import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: page
    property alias ipinput: ipinput
    property alias portinput: portinput
    property alias connectbtn: connectbtn
    //    property alias testbutton: testbutton
    Rectangle {
        //        color: "red"
        //        width:
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        RowLayout {
            //            Layout.fillWidth: true
            anchors.centerIn: parent
            Label {
                text: qsTr("ip:")
                font.pixelSize: 25
                color: "#343b74"
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 30
            }
            TextInput {
                id: ipinput
                text: qsTr("127.0.0.1")
                clip: true
                font.pixelSize: 25
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 8
            }
            Label {
                text: qsTr("port:")
                font.pixelSize: 25
                color: "#343b74"
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 15
            }
            TextInput {
                id: portinput
                text: qsTr("4567")
                clip: true
                font.pixelSize: 25
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 8
            }
            Button {
                id: connectbtn
                text: qsTr("Connect")
                font.pixelSize: 25
                Layout.preferredHeight: page.height / 15
                Layout.preferredWidth: page.width / 8
            }
        }
    }
}
