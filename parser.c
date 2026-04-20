
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
        "patch": 1
    },
    "description": [
        "This file handles parsing arguments.",
        "These arguments include (as of v0.0.1):",
        " - -f, --fps - maximum FPS to render",
        " - -v, --version - print version of glawp",
        " - -d, --debug - print logs into console"
        " - PATH - path to GLSL fragment shader"
        ]
    }    
}
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

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
    data.fragShaderPath = NULL;

    int opt;
    char *endptr;
    long fps;

    optind = 1;
    while ((opt = getopt(argc, argv, ":vdf:")) != -1) {
        switch (opt) {
            case 'v':
                printf("glawp < my current version is %s\n", "0.1.1");
                data.ver_flag = 1;
                return data;
            case 'd':
                printf("glawp < debugging this mess...\n");
                data.debug_flag = 1;
                break;
            case 'f':
                fps = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || fps < 1) {
                    printf("glawp < couldn't parse fps, using default 30\n");
                } else {
                    data.max_fps = (int)fps;
                }
                break;
            case ':':
                printf("glawp < option needs a value\n");
                return data;
            case '?':
                printf("glawp < eww, wtf is this option, imma quit real quick...\n");
                return data;
        }
    }

    if (optind < argc) {
        data.fragShaderPath = argv[optind];
    } else {
        printf("glawp < missing fragment shader path\n");
    }

    return data;
}