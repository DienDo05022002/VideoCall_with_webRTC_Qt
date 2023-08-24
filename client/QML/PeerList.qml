import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

PeerListForm {
    listView.model: ListModel {}
    listView.delegate: PeerDelegate {
        width: listView.width
        height: listView.height/20
        idlabel.text: model.id
        namelabel.text: model.name
    }
    Connections {
        target: conductor
        function onFreshPeerList() {
            console.log("fresh");
            listView.model.clear();
            for(let peer of conductor.getWsClient().getPeerList())
            {
                console.log("fresh:for loop");
                console.log(JSON.stringify(peer))
                listView.model.append(peer);
            }
        }
    }
    Connections {
        target:conductor
        function onSwitchToStreamingUi() {
            console.log("switchToStreamingUi");
            stackView.push(streamingpage);
        }
    }
    
}
