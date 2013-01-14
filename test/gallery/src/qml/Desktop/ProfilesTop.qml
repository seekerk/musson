import QtQuick 1.1
import QtDesktop 0.1
import MussonComponents 1.0
import "UIConstants.js" as UI

Item {
    id: filterHeader
    height: filterRect.height + UI.DEFAULT_MARGIN
    width: parent.width

    property string title: ""
    property bool isUpdating: false
    property alias myIcon: myInfo.iconSource

    signal refresh()
    signal showMe()
    signal showFilters()
    signal serviceChanged(string service, int serviceIndex)

    Rectangle {
        id: filterRect
        height: 64
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        color: "#137A94"

        ToolButton {
            id: myInfo
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: UI.DEFAULT_MARGIN

            height: 48
            width: 48

            iconSource: "qrc:/icons/contact.png"
            text: qsTr("My accounts")

            onClicked: showMe()
        }

        ToolButton {
            id: clearFilter
            anchors.left: myInfo.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: UI.DEFAULT_MARGIN

            height: 48
            width: 48

            iconSource: "qrc:/icons/delete filter.png"
            text: qsTr("Clear Filter")
            visible: servicesArea.count > 0 ? true : false

            onClicked: filterManager.selectService("Clear Filter", 0);
        }


        Row {
            anchors.left: clearFilter.right
            anchors.right: refreshBtn.left
            anchors.verticalCenter: parent.verticalCenter

            Repeater {
                id: servicesArea
                model: filterManager.serviceIconsModel

                delegate: ToolButton {
                    height: 40
                    width: 40

                    Icon {
                        anchors.fill: parent
                        icon: sIcon
                    }
                    onClicked: filterManager.selectService(name, index + 1)
                }

            }
        }

        ToolButton {
            id: refreshBtn
            width: 48
            height: 48
            anchors.right: parent.right
            anchors.rightMargin: UI.DEFAULT_MARGIN
            anchors.verticalCenter: parent.verticalCenter

            iconSource: "qrc:/icons/refresh.png"
            text: qsTr("Refresh list")

            enabled: isUpdating ? false : true;
            onClicked: refresh()
        }
    }
}
