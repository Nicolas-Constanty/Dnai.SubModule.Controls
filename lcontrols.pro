TEMPLATE = lib
CONFIG += plugin
QT += qml
QT += quick
CONFIG += c++11

RESOURCES += qml.qrc

TARGET  = dnaicontrolsplugin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG(release, debug|release) {
DEFINES += QT_NO_DEBUG_OUTPUT
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

pluginfiles.files += \
    Header.qml \
    PanelView.qml \
    SearchableMenu.qml \
    ColorWheel.qml


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
    plugins.qmltypes \
    SearchableMenu.qml \
    BaseMenu.qml \
    BaseAction.qml \
    ColorWheel.qml

HEADERS += \
    dnaicontrols_plugin.h \
    searchablemenu.h \
    fuzzymatch.h \
    triangle.h \
    colorpicker.h \
    callbackincubator.h

SOURCES += \
    dnaicontrols_plugin.cpp \
    searchablemenu.cpp \
    triangle.cpp \
    colorpicker.cpp \
    callbackincubator.cpp
