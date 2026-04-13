/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "main.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file is an entrypoint for a program,",
        "and is intended to have a single function 'main'.",
        "All other needed includes and functions are located in",
        "other files, with their name indicating their purpose.",
        "'main' doesn't handle anything, it just passes",
        "function calls to other functions in files."
        ]
    }    
}
*/
#include "parser.c"
#include "wl/init.c"
#include "egl/init.c"
#include "gles2/init.c"
#include "main_loop.c"
#include "slay.c"

int 
main(int argc, char **argv) {
    struct parsedData data = parse(argc, argv);
    if (data.ver_flag || data.debug_flag) return 0;

    struct wl_app main_app = init_app();
    if (main_app.err != 0) {
        return slay_app(&main_app);
    }
    init_egl(&main_app);
    if (main_app.err != 0) {
        return slay_app(&main_app);
    }
    init_gles2(&main_app,data.fragShaderPath);
    if (main_app.err != 0) {
        return slay_app(&main_app);
    }
    main_loop(&main_app, &data);
    return slay_app(&main_app);
}