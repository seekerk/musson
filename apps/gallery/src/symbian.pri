message(Symbian build)

TARGET.UID3 = 0xEDED3208

# Allow network access on Symbian
TARGET.CAPABILITY += NetworkServices

# Add dependency to Symbian components
# CONFIG += qt-components

qml_files.source = qml/Symbian
qml_files.target = qml/Musson
DEPLOYMENTFOLDERS = qml_files

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
