#include "kaleidospect_plugin.h"
#include "starburst.h"

#include <qqml.h>

void KaleidospectPlugin::registerTypes(const char *uri)
{
    // @uri org.ecloud.kaleidospect
    qmlRegisterType<Starburst>(uri, 1, 0, "Starburst");
}

