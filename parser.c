
/*
{
"project": "openGL Amazing WallPapers",
"license": "GPLv3",
"author": "Lev B. (LeV73)",
"file": {
    "name": "parser.c",
    "version": {
        "major": 0,
        "minor": 1,
        "patch": 0
    },
    "description": [
        "This file handles parsing arguments.",
        "These arguments include (as of v0.0.1):",
        " - -f, --fps - maximum FPS to render",
        " - -v, --version - print version of glawp",
        " - -d, --debug - print logs into console (NOT IMPL.)"
        " - PATH - path to GLSL fragment shader"
        ]
    }    
}
*/
#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct parsedData {
    int ver_flag;
    int debug_flag;
    int max_fps;
    char* fragShaderPath;
};

struct parsedData
parse(int argc, char **argv) {
    struct parsedData data;
    data.max_fps = 30;
    data.ver_flag = 0;
    data.debug_flag = 0;

    int parsed_fps;
    int opt;
    while (opt != -1) {
        opt = getopt(argc, argv, ":vdf:");
        //printf("opt = %i\n", opt);
        switch (opt) {
            case 'v':
                printf("glawp < my current version is %s\n", "0.1.0");
                data.ver_flag = 1;
                return data;
            case 'd':
                printf("glawp < I DON'T DEBUG, I'M HARDCORE!!!!!\n");
                data.debug_flag = 1;
                return data;
            case 'f':
                parsed_fps = atoi(optarg);
                if (parsed_fps < 1) {
                    printf("glawp < couldn't parse fps, using default 30\n");
                } else {
                    printf("glawp < parsed fps = %i\n", parsed_fps);
                    data.max_fps = parsed_fps;
                }
                break;
            case ':': 
                printf("option needs a value\n"); 
                data.ver_flag = 1;
                return data;
            case '?':
                printf("glawp < eww, wtf is this option, imma quit real quick...\n");
                data.ver_flag = 1;
                return data;
        }
    }
    data.fragShaderPath = argv[argc - 1];
    return data;
}