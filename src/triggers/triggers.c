#include "process.c"
#include "handlers.c"

/* run initial scripts */
void onLoad() {
    sh("./scripts/init.sh");
}

void loadTriggers() {
    /* bind key presses */
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("q")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("p")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("c")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);

    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("j")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("k")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);

    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("Return")), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
            XGrabKey(display, XKeysymToKeycode(display, XK_Shift_L), modkey,
            root, True, GrabModeAsync, GrabModeAsync);
    XSelectInput(display, root, SubstructureRedirectMask | SubstructureNotifyMask);

    /* fill the event table with dummies (nil) */
    for (int i = 0; i < LASTEvent; i++) {
        eventTable[i] = dummy;
    }

    /* add event handlers */
    eventTable[KeyPress] = handleKeyPress;
    eventTable[MapRequest] = handleMapRequest;
    eventTable[MapNotify] = handleMapNotify;
    eventTable[DestroyNotify] = handleDestroyNotify;
    onLoad();
}
