#define XA_ATOM ((Atom) 4)

typedef void (*EventHandler) (XEvent *);

Atom _NET_WM_WINDOW_TYPE;
Atom _NET_WM_WINDOW_TYPE_DOCK;
Atom _NET_WM_WINDOW_TYPE_UTILITY;
Atom _NET_WM_WINDOW_TYPE_DIALOG;
Atom _NET_WM_WINDOW_TYPE_TOOLBAR;
Atom _NET_WM_WINDOW_TYPE_SPLASH;
Atom _NET_WM_WINDOW_TYPE_POPUP_MENU;
Atom _NET_WM_WINDOW_TYPE_MENU;
Atom _NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
Atom _NET_WM_WINDOW_TYPE_TOOLTIP;
Atom _NET_WM_WINDOW_TYPE_NOTIFICATION;
Atom _NET_WM_STATE_MODAL;

Atom _NET_SUPPORT;
Atom _NET_NAME;
Atom UTF8;

Atom WM_DELETE_WINDOW;
Atom WM_PROTOCOLS;

void setWindowTypes(Display *display) {
    _NET_WM_WINDOW_TYPE = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    _NET_WM_WINDOW_TYPE_DOCK = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    _NET_WM_WINDOW_TYPE_UTILITY = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", False);
    _NET_WM_WINDOW_TYPE_DIALOG = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    _NET_WM_WINDOW_TYPE_TOOLBAR = XInternAtom(display, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
    _NET_WM_WINDOW_TYPE_SPLASH = XInternAtom(display, "_NET_WM_WINDOW_TYPE_SPLASH", False);
    _NET_WM_WINDOW_TYPE_POPUP_MENU = XInternAtom(display, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);
    _NET_WM_WINDOW_TYPE_MENU = XInternAtom(display, "_NET_WM_WINDOW_TYPE_MENU", False);
    _NET_WM_WINDOW_TYPE_DROPDOWN_MENU = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
    _NET_WM_WINDOW_TYPE_TOOLTIP = XInternAtom(display, "_NET_WM_WINDOW_TYPE_TOOLTIP", False);
    _NET_WM_WINDOW_TYPE_NOTIFICATION = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
    _NET_WM_STATE_MODAL = XInternAtom(display, "_NET_WM_STATE_MODAL", False);
    _NET_SUPPORT = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
    _NET_NAME = XInternAtom(display, "_NET_WM_NAME", False);

    WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    WM_PROTOCOLS =  XInternAtom(display, "WM_PROTOCOLS", False);

    UTF8 = XInternAtom(display, "UTF8_STRING", False);
}
