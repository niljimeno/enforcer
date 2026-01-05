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
int currentWorkspace = 0;

void removeWorkspaceNode(Window w) {
}

struct Workspace* getCurrentWorkspace() {
    struct Workspace* lastWS = &workspace;
    printf("start it!\n");
    for (int i=0; i<currentWorkspace; i++) {
        printf("one cycle\n");
        lastWS = lastWS->next;
    }

    return lastWS;
}

void createWorkspace() {
    struct Workspace* lastWS = &workspace;
    while (true) {
        if (lastWS->next == NULL)
            break;
        lastWS = lastWS->next;
    }

    struct Workspace* instance = malloc(sizeof(struct Workspace));
    lastWS->next = instance;
}

void createWorkspaceAt(int position) {
}

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

    while (true) {
        if (lastNode->next == NULL) {
            lastNode->next = instance;
            break;
        }
        lastNode = lastNode->next;
    }
}

void resizeWindows() {
    int snum = DefaultScreen(display);
    int width = DisplayWidth(display, snum);
    int height = DisplayHeight(display, snum);

    struct Node* node = (getCurrentWorkspace())->node;
    if (node == NULL) return;

    int i = 0;
    while (true) {
        ++i;
        if (!(node->isFloating))
            resizeWindow(node->window,
                         50+(i*10),
                         50+(i*10),
                         width - 200,
                         height - 200);
        if (node->next == NULL) {
            return;
        }

        node = node->next;
    }
}

void closeLastNode() {
    struct Workspace* currentWorkspace = getCurrentWorkspace();
    struct Node* currentNode = currentWorkspace->node;
    struct Node* previousNode = NULL;
    if (currentNode == NULL) return;
    while (true) {
        if (currentNode->next == NULL) {
            break;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    closeWindow(currentNode->window);
    free(currentNode);

    if (previousNode == NULL)
        currentWorkspace->node = NULL;
    else
        previousNode->next = NULL;
}

void setFocusBack() {
    struct Workspace* currentWorkspace = getCurrentWorkspace();
    struct Node* lastNode = currentWorkspace->node;
    if (lastNode == NULL) return;
    while (true) {
        if (lastNode->next == NULL) {
            break;
        }

        lastNode = lastNode->next;
    }

    if (lastNode->window == NULL) {
        printf("bad window");
        return;
    }

    XSetInputFocus(display, lastNode->window, RevertToPointerRoot, CurrentTime);
}

void removeNode(Window w) {
    struct Workspace* currentWorkspace = getCurrentWorkspace();
    struct Node* currentNode = currentWorkspace->node;
    struct Node* previousNode = NULL;
    if (currentNode == NULL) return;
    while (true) {
        if (currentNode->window == w) {
            break;
        }
        if (currentNode->next == NULL) {
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (previousNode == NULL)
        currentWorkspace->node = currentNode->next;
    else
        previousNode->next = currentNode->next;

    free(currentNode);
    setFocusBack();
}

void removeWindow(Window w) {
    struct Workspace* currentWorkspace = getCurrentWorkspace();
    struct Node* currentNode = currentWorkspace->node;
    struct Node* previousNode = NULL;
    if (currentNode == NULL) return;
    while (true) {
        if (currentNode->window == w) {
            break;
        }
        if (currentNode->next == NULL) {
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (previousNode == NULL)
        currentWorkspace->node = currentNode->next;
    else
        previousNode->next = currentNode->next;

    closeWindow(currentNode->window);
    free(currentNode);

    setFocusBack();
}

void closeFocusedWindow() {
    Window w = getCurrentWindow();
    removeWindow(w);
}

void setUpWorkspaces() {
    createWorkspace();
}
