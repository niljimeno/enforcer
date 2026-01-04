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
        closeWindow(ev->xkey.subwindow);
    }
}

void handleMapNotify(XEvent *ev) {
    Window w = ev->xmaprequest.window;
    createWindow(w);
    resizeWindows();
}
