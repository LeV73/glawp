/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "slay.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file is responsable for safely destroying program"
        ]
    }    
}
*/
#include "wl/layer-shell.h"
#include "wl/wl_app.c"

int
slay_app(struct wl_app *app) {
    if (app->err == 0) app->err = 99;

    if (app->err > 17) glDeleteProgram(app->program);
    // 16 is failure to compile fragment shader
    // 15 is failure to compile vertex shader
    // 14 is failure to load fragment shader from file
    if (app->err > 13) eglMakeCurrent(app->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (app->err > 12) eglDestroySurface(app->egl_display, app->egl_surface);
    if (app->err > 11) wl_egl_window_destroy(app->egl_window);
    if (app->err > 10) eglDestroyContext(app->egl_display, app->egl_context);
    // 9 is faliure to choose egl config
    // 8 is failure to initialize egl 
    if (app->err > 7) eglTerminate(app->egl_display);
    if (app->err > 6) zwlr_layer_surface_v1_destroy(app->layer_surface);
    if (app->err > 5) wl_surface_destroy(app->surface);
    if (app->err > 4) zwlr_layer_shell_v1_destroy(app->layer_shell);
    if (app->err > 3) wl_compositor_destroy(app->compositor);
    if (app->err > 2) wl_registry_destroy(app->registry);
    if (app->err > 1) wl_display_disconnect(app->display);
    if (app->err == 99) return 0;
    else return app->err;
}