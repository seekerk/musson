#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "vkdriver.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    VKdriver dr;
    dr.checkConnection();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/TestDriver/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
