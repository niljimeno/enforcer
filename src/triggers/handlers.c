EventHandler eventTable[LASTEvent];

void dummy(XEvent *ev) {}

void handleKeyPress(XEvent *ev) {
    printf("Initiate: handle keypress\n");
    KeyCode code = ev->xkey.keycode;
    if (code == XKeysymToKeycode(display, XStringToKeysym("Return"))) {
        spawnDaemon("st");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("c")) && ev->xkey.state & ShiftMask) {
        spawnDaemon("pkill -f xinit");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("p"))) {
        sh("dmenu_run");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("c"))) {
        closeFocusedWindow();
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("j"))) {
        changeFocus(+1);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("k"))) {
        changeFocus(-1);
    }
    printf("Terminate: handle keypress\n");
}

/* before mapping the window */
void handleMapRequest(XEvent *ev) {
    printf("Initiate: handle map request\n");
    Window w = ev->xmaprequest.window;
    if (nodeExists(w)) return;

    struct WindowData winData = createWindow(w);
    if (winData.shouldFloat) {
        XMapWindow(display, w);
        return;
    }

    resizeWindows();
    XMapWindow(display, w);
    focusWindow(w);
    printf("Terminate: handle map request\n");
}

/* after mapping the window */
void handleMapNotify(XEvent *ev) {
    printf("*Initiate: handle map notify\n");
    printf("*Terminate: handle map notify\n");
}

void handleDestroyNotify(XEvent *ev) {
    printf("Initiate: handle destroy notify\n");
    Window w = ev->xdestroywindow.window;
    if (w && nodeExists(w)) {
        removeNode(w);
        restoreWorkspace();
    }

    if (workspace.node == NULL)
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);

    printf("Terminate: handle destroy notify\n");
}
