#include <QtGui/QApplication>
#include <QtDeclarative>

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR) || defined (Q_OS_HARMATTAN)
#include "qmlapplicationviewer.h"
#else
#include "qmldesktopviewer/qmldesktopviewer.h"
#endif

#ifdef TARGET
    #define APP_NAME TARGET
#else
    #define APP_NAME "musson-gallery"
#endif



Q_DECL_EXPORT int main(int argc, char *argv[])
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR) || defined (Q_OS_HARMATTAN)
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    app->setApplicationName(APP_NAME);
    app->setOrganizationName("PetrSU");
    app->setOrganizationDomain("cs.karelia.ru");
#else
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setOrganizationName("PetrSU");
    app.setOrganizationDomain("cs.karelia.ru");
#endif

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    // Symbian and Simulator
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    viewer.setMainQmlFile(QLatin1String("qml/Symbian/main.qml"));

    viewer.showExpanded();
#elif defined Q_OS_HARMATTAN
    // Harmattan
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    viewer.setMainQmlFile(QLatin1String("qml/Harmattan/main.qml"));

    viewer.showExpanded();
#else
    // Desktop
    QmlDesktopViewer *view = new QmlDesktopViewer();

    view->engine()->addImportPath(QString(DATAPREFIX) + "/musson");
    view->engine()->addPluginPath(PLUGINPREFIX);

    view->open(QString(QMLDIR) + "/main.qml");
    //QObject::connect(app.data(), SIGNAL(lastWindowClosed()), viewer, SLOT(quit()));
#endif

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR) || defined (Q_OS_HARMATTAN)
    return app->exec();
#else
    return app.exec();
#endif
}
