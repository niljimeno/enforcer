struct WindowData {
    Bool shouldFloat;
};

void resizeWindow(Window win, int x, int y, int w, int h) {
    XMoveResizeWindow(display, win, x, y, w, h);
}

void closeWindow(Window w) {
    if (w == 0) {
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
                return;
            }
        }

        XUnmapWindow(display, w);
        XFree(protocols);
    }

    XKillClient(display, w);
}


struct WindowData initialiseWindow(Window w) {
    long supplied_ret;
    XSizeHints size_hints;
    XSetInputFocus(display, w, RevertToPointerRoot, CurrentTime);

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
                winData.shouldFloat = True;
                break;
            }
        }
        XFree(types);
    }

    /* check for fixed size windows */
    if (!winData.shouldFloat &&
        XGetWMNormalHints(display, w, &size_hints, &supplied_ret) &&
        (size_hints.flags & PMinSize) && (size_hints.flags & PMaxSize) &&
        size_hints.min_width == size_hints.max_width &&
        size_hints.min_height == size_hints.max_height) {
        winData.shouldFloat = True;
    }

    Atom net_wm_allowed_actions = XInternAtom(display, "_NET_WM_ALLOWED_ACTIONS", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *data;

    // check i resizing is allowed??? i think??
    if (!winData.shouldFloat) {
    if (XGetWindowProperty(display, w, net_wm_allowed_actions, 0, 1024, False, AnyPropertyType,
            &actual_type, &actual_format, &nitems, &bytes_after, &data) == Success) {
        if (actual_type != None && data != NULL) {
            Atom *actions = (Atom *)data;
            for (unsigned long i = 0; i < nitems; i++) {
                if (actions[i] == XInternAtom(display, "_NET_WM_ACTION_RESIZE", False)) {
                    winData.shouldFloat = True;
                    XFree(data);
                    return winData;
                }
            }
        }
        XFree(data);
    }}

    XClassHint *class_hint = XAllocClassHint();
    if (XGetClassHint(display, w, class_hint)) {
        printf("Window class: %s\n", class_hint->res_class);
        printf("Window name: %s\n", class_hint->res_name);
        if (strcmp(class_hint->res_name, "dmenu") == 0) {
            winData.shouldFloat = True;
        } else if (strcmp(class_hint->res_name, "libresprite") == 0) {
            winData.shouldFloat = True;
        }
    } else {
        printf("Failed to get window class hint\n");
    }
    XFree(class_hint);

    Window transient;
    if (!winData.shouldFloat && XGetTransientForHint(display, w, &transient)) {
        winData.shouldFloat = True;
    }

    return winData;
}
