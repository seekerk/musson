import QtQuick 1.1
import MussonComponents 1.0
import QtDesktop 0.1
import "UIConstants.js" as UI

Rectangle {
    id: driverDelegate
    height: 60
    width: driverDelegate.ListView.view.width
    color: "transparent"

    signal delegateClicked()
    signal delegateDoubleClicked()

    ListHighlight {
        id: highlight
        visible: false
    }
    Row {
        anchors.fill: parent
        spacing: 16
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.rightMargin: UI.DEFAULT_MARGIN
        anchors.topMargin: 8
        anchors.bottomMargin: 12

        Item {
            width: 45
            height: 45

            Icon {
                anchors.fill: parent
                icon: sIcon
            }
        }

        Text {
            id: driverName
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: UI.FONT_DEFAULT
            color: UI.COLOR_FOREGROUND
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: driverDelegate
        onClicked: delegateClicked()
        onDoubleClicked: delegateDoubleClicked()
    }

    states: [
        State {
            name: "selected"
            when: (driverManager.selected == driverId)
            PropertyChanges { target: highlight; visible: true; }
        }
    ]
}
