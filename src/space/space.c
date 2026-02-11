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
    if (ws->previous) {
        target = ws->previous;
    } else {
        target = getLastValid();
    }

    setFocus(target);
    XSync(display, False);
    resizeWindows();
}

void closeFocusedWindow() {
    Window w = getCurrentWindow();
    closeWindow(w);
    restoreWorkspace();
}

void changeWorkspace(int n) {
    if (currentWorkspaceIndex == n) return;

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;

    ws->previous = focusedNode;

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
}

void moveToWorkspace(struct Node* target, int n) {
    if (currentWorkspaceIndex == n) return;

    if (target == NULL || target->isAlive == false) return;

    XUnmapWindow(display, target->window);

    struct Node* newNode = malloc(sizeof(struct Node));
    *newNode = *target;

    removeNode(target);

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
}
