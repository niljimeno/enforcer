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

#include "config.h"
#include "types.h"

Display * display;
Window root;
EventHandler eventTable[LASTEvent];
void hdl_destroy(XEvent *ev);

#include "windows.c"
#include "actions.c"

void hdl_destroy(XEvent *ev) {
    spawn_daemon("redshift -O 3000 -P");
}

int main(void)
{
    if (setup() != 0) return 1;

    XEvent ev;
    for(;;)
    {
        XNextEvent(display, &ev);
        if (ev.type >= 0 && ev.type < LASTEvent) {
            eventTable[ev.type](&ev);
        }
    }
}
