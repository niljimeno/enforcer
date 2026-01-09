EventHandler eventTable[LASTEvent];

void dummy(XEvent *ev) {}

void handleKeyPress(XEvent *ev) {
    KeyCode code = ev->xkey.keycode;
    if (code == XKeysymToKeycode(display, XStringToKeysym("Return"))) {
        spawnDaemon("st");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("q")) && ev->xkey.state & ShiftMask) {
        spawnDaemon("pkill -f xinit");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("p"))) {
        sh("dmenu_run");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("c"))) {
        closeFocusedWindow();
    }
}

/* before mapping the window */
void handleMapRequest(XEvent *ev) {
    XSync(display, False);
    Window w = ev->xmaprequest.window;
    if (nodeExists(w)) return;

    struct WindowData winData = createWindow(w);
    if (winData.shouldFloat) {
        XMapWindow(display, w);
        return;
    }

    resizeWindows();
    XMapWindow(display, w);
    XSetInputFocus(display, w, RevertToPointerRoot, CurrentTime);
}

/* after mapping the window */
void handleMapNotify(XEvent *ev) {
}

void handleDestroyNotify(XEvent *ev) {
    XSync(display, False);
    Window w = ev->xdestroywindow.window;
    if (w && nodeExists(w)) {
        removeNode(w);
        restoreWorkspace();
    }
}
