#include "include/miso/miso_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "miso_plugin.h"

void MisoPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  miso::MisoPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
