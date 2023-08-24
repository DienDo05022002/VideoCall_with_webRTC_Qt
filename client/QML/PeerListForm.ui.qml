import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
Page {
    id: page
    property alias listView: listView
    title: qsTr("Peer List")
    /*Button {
                id: connectText
                text: qsTr("connectText")
                font.pixelSize: 18
                width: 80
                height: 50
    }*/

    ListView {
        id: listView
        anchors.fill: parent
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        header: listhead
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        delegate:Item {
            RowLayout {
                anchors.fill: parent
                Label {
                    id:id
                    text: qsTr("id")
                    font.pixelSize: 20
                    color: "#B3EE3A"
                    Layout.preferredWidth: parent.width/2
                    Layout.preferredHeight: parent.height
                }
                Label {
                    id:name
                    text: qsTr("name")
                    font.pixelSize: 20
                    color: "#00FFFF"
                    Layout.preferredWidth: parent.width/2
                    Layout.preferredHeight: parent.height
                }
            }
        }
    }


    Component {
        id: listhead
        RowLayout {
            Label {
                id:idhead
                text: qsTr("id")
                font.pixelSize: 20
                color: "#6B8E23"
                Layout.preferredWidth: page.width/2
                Layout.preferredHeight: page.height/10
            }
            Label {
                id:namehead
                text: qsTr("username")
                font.pixelSize: 20
                color: "#00CED1"
                Layout.preferredWidth: page.width/2
                Layout.preferredHeight: page.height/10
            }
        }
    }
}
