#pragma once

#include <gtk/gtk.h>
#include <flutter_linux/flutter_linux.h>

G_DECLARE_FINAL_TYPE(MisoWindow, miso_window, MISO, WINDOW, GtkApplicationWindow);

GtkWindow* miso_window_new(GtkApplication* app, FlView* view);
