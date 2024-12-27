#ifndef FLUTTER_PLUGIN_MISO_PLUGIN_H_
#define FLUTTER_PLUGIN_MISO_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace miso {

class MisoPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  MisoPlugin();

  virtual ~MisoPlugin();

  // Disallow copy and assign.
  MisoPlugin(const MisoPlugin&) = delete;
  MisoPlugin& operator=(const MisoPlugin&) = delete;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace miso

#endif  // FLUTTER_PLUGIN_MISO_PLUGIN_H_
