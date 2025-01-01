#include <miso/app.h>
#include <miso/win.h>
#include <shoyu-shell-gtk3/shoyu-shell-gtk3.h>
#include <flutter_linux/flutter_linux.h>

#include "flutter/generated_plugin_registrant.h"

struct _MisoApplication {
  FlApplication parent_instance;
  gchar** dart_entrypoint_arguments;
  int monitor_added_id;
  int monitor_removed_id;
};

G_DEFINE_TYPE(MisoApplication, miso_application, fl_application_get_type())

static void first_frame_cb(MisoApplication* self, FlView* view) {
  GtkWidget* window = gtk_widget_get_toplevel(GTK_WIDGET(view));

  // Show the main window.
  if (window != nullptr && GTK_IS_WINDOW(window)) {
    gtk_window_present(GTK_WINDOW(window));
  }
}

static void miso_application_monitor_added(GdkDisplay* display, GdkMonitor* monitor, MisoApplication* self) {
  FlView* view = nullptr;
  GList* windows = gtk_application_get_windows(GTK_APPLICATION(self));
  if (windows != nullptr) {
    MisoWindow* win = MISO_WINDOW(g_list_nth_data(windows, 0));
    if (win == nullptr) return;

    FlView* tmp_view = nullptr;
    g_object_get(win, "view", &tmp_view, nullptr);

    view = fl_view_new_for_engine(fl_view_get_engine(tmp_view));
  } else {
    g_autoptr(FlDartProject) project = fl_dart_project_new();
    fl_dart_project_set_dart_entrypoint_arguments(
      project, self->dart_entrypoint_arguments);

    view = fl_view_new(project);
  }

  g_assert(view != nullptr);
  g_signal_connect_swapped(view, "first-frame", G_CALLBACK(first_frame_cb), self);
  gtk_widget_show(GTK_WIDGET(view));

  GtkWindow* window = nullptr;
  g_signal_emit_by_name(self, "create-window", view, &window);
  g_assert(window != nullptr);

  gtk_widget_realize(GTK_WIDGET(view));
  shoyu_shell_gtk_monitor_set_window(monitor, gtk_widget_get_window(GTK_WIDGET(window)));

  g_signal_emit_by_name(self, "register-plugins", FL_PLUGIN_REGISTRY(view));

  if (windows != nullptr) gtk_window_present(window);
}

static void miso_application_monitor_removed(GdkDisplay* display, GdkMonitor* monitor, MisoApplication* self) {}

static void miso_application_register_plugins(FlApplication* app, FlPluginRegistry* registry) {
  (void)app;

  fl_register_plugins(registry);
}

static GtkWindow* miso_application_create_window(FlApplication* app, FlView* view) {
  return miso_window_new(GTK_APPLICATION(app), view);
}

static void miso_application_activate(GApplication* application) {
  MisoApplication* self = MISO_APPLICATION(application);

  gtk_init(NULL, NULL);
  shoyu_shell_gtk_init();

  GdkDisplay* display = gdk_display_get_default();
  self->monitor_added_id = g_signal_connect(display, "monitor-added", G_CALLBACK(miso_application_monitor_added), self);
  self->monitor_removed_id = g_signal_connect(display, "monitor-removed", G_CALLBACK(miso_application_monitor_removed), self);

  for (int i = 0; i < gdk_display_get_n_monitors(display); i++) {
    miso_application_monitor_added(display, gdk_display_get_monitor(display, i), self);
  }
}

static gboolean miso_application_local_command_line(GApplication* application,
                                                gchar*** arguments,
                                                int* exit_status) {
  MisoApplication* self = MISO_APPLICATION(application);

  self->dart_entrypoint_arguments = g_strdupv(*arguments + 1);

  g_autoptr(GError) error = nullptr;
  if (!g_application_register(application, nullptr, &error)) {
    g_warning("Failed to register: %s", error->message);
    *exit_status = 1;
    return TRUE;
  }

  // This will only run on the primary instance or this instance with
  // G_APPLICATION_NON_UNIQUE
  g_application_activate(application);
  *exit_status = 0;

  return TRUE;
}

static void miso_application_dispose(GObject* object) {
  MisoApplication* self = MISO_APPLICATION(object);

  g_clear_pointer(&self->dart_entrypoint_arguments, g_strfreev);

  G_OBJECT_CLASS(miso_application_parent_class)->dispose(object);
}

static void miso_application_class_init(MisoApplicationClass* klass) {
  FL_APPLICATION_CLASS(klass)->register_plugins = miso_application_register_plugins;
  FL_APPLICATION_CLASS(klass)->create_window = miso_application_create_window;

  G_APPLICATION_CLASS(klass)->activate = miso_application_activate;
  G_APPLICATION_CLASS(klass)->local_command_line = miso_application_local_command_line;

  G_OBJECT_CLASS(klass)->dispose = miso_application_dispose;
}

static void miso_application_init(MisoApplication* self) {}

MisoApplication* miso_application_new(const gchar* app_id) {
  return MISO_APPLICATION(g_object_new(miso_application_get_type(),
                                     "application-id", app_id,
                                     "flags", G_APPLICATION_NON_UNIQUE,
                                     nullptr));
}
