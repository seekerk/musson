import QtQuick 1.0
import QtDesktop 0.1
import MussonComponents 1.0
import MussonCore 1.2
import "UIConstants.js" as UI
import "constants.js" as ExtrasConstants

ApplicationWindow {
    id: accountSettings
    minimumWidth: 400
    minimumHeight: 350
    modal: true

    property alias accountsCount: accountsView.count

    toolBar: accountTools
    data: accountsList

    ToolBar {
        id: accountTools

        ToolButton {
            id: addBtn
            text: qsTr("Add account")
            iconSource: "qrc:/icons/add.png"
            enabled: true
            onClicked: {
                var stWindow = Qt.createComponent("AddAccount.qml");
                if (stWindow.status == Component.Ready)
                    stWindow.createObject(appWindow, {"visible": true})
            }
        }

        ToolButton {
            id: deleteBtn
            text: qsTr("Delete account")
            iconSource: "qrc:/icons/delete.png"
            enabled: (accountManager.selected != "")
            onClicked: deleteDialog.visible = true
            anchors.left: addBtn.right
        }

        ToolButton {
            id: reconnectBtn
            text: qsTr("Reconnect")
            iconSource: "qrc:/icons/refresh.png"
            enabled: (accountManager.selected != "")
            onClicked: accountManager.reconnect()
            anchors.left: deleteBtn.right
        }

        ToolButton {
            id: enableBtn
            text: accountManager.isDisabled ? qsTr("Enable") : qsTr("Disable");
            iconSource: accountManager.isDisabled ? "qrc:/icons/enable.png" : "qrc:/icons/disable.png";
            enabled: (accountManager.selected != "")
            onClicked: accountManager.toggleAccount()
            anchors.left: reconnectBtn.right
        }
    }

    Dialog {
        id: deleteDialog
        title: qsTr("Delete?")
        data: Text{text:qsTr("Are you sure you want to delete this account?"); anchors.centerIn: parent}

        onAccepted: {
            accountManager.deleteAccount(accountsList.selectedAccount)
            accountManager.selected = ""
        }
    }

    Rectangle {
        id: accountsList
        anchors.fill: parent
        color: "transparent"

        Component {
            id: sectionHeader

            Item {
                width: parent.width
                height: headerText.height + UI.DEFAULT_MARGIN

                Rectangle {
                    id: divider
                    anchors.leftMargin: UI.DEFAULT_MARGIN
                    anchors.rightMargin: UI.DEFAULT_MARGIN
                    anchors.left: parent.left
                    anchors.right: headerText.left
                    anchors.verticalCenter: parent.verticalCenter
                    color: ExtrasConstants.LIST_SUBTITLE_COLOR
                    height: 1
                }

                Text {
                    id: headerText
                    anchors.rightMargin: UI.DEFAULT_MARGIN
                    anchors.right: parent.right
                    anchors.verticalCenter: divider.verticalCenter
                    color: ExtrasConstants.LIST_SUBTITLE_COLOR
                    font.pixelSize: ExtrasConstants.LIST_SUBTILE_SIZE
                    font.weight: Font.Light
                    text: (section == "true") ? qsTr("disabled") : qsTr("active")
                }
            }
        }

        ListView {
            id: accountsView
            anchors.fill: parent

            section.property: "disabled"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeader

            model: accountManager.model
            delegate: AccountDelegate {
                id: accountDelegate

                Connections {
                    onDelegateClicked: {
                        accountManager.selected = accountId;
                        accountManager.isDisabled = disabled;
                    }
                }
            }
        }

        EmptyState {
            visible: (accountsView.currentIndex < 0)
            text: qsTr("No accounts")
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        if (accountsCount == 0)
            addBtn.clicked();
    }
}
