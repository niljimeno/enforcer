#include "process.c"
#include "handlers.c"

void grab(KeySym key) {
    XGrabKey(display, XKeysymToKeycode(display, key), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
}

void loadTriggers() {
    KeySym keys[] = {
        XK_Q,
        XK_W,
        XK_E,
        XK_R,
        XK_T,
        XK_Y,
        XK_U,
        XK_I,
        XK_O,
        XK_P,
        XK_C,
        XK_J,
        XK_K,
        XK_M,
        XK_S,
        XK_H,
        XK_Return,
        XK_Shift_L
    };

    /* grab keys */
    for (int i=0; i<(sizeof(keys)/sizeof(KeySym)); i++) {
        XGrabKey(display, XKeysymToKeycode(display, keys[i]), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    }

    XSelectInput(display, root, SubstructureRedirectMask | SubstructureNotifyMask);

    /* fill the event table with dummies (nil) */
    for (int i = 0; i < LASTEvent; i++) {
        eventTable[i] = dummy;
    }

    /* add event handlers */
    eventTable[KeyPress] = handleKeyPress;
    eventTable[MapRequest] = handleMapRequest;
    eventTable[MapNotify] = handleMapNotify;
    eventTable[UnmapNotify] = handleUnmapNotify;
    eventTable[DestroyNotify] = handleDestroyNotify;
}
