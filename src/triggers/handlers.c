EventHandler eventTable[LASTEvent];

void dummy(XEvent *ev) {}

void handleKeyPress(XEvent *ev) {
    KeySym key = XLookupKeysym(&ev->xkey, 0);

    if ((key == XK_c) && ev->xkey.state & ShiftMask) {
        spawnDaemon("pkill -f xinit");
    } else if (key == XK_Return) {
        spawnDaemon(terminal);
    } else if (key == XK_p) {
        sh("dmenu_run");
    } else if (key == XK_c) {
        closeFocusedWindow();
    } else if (key == XK_j) {
        changeFocus(+1);
    } else if (key == XK_k) {
        changeFocus(-1);

    } else if (key == XK_1) {
        moveToWorkspace(focusedNode, 0);
    } else if (key == XK_2) {
        moveToWorkspace(focusedNode, 1);
    } else if (key == XK_3) {
        moveToWorkspace(focusedNode, 2);
    } else if (key == XK_4) {
        moveToWorkspace(focusedNode, 3);
    } else if (key == XK_5) {
        moveToWorkspace(focusedNode, 4);
    } else if (key == XK_6) {
        moveToWorkspace(focusedNode, 5);
    } else if (key == XK_7) {
        moveToWorkspace(focusedNode, 6);
    } else if (key == XK_8) {
        moveToWorkspace(focusedNode, 7);
    } else if (key == XK_9) {
        moveToWorkspace(focusedNode, 8);

    } else if (key == XK_q) {
        changeWorkspace(0);
    } else if (key == XK_w) {
        changeWorkspace(1);
    } else if (key == XK_e) {
        changeWorkspace(2);
    } else if (key == XK_r) {
        changeWorkspace(3);
    } else if (key == XK_t) {
        changeWorkspace(4);
    } else if (key == XK_y) {
        changeWorkspace(5);
    } else if (key == XK_u) {
        changeWorkspace(6);
    } else if (key == XK_i) {
        changeWorkspace(7);
    } else if (key == XK_o) {
        changeWorkspace(8);
    } else if (key == XK_m) {
        monocleMode = true;
        restoreWorkspace();
    } else if (key == XK_s) {
        monocleMode = false;
        restoreWorkspace();
    } else if (key == XK_h) {
        printWorkspaceInformation();
    }
}

/* before mapping the window */
void handleMapRequest(XEvent *ev) {
    Window w = ev->xmaprequest.window;
    if (getNode(w) != NULL) return;

    struct WindowData winData = createWindow(w);
    if (winData.shouldFloat) {
        XMapWindow(display, w);
        return;
    }

    resizeWindows();
    XMapWindow(display, w);
    setFocus(getNode(w));
}

/* after mapping the window */
void handleMapNotify(XEvent *ev) {
    struct Node* node = getLocalNode(ev->xmaprequest.window);
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
    removeNode(getNode(w));
    restoreWorkspace();

    if (getCurrentWorkspace()->node == NULL)
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);
}
