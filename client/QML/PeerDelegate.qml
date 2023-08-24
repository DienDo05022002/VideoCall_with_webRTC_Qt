import QtQuick 2.15

PeerDelegateForm {
    Connections {
        target: mousearea
        function onClicked() {
            conductor.connectToPeer(Number(idlabel.text));
            console.log("connectto:")
            console.log(Number(idlabel.text));
        }
    }
}
