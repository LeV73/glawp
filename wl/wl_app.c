/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "wl/wl_app.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "File with main headers and wl_app struct",
        "Basically everything is defined here about this program"
        ]
    }    
}
*/
#pragma once
#include "layer-shell.h"

#include <wayland-egl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

struct wl_app {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_callback *frame_callback;
    struct zwlr_layer_shell_v1 *layer_shell;
    
    struct wl_surface *surface;
    struct zwlr_layer_surface_v1 *layer_surface;
    
    struct wl_egl_window *egl_window;
    EGLDisplay egl_display;
    EGLConfig egl_config;
    EGLContext egl_context;
    EGLSurface egl_surface;

    GLuint program;
    GLuint vbo;
    GLint attr_pos;
    GLint uni_time;
    GLint uni_res;

    int err;
    int layer_surface_configured;
    int running;
    int configured;
    int redraw;
    
    int width;
    int height;
    int buffer_scale;
};