import 'package:flutter_test/flutter_test.dart';
import 'package:miso/miso.dart';
import 'package:miso/miso_platform_interface.dart';
import 'package:miso/miso_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockMisoPlatform
    with MockPlatformInterfaceMixin
    implements MisoPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final MisoPlatform initialPlatform = MisoPlatform.instance;

  test('$MethodChannelMiso is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelMiso>());
  });

  test('getPlatformVersion', () async {
    Miso misoPlugin = Miso();
    MockMisoPlatform fakePlatform = MockMisoPlatform();
    MisoPlatform.instance = fakePlatform;

    expect(await misoPlugin.getPlatformVersion(), '42');
  });
}
