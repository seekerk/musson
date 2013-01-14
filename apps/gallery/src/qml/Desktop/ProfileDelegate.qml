import QtQuick 1.1
import "UIConstants.js" as UI

Rectangle {
    id: profileDelegate
    height: 80
    width: profileDelegate.ListView.view.width
    color: "transparent"

    property bool isUserProfile: false
    signal delegateClicked()

    ListHighlight {
        id: highlight
        visible: false
    }

    Row {
        anchors.fill: parent
        anchors.topMargin: 8
        anchors.bottomMargin: 12
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.rightMargin: UI.DEFAULT_MARGIN
        spacing: 16

        Item {
            id: iconContainer
            width: 48
            height: 48

            Image {
                id: iconImage
                fillMode: Image.PreserveAspectFit
                smooth: true
                anchors.fill: parent
                source: !profileIcon ? "qrc:/icons/contact.png" : "file:/" + profileIcon
                asynchronous: true
                onStatusChanged: {
                    // load service icon if there was problems with loading user icon
                    if (iconImage.status == Image.Error) {
                        Qt.createQmlObject('import QtQuick 1.1;'
                                           +  'Image {anchors.fill:parent;source:"qrc:/icons/contact.png";}',
                                           iconImage)
                    }
                }
            }

            Image {
                id: onlineIcon
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 1
                anchors.rightMargin: 1
                visible: !offline
                source: "qrc:/icons/online.png"
            }
        }

        Text {
            id: profileTitle
            anchors.verticalCenter: parent.verticalCenter
            text: name + (isUserProfile ? " (" + service + ")" : "")
            font.pixelSize: UI.FONT_DEFAULT
            width: parent.width - iconContainer.width - UI.DEFAULT_MARGIN
            elide: Text.ElideRight
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
            when: (profileManager.selected == ownerId)
            PropertyChanges {
                target: highlight;
                visible: true
            }
        }
    ]

}
