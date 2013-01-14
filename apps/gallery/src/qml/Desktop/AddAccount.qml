import QtQuick 1.0
import QtDesktop 0.1
import MussonComponents 1.0

ApplicationWindow {
    id: addAccount

    minimumWidth: 200
    minimumHeight: 300
    modal: true

    property string accountName: ""

    DriverManager {
        id: driverManager
        sm: global_sm
        selected: ""
    }

    Column {
        AccountHeader {
            width: addAccount.width
            id: accountHeader
            onNameChanged: {
                addAccount.accountName = name;
            }
        }
        ListView {
            id: driversView
            width: addAccount.width
            height: addAccount.height - accountHeader.height - acceptBtn.height - 5
            onCountChanged: console.log("driver count: " + driversView.count);
            model: driverManager.model
            clip: true
            delegate: DriverDelegate {
                id: driverDelegate

                Connections {
                    onDelegateClicked: {
                        driverManager.selected = driverId;
                        console.log("selected driver " + driverId);
                    }
                    onDelegateDoubleClicked: {
                        driverManager.selected = driverId;
                        console.log("selected driver " + driverId);
                        acceptBtn.clicked();
                    }
                }
            }
        }
        Row {
            Button {
                id: acceptBtn
                text: qsTr("Add")

                states: State {
                    name: "disabled"
                    when: (driverManager.selected == "")
                    PropertyChanges {
                        target: acceptBtn
                        enabled: false
                    }
                }
                onClicked: {
                    var curDriver = driverManager.getSelectedDriver();
                    console.log(curDriver.name);
                    console.log(addAccount.accountName);
                    if (addAccount.accountName != "")
                        accountManager.addAccount(addAccount.accountName, curDriver);
                    else
                        accountManager.addAccount(curDriver);
                    close();
                }
            }

            Button {
                text: qsTr("Cancel")

                onClicked: {
                    close();
                }
            }
        }

    }
}
