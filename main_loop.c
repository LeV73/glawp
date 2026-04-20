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
        "patch": 1
    },
    "description": [
        "This file contains main_loop function, heart of a program"
        ]
    }    
}
*/
#include "wl/wl_app.c"
#include "parser.c"
#include <time.h>
#include <unistd.h>

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

// Compute iDate uniform from current time
static void
get_iDate(GLfloat* date) {
    time_t now = time(NULL);
    struct tm* timeinfo = gmtime(&now);

    // iDate = (year, month (0-11), day, seconds in day)
    date[0] = (GLfloat)(1900 + timeinfo->tm_year);
    date[1] = (GLfloat)timeinfo->tm_mon;
    date[2] = (GLfloat)timeinfo->tm_mday;
    date[3] = (GLfloat)(timeinfo->tm_hour * 3600 + timeinfo->tm_min * 60 + timeinfo->tm_sec);
}

void
render(struct wl_app *app, double time) {
    glViewport(0, 0, app->width, app->height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(app->program);

    // ShaderToy uniforms
    double timeDelta = time - app->last_frame_time;
    app->last_frame_time = time;

    glUniform3f(app->uni_iResolution, (GLfloat)app->width, (GLfloat)app->height, 0.0f);
    glUniform1f(app->uni_iTime, (GLfloat)time);
    glUniform1f(app->uni_iTimeDelta, (GLfloat)timeDelta);
    glUniform1f(app->uni_iFrame, (GLfloat)app->frame_count);
    glUniform4f(app->uni_iMouse, 0.0f, 0.0f, 0.0f, 0.0f);

    GLfloat date[4];
    get_iDate(date);
    glUniform4fv(app->uni_iDate, 1, date);

    app->frame_count++;

    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glEnableVertexAttribArray((GLuint)app->attr_pos);
    glVertexAttribPointer((GLuint)app->attr_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    eglSwapBuffers(app->egl_display, app->egl_surface);
}

void
main_loop(struct wl_app *app, struct parsedData *parsed) {
    app->redraw = 1;

    // Detect if max_fps = 0 and render image just once
    // BUG(?): if max_fps = 1 -> sets itself to displays' refresh rate
    if (parsed->max_fps == 0) {
        wl_display_dispatch_pending(app->display);
        render(app, 1.0f);
        wl_display_dispatch(app->display);
        while (app->running) {
            sleep(UINT32_MAX);
        }
    } else {
        struct timespec frame_sleep;
        frame_sleep.tv_nsec = 1.0 / (float)parsed->max_fps * 1000000000.0;
        frame_sleep.tv_sec = 0;

        double start = now_sec();

        while (app->running) {
            nanosleep(&frame_sleep, NULL);
            double now = now_sec();
            wl_display_dispatch_pending(app->display);
            render(app, now - start);
            wl_display_dispatch(app->display);
        }
    }
}