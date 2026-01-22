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

    } else if (code == XKeysymToKeycode(display, XStringToKeysym("q")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(0);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("w")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(1);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("e")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(2);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("r")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(3);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("t")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(4);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("y")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(5);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("u")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(6);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("i")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(7);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("o")) && ev->xkey.state & ShiftMask) {
        moveToWorkspace(8);

    } else if (code == XKeysymToKeycode(display, XStringToKeysym("q"))) {
        changeWorkspace(0);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("w"))) {
        changeWorkspace(1);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("e"))) {
        changeWorkspace(2);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("r"))) {
        changeWorkspace(3);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("t"))) {
        changeWorkspace(4);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("y"))) {
        changeWorkspace(5);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("u"))) {
        changeWorkspace(6);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("i"))) {
        changeWorkspace(7);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("o"))) {
        changeWorkspace(8);
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

    if (getCurrentWorkspace()->node == NULL)
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);

    printf("Terminate: handle destroy notify\n");
}
