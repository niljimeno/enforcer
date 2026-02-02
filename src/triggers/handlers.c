EventHandler eventTable[LASTEvent];

void dummy(XEvent *ev) {}

void handleKeyPress(XEvent *ev) {
    KeyCode code = ev->xkey.keycode;

    if (code == XKeysymToKeycode(display, XStringToKeysym("c")) && ev->xkey.state & ShiftMask) {
        spawnDaemon("pkill -f xinit");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("Return"))) {
        spawnDaemon("st");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("p"))) {
        sh("dmenu_run");
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("c"))) {
        closeFocusedWindow();
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("j"))) {
        changeFocus(+1);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("k"))) {
        changeFocus(-1);

    } else if (code == XKeysymToKeycode(display, XStringToKeysym("1"))) {
        moveToWorkspace(0);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("2"))) {
        moveToWorkspace(1);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("3"))) {
        moveToWorkspace(2);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("4"))) {
        moveToWorkspace(3);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("5"))) {
        moveToWorkspace(4);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("6"))) {
        moveToWorkspace(5);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("7"))) {
        moveToWorkspace(6);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("8"))) {
        moveToWorkspace(7);
    } else if (code == XKeysymToKeycode(display, XStringToKeysym("9"))) {
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
}

/* before mapping the window */
void handleMapRequest(XEvent *ev) {
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
    drawBorder(getNode(w));
}

/* after mapping the window */
void handleMapNotify(XEvent *ev) {
    struct Node* node = getNode(ev->xmaprequest.window);
    if (node)
        node->isVisible = true;
}

/* after unmapping the window */
void handleUnmapNotify(XEvent *ev) {
    struct Node* node = getNode(ev->xmaprequest.window);
    if (node)
        node->isVisible = false;
}

void handleDestroyNotify(XEvent *ev) {
    Window w = ev->xdestroywindow.window;

    removeNode(w);
    restoreWorkspace();

    if (getCurrentWorkspace()->node == NULL)
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);
}
