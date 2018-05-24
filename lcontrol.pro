TEMPLATE = lib
CONFIG += plugin
QT += qml
QT += quick
CONFIG += c++11

TARGET  = dnaicontrolsplugin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

pluginfiles.files += \
    Header.qml \
    EditableText.qml \
    PanelView.qml


isEmpty(PROJECT_ROOT_DIRECTORY){
  PROJECT_ROOT_DIRECTORY = $$[QT_INSTALL_QML]
}

# message($${PROJECT_ROOT_DIRECTORY})

target.path += $${PROJECT_ROOT_DIRECTORY}/Dnai/Controls
pluginfiles.path += $${PROJECT_ROOT_DIRECTORY}/Dnai/Controls

qmldirsrc.path =  $${PROJECT_ROOT_DIRECTORY}/Dnai/Controls
qmldirsrc.files += \
    qmldir \
    plugins.qmltypes


INSTALLS += target pluginfiles qmldirsrc

CONFIG += install_ok

DISTFILES += \
    Header.qml \
    PanelView.qml \
    qmldir \
    EditableText.qml \
    plugins.qmltypes

HEADERS += \
    dnaicontrols_plugin.h

SOURCES += \
    dnaicontrols_plugin.cpp
