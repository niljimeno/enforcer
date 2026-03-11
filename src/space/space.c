#include "windows.c"
#include "objects/objects.c"
#include "scheme/scheme.c"

void setUpWorkspaces() {
    for (int i=0; i<9; i++)
        createWorkspace();
}

struct Node* getLastValid() {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    struct Node* lastValid = NULL;

    while (lastNode) {
        if (isValid(lastNode))
            lastValid = lastNode;
        lastNode = lastNode->next;
    }

    return lastValid;
}

/* todo! this breaks when emacs quits. something is in the way */
void restoreWorkspace() {
    struct Workspace* ws = getCurrentWorkspace();

    struct Node* target;
    if (focusedNode && isValid(focusedNode)) {
        target = focusedNode;
    } else if (ws->previous && isValid(ws->previous)) {
        target = ws->previous;
    } else {
        target = getLastValid();
    }

    setFocus(target);
    XSync(display, False);
    resizeWindows();
}

Window getFocusedWindow() {
    if (focusedNode == NULL) return 0;
    return focusedNode->window;
}

void closeFocusedWindow() {
    Window w = getCurrentWindow();
    closeWindow(w);
    restoreWorkspace();
}

/* calls a map function (f) while changing internal values
 * to prevent default behaviour */
void controlledMap(struct Node* node, int (*f)(Display*, Window)) {
    if (node->isAlive) {
        node->isTransitioning = true;
        f(display, node->window);
    }
}

void changeWorkspace(int n) {
    if (currentWorkspaceIndex == n) return;

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;

    ws->previous = focusedNode;
    focusedNode = NULL;

    while (node != NULL) {
        controlledMap(node, XUnmapWindow);
        node = node->next;
    }

    currentWorkspaceIndex = n;
    ws = getCurrentWorkspace();
    node = ws->node;

    while (node != NULL) {
        controlledMap(node, XMapWindow);
        node = node->next;
    }

    XSync(display, false);
    updateVisibility();
    restoreWorkspace();
}

void moveToWorkspace(struct Node* target, int n) {
    if (currentWorkspaceIndex == n) return;
    if (target == NULL || target->isAlive == false) return;

    XUnmapWindow(display, target->window);

    struct Node* newNode = malloc(sizeof(struct Node));
    *newNode = *target;
    newNode->next = NULL;
    newNode->isVisible = true;

    removeNode(target);

    struct Workspace *lastWS = workspace;
    struct Node *lastNode;

    for (int i=0; i<n; i++)
        lastWS = lastWS->next;

    lastNode = lastWS->node;
    if (lastNode) {
        while (lastNode->next != NULL)
            lastNode = lastNode->next;

        lastNode->next = newNode;
    } else {
        lastWS->node = newNode;
    }

    restoreWorkspace();
}
