struct WindowData {
    Bool shouldFloat;
    Bool isHelper;
};

void resizeWindow(Window win, int x, int y, int w, int h) {
    XMoveResizeWindow(display, win, x, y, w, h);
}

void closeWindow(Window w) {
    printf("Initiate: close window\n");
    if (w == 0) {
        printf("Terminate: close window (null)\n");
        return;
    }

    Atom *protocols;
    int n_protocols;

    if (XGetWMProtocols(display, w, &protocols, &n_protocols) && protocols) {
        for (int i = 0; i < n_protocols; i++) {
            if (protocols[i] == WM_DELETE_WINDOW) {
                XEvent ev = {.xclient = {
                    .type = ClientMessage,
                    .window = w,
                    .message_type = WM_PROTOCOLS,
                    .format = 32}};

                ev.xclient.data.l[0] = WM_DELETE_WINDOW;
                ev.xclient.data.l[1] = CurrentTime;
                XSendEvent(display, w, False, NoEventMask, &ev);
                XFree(protocols);
                printf("Terminate: close window (normal)\n");
                return;
            }
        }

        XUnmapWindow(display, w);
        XFree(protocols);
    }

    XKillClient(display, w);
    printf("Terminate: close window (forced)\n");
}


struct WindowData initialiseWindow(Window w) {
    printf("Initiate: initialise window\n");
    long supplied_ret;
    XSizeHints size_hints;

    struct WindowData winData;
    winData.shouldFloat = false;

    Atom type;
    int format;
    unsigned long n_items, after;
    Atom *types = NULL;

    /* check for unusal types */
    if (XGetWindowProperty(display, w, _NET_WM_WINDOW_TYPE, 0, 8, False, XA_ATOM, &type, &format,
                           &n_items, &after, (unsigned char **)&types) == Success && types) {
        for (unsigned long i = 0; i < n_items; i++) {
            if (types[i] == _NET_WM_WINDOW_TYPE_DOCK) {
                XFree(types);
                XMapWindow(display, w);
                winData.shouldFloat = true;
                printf("Terminate: initialise window\n");
                return winData;
            }

            if (types[i] == _NET_WM_WINDOW_TYPE_UTILITY ||
                types[i] == _NET_WM_WINDOW_TYPE_DIALOG  ||
                types[i] == _NET_WM_WINDOW_TYPE_TOOLBAR ||
                types[i] == _NET_WM_WINDOW_TYPE_SPLASH  ||
                types[i] == _NET_WM_WINDOW_TYPE_POPUP_MENU ||
                types[i] == _NET_WM_WINDOW_TYPE_DROPDOWN_MENU ||
                types[i] == _NET_WM_WINDOW_TYPE_MENU ||
                types[i] == _NET_WM_WINDOW_TYPE_DOCK ||
                types[i] == _NET_WM_WINDOW_TYPE_TOOLTIP ||
                types[i] == _NET_WM_WINDOW_TYPE_NOTIFICATION) {
                printf("is internal\n");
                winData.shouldFloat = true;
                break;
            }
        }
        XFree(types);
    }

    /* check for fixed size windows */
    /*
    if (!winData.shouldFloat &&
        XGetWMNormalHints(display, w, &size_hints, &supplied_ret) &&
        (size_hints.flags & PMinSize) && (size_hints.flags & PMaxSize) &&
        size_hints.min_width == size_hints.max_width &&
        size_hints.min_height == size_hints.max_height) {
        printf("is floating\n");
        winData.shouldFloat = True;
    }


    XClassHint *class_hint = XAllocClassHint();
    if (XGetClassHint(display, w, class_hint)) {
        printf("Window class: %s\n", class_hint->res_class);
        printf("Window name: %s\n", class_hint->res_name);
    } else {
        printf("Failed to get window class hint\n");
    }
    XFree(class_hint);

    Window transient;
    if (!winData.shouldFloat && XGetTransientForHint(display, w, &transient)) {
        printf("Is transient\n");
        winData.shouldFloat = True;
    }
    */

    printf("Terminate: initialise window\n");
    return winData;
}

Window getCurrentWindow() {
    printf("Initiate: get current window\n");
    Window w;
    int revert;
    XGetInputFocus(display, &w, &revert);
    printf("Terminate: get current window\n");
    return w;
}
