void dummy(XEvent *ev);
void actKey(XEvent *ev);

error setup() {
    if(!(display = XOpenDisplay(0x0))) return 1;
    root = DefaultRootWindow(display);

    WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    WM_PROTOCOLS =  XInternAtom(display, "WM_PROTOCOLS", False);

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

    for (int i = 0; i < LASTEvent; i++) {
        eventTable[i] = dummy;
    }

    eventTable[KeyPress] = actKey;
    eventTable[MapNotify] = handle_map_notify;
    eventTable[DestroyNotify] = hdl_destroy;

	setWindowTypes(display);

    return 0;
}

void run_cmd(char *cmd) {
    if (fork() == 0) {
        execvp(cmd, (char *[]){cmd, NULL});
    }
}

void spawn_daemon(char *command) {
    popen(command, "r");
}

void actKey(XEvent *ev) {
    KeyCode code = ev->xkey.keycode;
    if (code == XKeysymToKeycode(display, XStringToKeysym("Return"))) {
        spawn_daemon("st");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("q")) && ev->xkey.state & ShiftMask) {
        spawn_daemon("pkill -f xinit");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("p"))) {
        run_cmd("dmenu_run");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("c"))) {
        closeWindow(ev->xkey.subwindow);
    }
}

void dummy(XEvent *ev) {}
