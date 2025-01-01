#pragma once

#include <flutter_linux/flutter_linux.h>

struct _MisoApplication {
  FlApplication parent_instance;
};

G_DECLARE_FINAL_TYPE(MisoApplication, miso_application, MISO, APPLICATION, FlApplication)

/**
 * miso_application_new:
 *
 * Creates a new Flutter-based application.
 *
 * Returns: a new #MisoApplication.
 */
MisoApplication* miso_application_new(const gchar* app_id);
