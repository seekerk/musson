import QtQuick 1.1
import QtDesktop 0.1
import "UIConstants.js" as UI

Rectangle {
    id: albumHeader
    height: 64
    width: parent.width
    color: "#137A94"

    property string title: ""
    property string icon: ""
    property bool hideRefresh: false
    property bool isUpdating: false
    signal refresh()
    signal back()
    property alias backButton: backBtn

    ToolButton {
        id: backBtn
        anchors.left: parent.left
        anchors.leftMargin: UI.DEFAULT_MARGIN
        width: 48
        height: 48
        anchors.verticalCenter: parent.verticalCenter
        visible: false
        iconSource: "qrc:/icons/back.png"
        text: qsTr("Return")

        onClicked: back()
    }

    ToolButton {
        id: refreshBtn
        anchors.right: parent.right
        anchors.rightMargin: UI.DEFAULT_MARGIN
        width: 48
        height: 48
        anchors.verticalCenter: parent.verticalCenter
        visible: !hideRefresh
        iconSource: "qrc:/icons/refresh.png"
        text: qsTr("Refresh list")

        enabled: isUpdating ? false : true;
        onClicked: refresh()
    }

    Item {
        id: ownerInfo
        anchors.left: backBtn.visible ? backBtn.right : parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: UI.DEFAULT_MARGIN

        height: 60
        width: 60

            Image {
                id: ownerIcon
                fillMode: Image.PreserveAspectCrop
                clip: true
                smooth: true
                anchors.fill: parent
                source: !icon ? "qrc:/icons/contact.png" : "file:/" + icon
                onStatusChanged: {
                    // load service icon if there was problems with loading user icon
                    if (ownerIcon.status == Image.Error) {
                        Qt.createQmlObject('import QtQuick 1.1;'
                                           +  'Image {anchors.fill:parent;source:"qrc:/icons/contact.png";}',
                                           ownerIcon)
                    }
                }
            }
    }

    Text {
        id: titleText
        anchors.left: ownerInfo.right
        anchors.verticalCenter: ownerInfo.verticalCenter
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.right: refreshBtn.left
        elide: Text.ElideRight
        text: title
        font.pixelSize: UI.FONT_XLARGE
        font.weight: Font.Light
        color: "white"
        smooth: true
    }
}
