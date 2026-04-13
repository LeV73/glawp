/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "main_loop.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file contains main_loop function, heart of a program"
        ]
    }    
}
*/
#include "wl/wl_app.c"
#include "wl/wl_listeners.c"
#include "parser.c"
#include <time.h>

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void
render(struct wl_app *app, double time) {
    glViewport(0, 0, app->width, app->height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(app->program);
    glUniform1f(app->uni_time, (GLfloat)time);
    glUniform2f(app->uni_res, (GLfloat)app->width, (GLfloat)app->height);

    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glEnableVertexAttribArray((GLuint)app->attr_pos);
    glVertexAttribPointer((GLuint)app->attr_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    eglSwapBuffers(app->egl_display, app->egl_surface);
}

void
schedule_frame(struct wl_app *app) {
    if (app->frame_callback)
        return;

    app->frame_callback = wl_surface_frame(app->surface);
    wl_callback_add_listener(app->frame_callback, &frame_callbackl, app);
    wl_surface_commit(app->surface);
}

void
main_loop(struct wl_app *app, struct parsedData *parsed) {
    app->redraw = 1;
    double start = now_sec();
    double target_dt = 1.0 / (double)parsed->max_fps;
    double next_frame_time = start;
    
    while (app->running) {
        wl_display_dispatch_pending(app->display);

        double now = now_sec();
        if (app->configured && now >= next_frame_time) {
            render(app, now - start);
            next_frame_time += target_dt;
            if (next_frame_time < now)
                next_frame_time = now + target_dt;
        }
    }
}