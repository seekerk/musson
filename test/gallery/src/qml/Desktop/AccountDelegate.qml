import QtQuick 1.1
import QtDesktop 0.1
import MussonComponents 1.0
import "UIConstants.js" as UI

Rectangle {
    id: accountDelegate
    height: 80
    width: accountDelegate.ListView.view.width
    color: "transparent"

    signal delegateClicked()

    ListHighlight {
        id: highlight
        visible: false
    }

    Row {
        anchors.fill: parent
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.rightMargin: UI.DEFAULT_MARGIN
        anchors.topMargin: 8
        anchors.bottomMargin: 12
        spacing: 16

        Item {
            id: iconContainer
            height: 64
            width: 64

            Image {
                id: iconImage
                anchors.fill: parent
                source: "file:/" + accIcon
                fillMode: Image.PreserveAspectCrop

                onStatusChanged: {
                    // load service icon if there was problems with loading user icon
                    if (iconImage.status == Image.Error) {
                        Qt.createQmlObject('import QtQuick 1.1;'
                                           +  'import MussonComponents 1.0;'
                                           +  'Icon {anchors.fill:parent;icon:driverIcon;}',
                                           iconImage)
                    }
                }
            }

            Icon {
                anchors.top: parent.top
                anchors.right: parent.right
                width: 20
                height: 20
                anchors.topMargin: 1
                anchors.rightMargin: 1
                icon: driverIcon
                visible: accIcon != ""
            }
        }

        Text {
            id: accountTitle
            anchors.verticalCenter: parent.verticalCenter
            text: isNetworkEnabled ? name : name + qsTr(" (offline)")
            font.pixelSize: UI.FONT_DEFAULT
            elide: Text.ElideRight
            width: parent.width - iconContainer.width - UI.DEFAULT_MARGIN
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
            when: (accountManager.selected == accountId)
            PropertyChanges {
                target: highlight;
                visible: true
            }
        }
    ]
}
