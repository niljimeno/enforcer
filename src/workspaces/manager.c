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

void setUpWorkspaces() {
    createWorkspace();
    createWorkspace();
    createWorkspace();
    createWorkspace();
    createWorkspace();
    createWorkspace();
    createWorkspace();
}
