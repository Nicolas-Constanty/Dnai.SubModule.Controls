#include "dnaicontrols_plugin.h"

#include <QDebug>
#include <qqml.h>
#include <QGuiApplication>
#include "searchablemenu.h"

void DnaiControlsPlugin::registerTypes(const char *uri)
{
    // @uri com.saltystudio.fontawesomecomponent
    Q_ASSERT(uri == QLatin1String("Dnai.Controls"));
    qmlRegisterType<dnai::controls::SearchableMenu>("Dnai.Controls", 1, 0, "BackendSearchableMenu");
}

