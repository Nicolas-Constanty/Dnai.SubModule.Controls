#include "dnaicontrols_plugin.h"

#include <QDebug>
#include <qqml.h>

void DnaiControlsPlugin::registerTypes(const char *uri)
{
    // @uri com.saltystudio.fontawesomecomponent
    Q_ASSERT(uri == QLatin1String("Dnai.Controls"));
}

