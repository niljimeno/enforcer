#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

/* values and general use */
#include "data/data.c"

/* global values */
Display * display;
Window root;

/* moving parts */
#include "workspaces/workspaces.c"
#include "triggers/triggers.c"
#include "test.c"

int setup() {
    XInitThreads();
    if(!(display = XOpenDisplay(0x0))) return 1;
    root = DefaultRootWindow(display);
    setWindowTypes(display);
    loadTriggers();
    setUpWorkspaces();

    return 0;
}

int main(int argc, char* args[])
{
    if (argc > 1 && strcmp("test",args[1]) == 0) {
        test();
        return 0;
    }

    if (setup() != 0) return 1;

    XEvent ev;
    for(;;)
    {
        printf("before XNextEvent\n");
        fflush(stdout);
        XNextEvent(display, &ev);
        printf("got event: %d\n", ev.type);
        fflush(stdout);

        // XNextEvent(display, &ev);
        if (ev.type >= 0 && ev.type < LASTEvent) {
            eventTable[ev.type](&ev);
        }
    }
}
