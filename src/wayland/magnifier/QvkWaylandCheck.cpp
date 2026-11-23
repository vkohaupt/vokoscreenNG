#include "QvkWaylandCheck.h"

#include <wayland-client.h>
#include <cstring>

bool QvkWaylandCheck::isWlrLayerShellSupported()
{
    struct wl_display *display = wl_display_connect(nullptr);
    if (!display) {
        return false; // Kein Wayland-Display aktiv (z. B. X11)
    }

    bool supported = false;

    auto handleGlobal = [](void *data, struct wl_registry *, uint32_t,
                           const char *interface, uint32_t) {
        if (std::strcmp(interface, "zwlr_layer_shell_v1") == 0) {
            *static_cast<bool *>(data) = true;
        }
    };

    auto handleGlobalRemove = [](void *, struct wl_registry *, uint32_t) {};

    static const struct wl_registry_listener listener = {
        handleGlobal,
        handleGlobalRemove
    };

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &listener, &supported);
    wl_display_roundtrip(display);

    wl_registry_destroy(registry);
    wl_display_disconnect(display);

    return supported;
}
