#include "windows.c"
#include "manager.c"
#include "scheme.c"

/* Changes focus for the last window in the workspace */
void restoreFocus() {
    printf("Initiate: restore focus\n");
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    struct Node* lastValid = NULL;

    while (lastNode) {
        if (lastNode->isAlive)
            lastValid = lastNode;
        lastNode = lastNode->next;
    }

    if (lastValid != NULL) {
        XSetInputFocus(display, lastValid->window, RevertToPointerRoot, CurrentTime);
    } else {
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);
    }


    printf("Terminate: restore focus\n");
}

/* todo! this breaks when emacs quits. something is in the way */
void restoreWorkspace() {
    printf("Initiate: restore workspace\n");
    if (workspace.node) {
        resizeWindows();
        restoreFocus();
    }
    printf("Terminate: restore workspace\n");
}

void closeFocusedWindow() {
    printf("Initiate: close focused window\n");
    Window w = getCurrentWindow();
    removeWindow(w);
    restoreWorkspace();
    printf("Terminate: close focused window\n");
}
