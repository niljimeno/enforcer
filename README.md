# Enforcer
Attempt at making a window manager,
inspired by SXWM, Vox-WM, and Ratpoison.

## State:
Incredibly early version. Practically unusable.

## Testing instructions
Run `make init` to open a Xephyr environment,
then `make run` to build and run the program.
Change the init script at `scripts/init.sh`
to change the background.

## Usage
- `Mod-enter`: open a new terminal (st)
- `Mod-c`: close focused window
- `Mod-Shift-Q`: close enforcer
- `Mod-j / Mod-k`: change focus


Wishlist:
- Create/remove workspaces and windows at will
- Modify the amount of windows shown (like mod+i in dwm) and scroll windows horizontally (like ratpoison)
- Global floating centered terminal (required)
- Let st and dmenu be the only default application binds. Everything else shall be hooks
- Non-immediate keybind execution like Ratpoison
- Mod+Tab to open hacker screen (I still not know what to use it for)
