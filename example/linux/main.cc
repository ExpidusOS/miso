#include <miso/miso.h>

int main(int argc, char** argv) {
  g_autoptr(MisoApplication) app = miso_application_new(APPLICATION_ID);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
