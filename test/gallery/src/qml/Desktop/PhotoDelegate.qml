import QtQuick 1.1
import QtDesktop 0.1
import "UIConstants.js" as UI
import "date.js" as DT

Rectangle {
    id: photoDelegate
    height: photosView.cellHeight
    width: photosView.cellWidth
    color: "transparent"
    radius: 10

    signal delegateClicked()

    ListHighlight {
        id: highlight
        visible: false
    }

    Item {
        id: iconItem
        anchors.fill: parent
        anchors.margins: 2

        Image {
            id: iconImage
            fillMode: Image.PreserveAspectCrop
            clip: true
            smooth: true
            anchors.fill: parent
            visible: canShowIcon
            source: "file:/" + photoIcon
        }

        Rectangle {
            anchors.fill: parent
            visible: !canShowIcon
            clip: true
            color: "#f0f1f3"
            border.color: "#706F6F"
            border.width: 1
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onClicked: delegateClicked()
    }

    states: [
        State {
            name: "selected"
            when: (photosManager.selected == photoId)
            PropertyChanges {
                target: highlight;
                visible: true
            }
        }
    ]
}
