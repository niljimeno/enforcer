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
        printf("Focusable window found\n");
        focusWindow(lastValid->window);
    } else {
        printf("No windows found here.\n");
    }

    printf("Terminate: restore focus\n");
}

/* todo! this breaks when emacs quits. something is in the way */
void restoreWorkspace() {
    printf("Initiate: restore workspace\n");
    if (getCurrentWorkspace()->node) {
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

void changeWorkspace(int n) {
    printf("Initialise: Changing workspace\n");
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;

    while (node != NULL) {
        if (node->isAlive)
            XUnmapWindow(display, node->window);

        node = node->next;
    }

    currentWorkspaceIndex = n;
    ws = getCurrentWorkspace();
    node = ws->node;

    while (node != NULL) {
        if (node->isAlive)
            XMapWindow(display, node->window);

        node = node->next;
    }

    restoreWorkspace();
    printf("Terminate: Changing workspace\n");
}
