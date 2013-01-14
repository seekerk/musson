message(Harmattan build)

DEFINES += Q_OS_HARMATTAN

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

harm_qml.source = qml/Harmattan
harm_qml.target = qml/Musson
DEPLOYMENTFOLDERS = harm_qml

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
