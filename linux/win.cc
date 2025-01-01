#include <miso/win.h>
#include <flutter_linux/flutter_linux.h>
#include <shoyu-shell-gtk3/shoyu-shell-gtk3.h>

struct _MisoWindow {
  GtkApplicationWindow parent_instance;
  FlView* view;
  GdkMonitor* monitor;
};

G_DEFINE_TYPE(MisoWindow, miso_window, GTK_TYPE_APPLICATION_WINDOW)

enum {
  PROP_0,
  PROP_VIEW,
  PROP_MONITOR,
  N_PROPS,
};

static GParamSpec* obj_props[N_PROPS] = { nullptr };

static void miso_window_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  MisoWindow* self = MISO_WINDOW(object);

  switch (prop_id) {
    case PROP_VIEW:
      self->view = FL_VIEW(g_value_dup_object(value));
      break;
    case PROP_MONITOR:
      self->monitor = GDK_MONITOR(g_value_dup_object(value));
      if (self->monitor != nullptr)
        shoyu_shell_gtk_monitor_set_window(self->monitor, gtk_widget_get_window(GTK_WIDGET(self)));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void miso_window_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  MisoWindow* self = MISO_WINDOW(object);

  switch (prop_id) {
    case PROP_VIEW:
      g_value_set_object(value, G_OBJECT(self->view));
      break;
    case PROP_MONITOR:
      g_value_set_object(value, G_OBJECT(self->monitor));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void miso_window_constructed(GObject* object) {
  G_OBJECT_CLASS(miso_window_parent_class)->constructed(object);

  MisoWindow* self = MISO_WINDOW(object);

  gtk_widget_show(GTK_WIDGET(self->view));
  gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(self->view));

  GdkRGBA background_color;
  gdk_rgba_parse(&background_color, "#00000000");
  fl_view_set_background_color(self->view, &background_color);

  gtk_widget_map(GTK_WIDGET(self->view));
  gtk_widget_grab_focus(GTK_WIDGET(self->view));

  gtk_window_set_decorated(GTK_WINDOW(self), FALSE);
}

static void miso_window_dispose(GObject* object) {
  MisoWindow* self = MISO_WINDOW(object);

  g_clear_object(&self->view);
  g_clear_object(&self->monitor);

  G_OBJECT_CLASS(miso_window_parent_class)->dispose(object);
}

static void miso_window_class_init(MisoWindowClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->set_property = miso_window_set_property;
  object_class->get_property = miso_window_get_property;
  object_class->constructed = miso_window_constructed;
  object_class->dispose = miso_window_dispose;

  obj_props[PROP_VIEW] = g_param_spec_object(
    "view",
    "View",
    "The Flutter view which this window uses.",
    fl_view_get_type(),
    (GParamFlags)(G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE)
  );

  obj_props[PROP_MONITOR] = g_param_spec_object(
    "monitor",
    "Monitor",
    "The monitor the window is displaying on.",
    GDK_TYPE_MONITOR,
    (GParamFlags)(G_PARAM_READWRITE)
  );

  g_object_class_install_properties(object_class, N_PROPS, obj_props);
}

static void miso_window_init(MisoWindow* self) {}

GtkWindow* miso_window_new(GtkApplication* app, FlView* view) {
  return GTK_WINDOW(g_object_new(miso_window_get_type(), "application", app, "view", view, nullptr));
}
