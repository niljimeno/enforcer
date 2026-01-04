#include "process.c"
#include "handlers.c"

void bindTriggers() {
    /* bind key presses */
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("q")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("p")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("c")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("Return")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
            XGrabKey(display, XKeysymToKeycode(display, XK_Shift_L), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XSelectInput(display, root, SubstructureNotifyMask);

    /* fill the event table with dummies (nil) */
    for (int i = 0; i < LASTEvent; i++) {
        eventTable[i] = dummy;
    }

    /* add event handlers */
    eventTable[KeyPress] = handleKeyPress;
    eventTable[MapNotify] = handleMapNotify;
}

/*
example event:
void hdl_destroy(XEvent *ev) {
    spawnDaemon("redshift -O 3000 -P");
}
*/
