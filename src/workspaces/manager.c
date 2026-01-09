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
    instance->node = 0;
    lastWS->next = instance;
}

/* Initialise window with its correspondent node */
struct WindowData createWindow(Window w) {
    struct Workspace* currentWS = getCurrentWorkspace();
    struct Node* lastNode;

    struct Node* instance = malloc(sizeof(struct Node));
    struct WindowData winData = initialiseWindow(w);

    instance->window = w;
    instance->isFloating = winData.shouldFloat;
    instance->isAlive = true;
    instance->next = NULL;

    printf("should float %b\n", instance->isFloating);

    if (currentWS->node == NULL) {
        currentWS->node = instance;
        return winData;
    }

    lastNode = currentWS->node;

    while (lastNode->next)
        lastNode = lastNode->next;

    lastNode->next = instance;
    return winData;
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

    currentNode->isAlive = false;

    if (previousNode == NULL) {
        if (currentNode->next != NULL)
            ws->node = currentNode->next;
        else
            ws->node = NULL;
    } else
        previousNode->next = currentNode->next;

    closeWindow(currentNode->window);
    free(currentNode);
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

    currentNode->isAlive = false;

    if (previousNode == NULL) {
        ws->node = currentNode->next;
    } else {
        previousNode->next = currentNode->next;
    }

    free(currentNode);
}

Bool nodeExists(Window w) {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;
    while (node) {
        if (node->window == w)
            return true;
        node = node->next;
    }

    return false;
}

struct Node* getNode(Window w) {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;
    while (node) {
        if (node->window == w)
            return node;
        node = node->next;
    }

    return NULL;
}

void setUpWorkspaces() {
    createWorkspace();
}
