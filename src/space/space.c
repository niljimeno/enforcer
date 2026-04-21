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
    drawBorder(focusedNode);
}

Window getFocusedWindow() {
    if (focusedNode == NULL) return 0;
    return focusedNode->window;
}

void closeFocusedWindow() {
    Window w = getCurrentWindow();
    closeWindow(w);
}

void killFocusedWindow() {
    Window w = getCurrentWindow();
    killWindow(w);
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

void moveInWorkspace(struct Node* target, int direction) {
    if (target == NULL || target->isAlive == false) return;

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;

    struct Node* previous = NULL;
    struct Node* previous2 = NULL;

    while (node) {
        if (node == target) {
            if (direction == LEFT) {
                if (!previous) return;

                if (previous2)
                    previous2->next = node;
                else
                    ws->node = node;

                previous->next = node->next;
                node->next = previous;

            } else if (direction == RIGHT) {
                struct Node* nextNode = node->next;
                if (!nextNode) return;

                node->next = nextNode->next;
                nextNode->next = node;
                if (previous) previous->next = nextNode;
                else ws->node = nextNode;
            }

            resizeWindows();
            return;
        }

        previous2 = previous;
        previous = node;
        node = node->next;
    }
}
