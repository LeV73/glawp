# GLAWP👽
OpenGL® Awesome WallPapers is Wayland Wallpaper *"engine"* for running shaders as wallpapers.

# ⚠️HEAVILY WORK IN PROGRESS⚠️
I've yet to grasp innerworkings of Wayland and EGL®/OpenGL®|ES (and program options) to call this any **good** or optimized.  
(One of the issues right now is high cpu usage with low fps options)  
One thing for sure is that it works (on my machine;)

## Requirements
 * Wayland compositor with `wlr-layer-shell` support.
   * Check support at https://wayland.app/protocols/wlr-layer-shell-unstable-v1
 * GPU with OpenGL®|ES 2.0 support (any, basically)
### Dependencies
Fedora
```sh
sudo dnf install wayland-devel wlr-protocols-devel wayland-protocols-devel mesa-libEGL-devel
```
### How 2 Build
 1. Clone Repo with `git clone`
 2. `cd` to Repo folder (glawp)
 3. Run `make protocols`
 4. Run `make`  

Binary will be built in `build` dir, just run `./build/glawp_gdb ./shaders/opticircle.glsl`

Program takes `-f {fps}` option to set max fps to render at (from 1 to 8999), `-v` for version and `-d` for debug (not implemented).

# Acknowledgements
 * OpenGL® and EGL® are trademarks or registered trademarks of Hewlett Packard Enterprise in the United States and/or other countries worldwide.  
 * Shader for testing is shamelessly taken from https://www.shadertoy.com/view/XsXXDn and was created by Danilo Guanabara.
