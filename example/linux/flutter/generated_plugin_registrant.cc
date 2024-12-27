//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <miso/miso_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) miso_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "MisoPlugin");
  miso_plugin_register_with_registrar(miso_registrar);
}
