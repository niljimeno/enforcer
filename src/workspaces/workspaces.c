#include "windows.c"
#include "manager.c"
#include "scheme.c"

struct Node* getLastValid() {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    struct Node* lastValid = NULL;

    while (lastNode) {
        if (lastNode->isAlive && lastNode->visible)
            lastValid = lastNode;
        lastNode = lastNode->next;
    }

    return lastValid;
}

/* Changes focus for the last window in the workspace */
void restoreFocus(struct Node* lastValid) {
    printf("Initiate: restore focus\n");

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
    // if (getCurrentWorkspace()->node) {

    struct Node* lastValid = getLastValid();

    restoreFocus(lastValid);
    XSync(display, False);
    resizeWindows();

    drawBorder(lastValid);

    // }
    printf("Terminate: restore workspace\n");
}

void closeFocusedWindow() {
    printf("Initiate: close focused window\n");
    Window w = getCurrentWindow();
    closeWindow(w);
    restoreWorkspace();
    printf("Terminate: close focused window\n");
}

void changeWorkspace(int n) {
    if (currentWorkspaceIndex == n) return;

    printf("Initiate: Changing workspace\n");
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

void moveToWorkspace(int n) {
    printf("Initiate: Move to workspace\n");
    if (currentWorkspaceIndex == n) return;

    Window w = getCurrentWindow();
    struct Node* target = getNode(w);
    if (target == NULL || target->isAlive == false) return;

    XUnmapWindow(display, target->window);

    struct Node* newNode = malloc(sizeof(struct Node));
    *newNode = *target;

    removeNode(w);

    struct Workspace *lastWS = workspace;
    struct Node *lastNode;

    for (int i=0; i<n; i++)
        lastWS = lastWS->next;

    lastNode = lastWS->node;
    if (lastNode == NULL) {
        lastWS->node = newNode;
        restoreWorkspace();
        return;
    }

    while (lastNode->next != NULL)
        lastNode = lastNode->next;

    lastNode->next = newNode;
    restoreWorkspace();

    printf("Terminate: Move to workspace\n");
}
