void closeWindow(Window w) {
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

void handle_map_notify(XEvent *ev) {
    long supplied_ret;
    XSizeHints size_hints;

    Window w = ev->xmaprequest.window;

	Atom type;
	int format;
	unsigned long n_items, after;
	Atom *types = NULL;
	Bool should_float = False;

    if (XGetWindowProperty(display, w, _NET_WM_WINDOW_TYPE, 0, 8, False, XA_ATOM, &type, &format,
    			           &n_items, &after, (unsigned char **)&types) == Success && types) {

    	for (unsigned long i = 0; i < n_items; i++) {
    		if (types[i] == _NET_WM_WINDOW_TYPE_DOCK) {
    		    XFree(types);
    		    XMapWindow(display, w);
    		    return;
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
    			should_float = True;
    			break;
    		}
    	}
    	XFree(types);
	}

    if (!should_float &&
        XGetWMNormalHints(display, w, &size_hints, &supplied_ret) &&
		(size_hints.flags & PMinSize) && (size_hints.flags & PMaxSize) &&
		size_hints.min_width == size_hints.max_width &&
		size_hints.min_height == size_hints.max_height) {
    	should_float = True;
	}

    Atom net_wm_allowed_actions = XInternAtom(display, "_NET_WM_ALLOWED_ACTIONS", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *data;

	if (!should_float) {
    if (XGetWindowProperty(display, w, net_wm_allowed_actions, 0, 1024, False, AnyPropertyType,
            &actual_type, &actual_format, &nitems, &bytes_after, &data) == Success) {
        if (actual_type != None && data != NULL) {
            Atom *actions = (Atom *)data;
            for (unsigned long i = 0; i < nitems; i++) {
                if (actions[i] == XInternAtom(display, "_NET_WM_ACTION_RESIZE", False)) {
                    should_float = True;
                    XFree(data);
                    return;
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
            should_float = True;
        } else if (strcmp(class_hint->res_name, "libresprite") == 0) {
            should_float = True;
        }
    } else {
        printf("Failed to get window class hint\n");
    }
    XFree(class_hint);

	Window transient;
	if (!should_float && XGetTransientForHint(display, w, &transient)) {
		should_float = True;
	}

	printf("window should float: %b\n", should_float);

	if (!should_float) {
        XMoveResizeWindow(display, w, 200, 200, 1000, 800);
	}
}
