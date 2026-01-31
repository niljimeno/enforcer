EventHandler eventTable[LASTEvent];

void dummy(XEvent *ev) {}

void handleKeyPress(XEvent *ev) {
    printf("Initiate: handle keypress\n");
    KeyCode code = ev->xkey.keycode;

    printf("Keycode is %d\n", code);
    printf("Exclam Keycode is %d\n", XKeysymToKeycode(display, XStringToKeysym("!")));

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
}

/* after unmapping the window */
void handleUnmapNotify(XEvent *ev) {
    /*
     * unfortunately this activates when changing workspace

    Window w = ev->xmaprequest.window;
    if (!nodeExistsInWorkspace(w, getCurrentWorkspace()))
        removeNode(w);
    */
}

void handleDestroyNotify(XEvent *ev) {
    printf("Initiate: handle destroy notify\n");
    Window w = ev->xdestroywindow.window;
    /*
    if (w && nodeExists(w)) {
        removeNode(w);
        restoreWorkspace();
    }
    */

    removeNode(w);
    restoreWorkspace();


    if (getCurrentWorkspace()->node == NULL) {
        printf("Set input focus -- handleDestroyNotify");
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);
    }

    printf("Terminate: handle destroy notify\n");
}
