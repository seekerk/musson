import QtQuick 1.0
import QtDesktop 0.1
import "UIConstants.js" as UI
import "constants.js" as ExtrasConstants
import "date.js" as DT

Rectangle {
    id: albumDelegate
    height: (infoColumn.height > 80) ? infoColumn.height + UI.DEFAULT_MARGIN : 80
    width: albumDelegate.ListView.view.width
    color: "transparent"

    signal delegateClicked()

    ListHighlight {
        id: highlight
        visible: false
    }

    Item {
        id: iconItem
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.bottomMargin: 12
        anchors.leftMargin: UI.DEFAULT_MARGIN

        width: 48
        height: 48

            Image {
                id: iconImage
                fillMode: Image.PreserveAspectCrop
                clip: true
                smooth: true
                anchors.fill: parent
                source: !canShowIcon ? "qrc:/icons/photoalbum.png" : "file:/" + albumIcon
            }
    }

    Column {
        id: infoColumn
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.rightMargin: UI.DEFAULT_MARGIN
        anchors.topMargin: 8
        anchors.bottomMargin: 12
        anchors.left: iconItem.right
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 1

        Text {
            id: titleText
            text: title
            font.pixelSize: UI.FONT_DEFAULT
            wrapMode: Text.Wrap
            width: parent.width
        }

        Text {
            id: descriptionText
            text: description
            font.weight: Font.Light
            width: parent.width
            wrapMode: Text.Wrap
            font.pixelSize: UI.FONT_LSMALL
            visible: text
        }

        Text {
            id: albumSizeText
            text: qsTr("Photos: ") + size
            color: "#706F6F"
            font.weight: Font.Light
            font.pixelSize: UI.FONT_XSMALL
        }

        Text {
            id: timeCreatedText
            text: qsTr("Created: ") + DT.formattedTime(timeCreate)
            visible: timeCreate
            color: "#706F6F"
            font.weight: Font.Light
            font.pixelSize: UI.FONT_XSMALL
        }

        Text {
            id: timeUpdatedText
            visible: timeUpdate
            text: qsTr("Last update: ") + DT.formattedTime(timeUpdate)
            color: "#706F6F"
            font.weight: Font.Light
            font.pixelSize: UI.FONT_XSMALL
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
            when: (albumsManager.selected == albumId)
            PropertyChanges {
                target: highlight;
                visible: true
            }
        }
    ]
}
