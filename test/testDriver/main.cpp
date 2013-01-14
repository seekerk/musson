#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "drivers/common/interface.h"
#include <QPluginLoader>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

void msgHandler( QtMsgType type, const char* msg )
{
    const char symbols[] = { 'I', 'E', '!', 'X' };
    QString output = QString("[%1] %2").arg( symbols[type] ).arg( msg );
    fprintf(stderr, "%s\n", output.toUtf8().data());
    if( type == QtFatalMsg ) abort();
}

int main(int argc, char *argv[])
{
    qInstallMsgHandler(msgHandler);
    QApplication app(argc, argv);

    QDir dir(QApplication::applicationDirPath());

    if (!dir.cd("drivers")) {
        QMessageBox::critical(0, "", "plugins directory does not exist");
        return 1;
    }

    QList<DriverInterface *> drivers;

    foreach (QString strFileName, dir.entryList(QDir::Files)) {
        QPluginLoader loader (dir.absoluteFilePath(strFileName));
        loader.load();
        qDebug() << "load" << dir.absoluteFilePath(strFileName) << "is" << (loader.isLoaded() ? "successful" : "failed");
        if (!loader.isLoaded())
            continue;
        DriverInterface* drv = qobject_cast<DriverInterface*>(loader.instance());

        if (drv == NULL)
            continue;

        qDebug() << "check driver " << drv->getDriverDisplayName();

        drv->checkConnection();

        drivers.append(drv);
    }

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/TestDriver/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
