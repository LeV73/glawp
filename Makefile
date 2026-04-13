CC=cc
CFLAGS=
DFLAGS=-ggdb
LIBS=-lwayland-client -lwayland-egl -lEGL -lGLESv2
RELEASE_EXEC=build/glawp
DEBUG_EXEC=build/glawp_gdb

LAYER_SHELL=wl/layer-shell.c
XDG_SHELL=wl/xdg-shell.c
WL_SHELLS=$(LAYER_SHELL) $(XDG_SHELL)

debug:
	$(CC) $(DFLAGS) $(CFLAGS) $(LIBS) -o $(DEBUG_EXEC) $(WL_SHELLS) main.c

protocols:
	wayland-scanner client-header /usr/share/wlr-protocols/unstable/wlr-layer-shell-unstable-v1.xml wl/layer-shell.h
	wayland-scanner private-code /usr/share/wlr-protocols/unstable/wlr-layer-shell-unstable-v1.xml wl/layer-shell.c
	wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml wl/xdg-shell.h
	wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml wl/xdg-shell.c