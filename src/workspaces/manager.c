struct Node {
    Window window;
    struct Node* next;
    bool isFloating;
    bool isAlive;
};

struct Workspace {
    struct Workspace* next;
    struct Node* node;
};

struct Workspace workspace;
int currentWorkspaceIndex = 0;

struct Workspace* getCurrentWorkspace() {
    struct Workspace* lastWS = &workspace;
    for (int i=0; i<currentWorkspaceIndex; i++)
        lastWS = lastWS->next;

    return lastWS;
}

/* Append a workspace at the end of the list */
void createWorkspace() {
    struct Workspace* lastWS = &workspace;
    while (lastWS->next)
        lastWS = lastWS->next;

    struct Workspace* instance = malloc(sizeof(struct Workspace));
    lastWS->next = instance;
}

/* Initialise window with its correspondent node */
void createWindow(Window w) {
    struct Workspace* currentWS = getCurrentWorkspace();
    struct Node* lastNode;

    struct Node* instance = malloc(sizeof(struct Node));
    struct WindowData data = initialiseWindow(w);
    instance->window = w;
    instance->isFloating = data.shouldFloat;
    instance->isAlive = true;
    instance->next = NULL;

    if (currentWS->node == NULL) {
        currentWS->node = instance;
        return;
    }

    lastNode = currentWS->node;

    while (lastNode->next)
        lastNode = lastNode->next;

    lastNode->next = instance;
}

/* apply resizing to all windows in the workspace */
void resizeWindows() {
    int snum = DefaultScreen(display);
    int width = DisplayWidth(display, snum);
    int height = DisplayHeight(display, snum);

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;
    if (node == NULL) return;

    int count = 0;
    while (node) {
        if (!(node->isFloating))
            ++count;

        node = node->next;
    }

    node = ws->node;

    int i = 0;
    while (node) {
        ++i;
        if (!(node->isFloating))
            resizeWindow(node->window,
                         50+(i*5*count),
                         50+(i*5*count),
                         width - 200,
                         height - 200 + (count*10));

        node = node->next;
    }
}

/* Changes focus for the last window in the workspace */
void restoreFocus() {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    if (lastNode == NULL) return;
    while (lastNode->next != NULL)
        lastNode = lastNode->next;

    if (lastNode->window == 0) {
        printf("bad window\n");
        return;
    }

    XSetInputFocus(display, lastNode->window, RevertToPointerRoot, CurrentTime);
}

void restoreWorkspace() {
    resizeWindows();
    restoreFocus();
}

/* remove window and its node */
void removeWindow(Window w) {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* currentNode = ws->node;
    struct Node* previousNode = NULL;
    if (currentNode == NULL) return;
    while (true) {
        if (currentNode->window == w)
            break;
        if (currentNode->next == NULL)
            return;

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (previousNode == NULL)
        ws->node = currentNode->next;
    else
        previousNode->next = currentNode->next;

    closeWindow(currentNode->window);
    free(currentNode);
    restoreWorkspace();
}
/* remove Node (not window) */
void removeNode(Window w) {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* currentNode = ws->node;
    struct Node* previousNode = NULL;
    if (currentNode == NULL) return;
    while (true) {
        if (currentNode->window == w)
            break;
        if (currentNode->next == NULL)
            return;

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (previousNode == NULL)
        ws->node = currentNode->next;
    else
        previousNode->next = currentNode->next;

    free(currentNode);
    restoreWorkspace();
}
void closeFocusedWindow() {
    Window w = getCurrentWindow();
    removeWindow(w);
}

void setUpWorkspaces() {
    createWorkspace();
}
