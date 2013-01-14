message(Desktop build)

target.path = $$BINDIR
INSTALLS += target

QML_FILES += \
    qml/Desktop/AccountDelegate.qml  \
    qml/Desktop/AlbumDelegate.qml \
    qml/Desktop/EmptyState.qml \
    qml/Desktop/PhotoDelegate.qml \
    qml/Desktop/ProfilesFooter.qml \
    qml/Desktop/StatInfo.qml \
    qml/Desktop/AccountHeader.qml \
    qml/Desktop/AlbumHeader.qml \
    qml/Desktop/constants.js \
    qml/Desktop/ListHighlight.qml \
    qml/Desktop/PhotoHeader.qml \
    qml/Desktop/ProfilesPage.qml \
    qml/Desktop/stat.js \
    qml/Desktop/AccountSettings.qml \
    qml/Desktop/AlbumsPage.qml \
    qml/Desktop/date.js \
    qml/Desktop/main.qml \
    qml/Desktop/PhotosPage.qml \
    qml/Desktop/ProfilesTop.qml \
    qml/Desktop/UIConstants.js \
    qml/Desktop/AddAccount.qml \
    qml/Desktop/BugReport.qml \
    qml/Desktop/DriverDelegate.qml \
    qml/Desktop/NetworkSettings.qml \
    qml/Desktop/ProfileDelegate.qml \
    qml/Desktop/StatDelegate.qml

!contains(DEFINES, QML_TO_RESOURCES) {
    DEFINES += QMLDIR=\\\"$$DATADIR/$${TARGET}/qml\\\"
    qml.path = $$DATADIR/$${TARGET}/qml
    qml.files = $$QML_FILES
    INSTALLS += qml
} else {
    RESOURCES += qml/Desktop/qml.qrc
    DEFINES += QMLDIR=\\\"qrc:/qml\\\"
}

# Please do not modify the following two lines. Required for deployment.
include(qmldesktopviewer/qmldesktopviewer.pri)

RESOURCES += \
    ../data/Desktop/icons.qrc
