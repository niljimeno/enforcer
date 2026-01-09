#include "windows.c"
#include "manager.c"
#include "scheme.c"

/* Changes focus for the last window in the workspace */
void restoreFocus() {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    struct Node* lastValid = NULL;
    if (lastNode == NULL) return;
    while (lastNode) {
        if (lastNode->isAlive)
            lastValid = lastNode;
        lastNode = lastNode->next;
    }

    XSetInputFocus(display, lastValid->window, RevertToPointerRoot, CurrentTime);
}

/* todo! this breaks when emacs quits. something is in the way */
void restoreWorkspace() {
    if (workspace.node) {
        resizeWindows();
        restoreFocus();
    }
}

void closeFocusedWindow() {
    Window w = getCurrentWindow();
    removeWindow(w);
    restoreWorkspace();
}
