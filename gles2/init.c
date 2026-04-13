/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "gles2/init.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file has initialization function for GLES2",
        "Can probably expand for more backends",
        "(Vulkan can be more flexible and efficient)"
        ]
    }    
}
*/
#include "../wl/wl_app.c"
#include <stdio.h>
#include <stdlib.h>

// This part is sloppinated, idc really, 
// should've been a library to read files
char*
get_fragment_shader(char* fragmentShaderPath) {
    FILE *file = fopen(fragmentShaderPath, "rb");
    if (!file) {
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size < 0) {
        fclose(file);
        return NULL;
    }

    // Allocate memory for file contents + null terminator
    char *buffer = (char*)malloc((size_t)size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    // Read file into buffer
    size_t readSize = fread(buffer, 1, (size_t)size, file);
    buffer[readSize] = '\0';  // null-terminate

    fclose(file);

    return buffer;
}

// builtin for fullscreen quad
static const char* vertex_shader_code = 
"attribute vec2 pos;\n"
"varying vec2 v_uv;\n"
"void main() {\n"
"    v_uv = (pos + 1.0) * 0.5;\n"
"    gl_Position = vec4(pos, 0.0, 1.0);\n"
"}\n";

GLuint
compile_shader_gles2(GLenum type, const char *srcode) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &srcode, NULL);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[4096];
        GLsizei len = 0;
        glGetShaderInfoLog(shader, sizeof(log), &len, log);
        fprintf(stderr, "glawp < shader compile error: %s\n", log);
        return -1;
    }
    return shader;
}

void
init_gles2(struct wl_app *app, char* fragShaderPath) {
    char* fragment_shader_code = get_fragment_shader(fragShaderPath);
    if (fragment_shader_code == NULL) { free(fragment_shader_code); app->err = 14; return; }
    
    GLuint vs = compile_shader_gles2(GL_VERTEX_SHADER, vertex_shader_code);
    if (vs == -1) { free(fragment_shader_code); app->err = 15; return; }
    
    GLuint fs = compile_shader_gles2(GL_FRAGMENT_SHADER, fragment_shader_code);
    if (fs == -1) { glDeleteShader(vs); free(fragment_shader_code); app->err = 16; return; }
    
    app->program = glCreateProgram();
    glAttachShader(app->program, vs);
    glAttachShader(app->program, fs);
    glBindAttribLocation(app->program, 0, "pos");
    glLinkProgram(app->program);

    GLint ok = GL_FALSE;
    glGetProgramiv(app->program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[4096];
        GLsizei len = 0;
        glGetProgramInfoLog(app->program, sizeof(log), &len, log);
        fprintf(stderr, "glawp < program link error: %s\n", log);
        app->err = 17;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    free(fragment_shader_code);
    if (app->err == 17) { return; }

    const GLfloat verts[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    glGenBuffers(1, &app->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    
    app->attr_pos = 0;
    app->uni_time = glGetUniformLocation(app->program, "u_time");
    app->uni_res  = glGetUniformLocation(app->program, "u_res");

    glUseProgram(app->program);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glEnableVertexAttribArray((GLuint)app->attr_pos);
    glVertexAttribPointer((GLuint)app->attr_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    return;
}