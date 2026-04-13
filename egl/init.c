/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "egl/init.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file is for initializing EGL. Windowing stuff."
        ]
    }    
}
*/
#include "../wl/wl_app.c"

const EGLint attrib_list[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_NONE
};

const EGLint ctx_attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};

void
init_egl(struct wl_app *app) {
    app->egl_display = eglGetDisplay((EGLNativeDisplayType)app->display);
    if (app->egl_display == EGL_NO_DISPLAY) { app->err = 7; return; }

    EGLBoolean eglInit = eglInitialize(app->egl_display, NULL, NULL);
    if (!eglInit) { app->err = 8; return; }

    EGLint num = 0;
    EGLBoolean eglConf = eglChooseConfig(app->egl_display, attrib_list, 
        &app->egl_config, 1, &num
    );
    if (!eglConf) { app->err = 9; return; }

    app->egl_context = eglCreateContext(app->egl_display, 
        app->egl_config, EGL_NO_CONTEXT, ctx_attribs);
    if (app->egl_context == EGL_NO_CONTEXT) { app->err = 10; return; }

    app->egl_window = wl_egl_window_create(
        app->surface, 
        app->width, app->height
    );
    if (!app->egl_window) { app->err = 11; return; }

    app->egl_surface = eglCreateWindowSurface(app->egl_display, 
        app->egl_config, (EGLNativeWindowType)app->egl_window, NULL
    );
    if (app->egl_surface == EGL_NO_SURFACE) { app->err = 12; return; }

    EGLBoolean eglCurr = eglMakeCurrent(app->egl_display, app->egl_surface, 
        app->egl_surface, app->egl_context);
    if (!eglCurr) { app->err = 13; return; }
    return;
}