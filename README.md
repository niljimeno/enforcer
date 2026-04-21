# Enforcer
Attempt at making a window manager

![img](thumbnail.png)

> NCMPCPP, Blue Revolver and Kakoune running under Enforcer

## State:
Usable, but still not recommended.
It's a low-quality, column-based DWM skeleton.

Floating and statically sized windows are treated as resizeable.

## Usage
- `Mod-enter`: open a new terminal (st)
- `Mod-p`: open dmenu
- `Mod-c`: close focused window
- `Mod-Shift-c`: close enforcer
- `Mod-j / Mod-k`: change focus
- `Mod-qwertyuio`: change workspace
- `Mod-123456789`: change the focused window's workspace
- `Mod-m / Mod-s`: enable/disable monocle mode

## Testing instructions
Run `make init` to open a Xephyr environment,
then `make run` to build and run the program.
Change the init script at `scripts/init.sh`
to change the background.

----------------

## Todo:
- [x] Shift+workspaceKey to move node
- [x] Handle map/unmap/destroy requests *globally* to avoid leftovers
- [x] Fullscreen mode (monocle)
- [/] Properly handle floating and popups (they often appear cut)
