/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "wl/wl_listner.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "File simply contains functions for Wayland listners"
        ]
    }    
}
*/
#pragma once
#include <string.h>

#include "layer-shell.h"
#include "wl_app.c"

// Registry listeners
void 
registryglob(
void *data, struct wl_registry *wl_registry, 
uint32_t name, const char *interface, uint32_t version) {
    struct wl_app *app = data;

    if (!strcmp(interface, wl_compositor_interface.name)) {
        app->compositor = wl_registry_bind(wl_registry, name, 
            &wl_compositor_interface, version);
    
    } else if (!strcmp(interface, zwlr_layer_shell_v1_interface.name)) {
        app->layer_shell = wl_registry_bind(wl_registry, name, 
            &zwlr_layer_shell_v1_interface, version);

    }
}
void
registryglobrm(void *data, struct wl_registry *wl_registry, uint32_t name) {
    (void)data; (void)wl_registry; (void)name;
}
struct wl_registry_listener registryl = {
    .global = registryglob,
    .global_remove = registryglobrm
};


// Layer shell listener
void layer_surfaceconf(
void *data, struct zwlr_layer_surface_v1 *zwlr_layer_surface_v1,
uint32_t serial, uint32_t width, uint32_t height) {
    zwlr_layer_surface_v1_ack_configure(zwlr_layer_surface_v1, serial);
    struct wl_app *app = data;

    if (width > 0) app->width = (int)width;
    if (height > 0) app->height = (int)height;

    if (app->egl_window) {
        wl_egl_window_resize(
            app->egl_window, 
            app->width, 
            app->height, 
            0, 0
        );
    }
    app->layer_surface_configured = 1;
}
void layer_surfaceclose(
void *data, struct zwlr_layer_surface_v1 *zwlr_layer_surface_v1) {
    (void) zwlr_layer_surface_v1;
    struct wl_app *app = data;
    app->running = 0;
}
struct zwlr_layer_surface_v1_listener layer_surfacel = {
    .configure = layer_surfaceconf,
    .closed = layer_surfaceclose
};

// Frame callback listner
static void frame_done(void *data, struct wl_callback *callback, uint32_t time_ms) {
    struct wl_app *app = data;
    wl_callback_destroy(callback);
    app->frame_callback = NULL;
    app->redraw = 1;
    (void)time_ms;
}

static const struct wl_callback_listener frame_callbackl = {
    .done = frame_done
};