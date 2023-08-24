import QtQuick 2.15

ConnectForm {
    Connections {
        target: connectbtn
        function onClicked() {
            conductor.getWsClient().startLogin(ipinput.text,Number(portinput.text));
            console.log("Connections");
            stackView.push(peerpage)
        }
    }

}
