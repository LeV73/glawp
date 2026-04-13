/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "wl/init.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file is responsible for initializing Wayland window"
        ]
    }    
}
*/
#include "wl_app.c"
#include "wl_listeners.c"

struct wl_app
init_app() {
    struct wl_app app;
    app.err = 0;
    app.width = 1360;  // yeah, it's my sloppy laptops' resolution :^)
    app.height = 768;
    app.running = 1;
    app.egl_window = NULL;

    // Init display
    app.display = wl_display_connect(NULL);
    if (!app.display) { app.err = 1; return app; }

    // Init registry, listener and interfaces
    app.registry = wl_display_get_registry(app.display);
    wl_registry_add_listener(app.registry, &registryl, &app);
    wl_display_roundtrip(app.display);

    if (!app.compositor) {app.err = 2; return app;}
    if (!app.layer_shell) {app.err = 3; return app;}

    // Init surfaces
    app.surface = wl_compositor_create_surface(app.compositor);
    if (!app.surface) {app.err = 4; return app;}

    app.layer_surface = zwlr_layer_shell_v1_get_layer_surface(
        app.layer_shell,
        app.surface, 
        NULL, // TODO: add binding to certain monitor
        ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND, 
        "glawp"
    );
    if (!app.layer_surface) {app.err = 5; return app;}

    zwlr_layer_surface_v1_add_listener(
        app.layer_surface, 
        &layer_surfacel, 
        &app
    );
    zwlr_layer_surface_v1_set_anchor(
        app.layer_surface, 
        ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM |
        ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT
    );
    zwlr_layer_surface_v1_set_exclusive_zone(app.layer_surface, -1);
    zwlr_layer_surface_v1_set_keyboard_interactivity(
        app.layer_surface, 
        ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE
    );
    wl_surface_commit(app.surface);
    wl_display_roundtrip(app.display);
    wl_display_roundtrip(app.display);

    return app;
}