import QtQuick 2.15

RegisterForm {
    Connections {
        target: usrinputbtn
        function onClicked() {
            if(usernameinput.text==="") return;

            //console.log("Code QML: conductor.getWsClient()=== " + conductor.getWsClient());
            conductor.getWsClient().setUsername(usernameinput.text);
            stackView.push(connectpage)
            console.log("Code QML: Username::: " +conductor.getWsClient().getUsername());
        }
    }
}
