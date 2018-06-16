#include "dnaicontrols_plugin.h"

#include <QDebug>
#include <qqml.h>
#include <QGuiApplication>
#include "searchablemenu.h"
#include "triangle.h"
#include "colorpicker.h"

void DnaiControlsPlugin::registerTypes(const char *uri)
{
    // @uri com.saltystudio.fontawesomecomponent
    Q_ASSERT(uri == QLatin1String("Dnai.Controls"));
    qmlRegisterType<dnai::controls::SearchableMenu>("Dnai.Controls", 1, 0, "BackendSearchableMenu");
    qmlRegisterType<dnai::controls::Triangle>("Dnai.Controls", 1, 0, "ControlTriangle");
    qmlRegisterType<dnai::controls::ColorPicker>("Dnai.Controls", 1, 0, "ColorPicker");
}

