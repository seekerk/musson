import QtQuick 1.1
import com.nokia.meego 1.0
import MussonComponents 1.0
import com.nokia.extras 1.1
import MussonCore 1.2

PageStackWindow {
    id: appWindow

    property bool lockPortraitOrientation: programManager.lockOrientation
    property int _PROFILES_TAB: 0
    property int _ALBUMS_TAB: 1

    ServiceManager { id: global_sm; }

    ProgramManager {
        id: programManager
        sm: global_sm

        onLockOrientationChanged: lockPortraitOrientation = lockOrientation
        onLightThemeChanged: theme.inverted = !lightTheme
        onHideStatusBarChanged: appWindow.showStatusBar = !hideStatusBar
    }

    NotificationManager {
        id: notificationManager
        sm: global_sm

        onShowNotification: {
            banner.text = text
            banner.show()            
        }

        onShowBugReport: {
            bugReport.isManualFill = false
            bugReport.bugAuthor = notificationManager.bugAuthor
            bugReport.bugTitle = notificationManager.bugTitle
            bugReport.bugBody = notificationManager.bugBody
            bugReport.open()
        }

        onShowResultBugSending: {
            banner.text = text
            banner.show()
        }
    }

    InfoBanner {
        id: banner
        topMargin: appWindow.showStatusBar ? 42 : 10
        timerShowTime: 5000
    }

    FilterManager { id: filterManager; sm: global_sm; }
    AccountManager { id: accountManager; sm: global_sm; }
    AlbumsManager { id: albumsManager; sm: global_sm; }
    ProfileManager { id: profileManager; sm: global_sm; }

    ProfilesPage {id: profilesPage}
    ImageViewerPage {id: imageViewerPage}

    ProfileBrowserPage {
        id: profileBrowserPage

        ToolBarLayout {
            id: pbTools

            ToolIcon {
                id: backBtn
                platformIconId: "toolbar-back"
                anchors.left: parent===undefined ? undefined : parent.left
                onClicked: profileBrowserPage.userTools ? pageStack.pop(userProfiles) : pageStack.pop(profilesPage)
            }

            ButtonRow {
                ToolIcon {
                    platformIconId: "toolbar-add"
                    visible: !profileBrowserPage.profileTools
                    onClicked: {
                        userProfiles.isCreateAlbum = true
                        pageStack.push(userProfiles)
                    }
                }

                TabButton {
                    id: profileBrowserTab
                    text: qsTr("Friends")
                    visible: profileBrowserPage.profileTools
                    iconSource: theme.inverted ? "qrc:/profiles_inverted.png" : "qrc:/profiles.png"
                    onClicked: pageStack.push(profileBrowserPage)
                    states: State {
                        name: "checked"
                        when: profileBrowserPage.status == PageStatus.Activating
                        PropertyChanges {
                            target: profileBrowserTab
                            checked: true
                        }

                    }
                }

                TabButton {
                    id: profileMessagesTab
                    visible: profileBrowserPage.profileTools
                    checked: (messagesPage.visible)
                    text: qsTr("Messages")
                    iconSource: theme.inverted ? "qrc:/messages_inverted.png" : "qrc:/messages.png"
                    onClicked: {
                        messagesPage.isFriendMessages = true
                        profileBrowserPage.showFriendMessages()
                    }
                    states: State {
                        name: "checked"
                        when: messagesPage.status == PageStatus.Activating
                        PropertyChanges {
                            target: profileMessagesTab
                            checked: true
                        }

                    }
                }

                TabButton {
                    id: profileAlbumsTab
                    checked: albumsPage.visible
                    text: qsTr("Albums")
                    visible: profileBrowserPage.profileTools
                    iconSource: theme.inverted ? "qrc:/albums_inverted.png" : "qrc:/albums.png"
                    onClicked: {
                        profileBrowserPage.showFriendAlbums()
                        photosPage.isMyAlbums = false
                    }
                    states: State {
                        name: "checked"
                        when: albumsPage.status == PageStatus.Activating
                        PropertyChanges {
                            target: profileAlbumsTab
                            checked: true
                        }

                    }
                }
            }

            ToolIcon {
                platformIconId: "toolbar-view-menu";
                anchors.right: parent===undefined ? undefined : parent.right
                onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
            }
        }
    }

    AccountSettings {id: accountSettings}
    NetworkSettings { id: networkSettings}
    ProgramSettings { id: programSettings}
    StatInfo { id: statInfo }

    Page {
        id: albumsPage
        orientationLock: appWindow.lockPortraitOrientation ? PageOrientation.LockPortrait : PageOrientation.Automatic
        tools: pbTools

        property bool profileTools: false
        property alias showMyAlbums: albumsManager.showMyAlbums
        property alias profileIcon: albumsManager.profileIcon

        function setOwner(owner) {
            albumsManager.owner = owner
        }

        function createAlbum(owner, name, description, privacy) {
            if (!albumsManager.createAlbum(owner, name, description, privacy)) {
                notificationManager.createNotification("Album was successfully created.")
            }
        }

        function refresh() {
            albumsManager.refreshAlbums()
        }

        property string ownerName: ""
        property string ownerIcon: ""

        function load() {
            albumsLoader.source = "AlbumsPage.qml"
        }

        Loader { id: albumsLoader; anchors.fill: parent; }
    }

    Page {
        id: newsPage
        tools: newsTools
        orientationLock: appWindow.lockPortraitOrientation ? PageOrientation.LockPortrait : PageOrientation.Automatic

        function load() {
            newsLoader.source = "NewsPage.qml"
        }

        Loader { id: newsLoader; anchors.fill: parent; }

        ToolBarLayout {
            id: newsTools

            ToolIcon {
                platformIconId: "toolbar-back"
                anchors.left: parent===undefined ? undefined : parent.left
                onClicked: pageStack.pop();
            }

            ToolIcon {
                platformIconId: "toolbar-view-menu";
                anchors.right: parent===undefined ? undefined : parent.right
                onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
            }
        }
    }

    UserProfilesPage {id: userProfiles}
    PhotosPage { id: photosPage }
    BugReport {id: bugReport}

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Accounts"); onClicked: pageStack.push(accountSettings) }
            MenuItem { text: qsTr("Network"); onClicked: networkSettings.open()}
            MenuItem { text: qsTr("Program settings"); onClicked: programSettings.open()}
            MenuItem { text: qsTr("Statistics"); onClicked: statInfo.open()}
            MenuItem { text: qsTr("Bug report");
                onClicked: {
                    bugReport.isManualFill = true;
                    bugReport.open()
                    bugReport.cleanForm()
                }
            }
        }
    }

    Menu {
        id: moreMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Albums"); onClicked: {
                    var owner
                    albumsPage.setOwner(owner)
                    albumsPage.ownerName = qsTr("My albums")
                    albumsPage.ownerIcon = albumsPage.profileIcon
                    profileBrowserPage.profileTools = false
                    photosPage.isMyAlbums = true
                    albumsPage.load()
                    pageStack.push(albumsPage)
                }
            }
            MenuItem { text: qsTr("User profiles"); onClicked: {userProfiles.isCreateAlbum = false; pageStack.push(userProfiles)} }
        }
    }

    Component.onCompleted: {
        lockPortraitOrientation = programManager.lockOrientation
        theme.inverted = !programManager.lightTheme
        appWindow.showStatusBar = !programManager.hideStatusBar
        filterManager.initFilters()

        pageStack.push(profilesPage)

        if (accountSettings.accountsCount == 0)
            pageStack.push(accountSettings)
    }
}
