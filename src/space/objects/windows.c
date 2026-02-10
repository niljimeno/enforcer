struct Node* getNode(Window w) {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node;

    while (ws) {
        node = ws->node;

        while (node) {
            if (node->window == w)
                return node;
            node = node->next;
        }

        ws = ws->next;
    }

    return NULL;
}

struct Node* getLocalNode(Window w) {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;

    while (node) {
        if (node->window == w)
            return node;
        node = node->next;
    }

    return NULL;
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
    instance->isVisible = true;

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

/* remove Node (not window) */
void removeNode(Window w) {
    struct Workspace* ws = workspace;
    struct Node* currentNode;
    struct Node* previousNode;
    struct Node* nextNode;

    while (ws) {
        currentNode = ws->node;
        previousNode = NULL;

        while (currentNode) {
            nextNode = currentNode->next;

            if (currentNode->window == w) {
                currentNode->isAlive = false;

                if (previousNode == NULL) {
                    ws->node = currentNode->next;
                } else {
                    previousNode->next = currentNode->next;
                }

                free(currentNode);
                currentNode = NULL;
            }

            previousNode = currentNode ? currentNode : previousNode;
            currentNode = nextNode;
        }

        ws = ws->next;
    }
}
