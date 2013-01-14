import QtQuick 1.1
import com.nokia.symbian 1.1
 
PageStackWindow { 
    id: rootWindow
 
    property int orientation: PageOrientation.Automatic
 
    showStatusBar: true 
    showToolBar: true
 
    initialPage: FriendsPage {}
 }
