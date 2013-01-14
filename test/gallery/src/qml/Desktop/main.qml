// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import QtDesktop 0.1
import MussonCore 1.2

ApplicationWindow {
    id: appWindow
    title: "Musson Gallery"
    visible: true
    minimumHeight: 400
    minimumWidth: 600

    ServiceManager { id: global_sm; }

    AccountManager { id: accountManager; sm: global_sm; }
    ProfileManager { id: profileManager; sm: global_sm; }
    FilterManager { id: filterManager; sm: global_sm; }

//    AccountSettings {id: accountSettings}
//    NetworkSettings { id: networkSettings}
//    ProgramSettings { id: programSettings}
//    BugReport {id: bugReport}

    MenuBar {
        Menu {
            text: qsTr("File")
            MenuItem {text: qsTr("Quit"); shortcut: "Ctrl+Q"; onTriggered: Qt.quit()}
        }
        Menu {
            text: qsTr("Settings")
            MenuItem {
                text: qsTr("Accounts");
                onTriggered: {
                    var stWindow = Qt.createComponent("AccountSettings.qml");
                    if (stWindow.status == Component.Ready)
                        stWindow.createObject(appWindow, {"visible": true})
                }
            }
            MenuItem {
                text: qsTr("Network");
                onTriggered: {
                    var stWindow = Qt.createComponent("NetworkSettings.qml");
                    if (stWindow.status == Component.Ready)
                        stWindow.createObject(appWindow, {"visible": true})
                }
            }

//            MenuItem {text:  qsTr("Program settings"); onTriggered: programSettings.visible = true}
        }

        Menu {
            text: qsTr("Help")
            MenuItem { text: qsTr("Statistics");
                onTriggered: {
                    var stWindow = Qt.createComponent("StatInfo.qml");
                    if (stWindow.status === Component.Ready)
                        stWindow.createObject(appWindow, {"visible": true})
                }
            }

            MenuItem { text: qsTr("Bug report");
                onTriggered: {
                    var stWindow = Qt.createComponent("BugReport.qml");
                    if (stWindow.status === Component.Ready) {
                        stWindow.createObject(appWindow, {"isManualFill": true, "visible": true})
                    }
                }
            }
        }
    }

    NotificationManager {
        id: notificationManager
        sm: global_sm

        onShowNotification: {
            banner.text = text
            banner.visible = true
        }

        onShowBugReport: {
            var stWindow = Qt.createComponent("BugReport.qml");
            if (stWindow.status === Component.Ready) {
                stWindow.createObject(appWindow, {"isManualFill": false,
                                          "bugAuthor": notificationManager.bugAuthor,
                                          "bugTitle": notificationManager.bugTitle,
                                          "bugBody": notificationManager.bugBody,
                                          "visible": true})
            }
        }

        onShowResultBugSending: {
            banner.text = text
            banner.visible = true
        }
    }

    Dialog {
        id: banner
        buttons: ok
        property alias text: message.text
        data {
            Text {
                id: message
                anchors.fill: parent
                wrapMode: "WordWrap"
            }
        }
    }

    Item {
        id: mainPage
        anchors.fill: parent
        Row {
            anchors.fill: parent

            Column {
                id: profilesPanel
                ProfilesPage {
                    id: profilesPage
                    width: appWindow.width / 2
                    height: mainPage.height
                    onProfileSelected: {
                        albumsPage.setOwner(ownerName, ownerIcon, owner)
                    }
                }
                visible: true
            }

            Column {
                id: albumsPanel
                AlbumsPage {
                    id: albumsPage
                    width: appWindow.width / 2
                    height: mainPage.height

                    onAlbumSelected: {
                        photosPage.setAlbum(albumTitle, albumIcon, album);
                        photosPanel.visible = true;
                        profilesPanel.visible = false;
                        albumsPage.showBackButton(true)
                    }
                    onBack: {
                        photosPanel.visible = false;
                        profilesPanel.visible = true;
                        albumsPage.showBackButton(false)
                    }
                }
                visible: true
            }

            Column {
                id: photosPanel
                PhotosPage {
                    id: photosPage
                    width: appWindow.width / 2
                    height: mainPage.height
                }
                visible: false
            }
        }
    }

    Component.onCompleted: {
        filterManager.initFilters();
        if (!accountManager.hasAccounts)
        {
            var stWindow = Qt.createComponent("AccountSettings.qml");
            if (stWindow.status === Component.Ready)
                stWindow.createObject(appWindow, {"visible": true});
        }
    }
}
