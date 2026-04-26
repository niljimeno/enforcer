struct Node* getNode(Window w) {
    struct Workspace* ws = workspace;
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
void createWindow(Window w) {
    struct Workspace* currentWS = getCurrentWorkspace();
    struct Node* lastNode;

    struct Node* instance = malloc(sizeof(struct Node));
    struct WindowData winData = initialiseWindow(w);

    instance->window = w;
    instance->isFloating = winData.shouldFloat;
    instance->isAlive = true;
    instance->next = NULL;
    instance->isVisible = true;
    instance->isNew = true;
    instance->isTransitioning = true;

    if (currentWS->node == NULL) {
        currentWS->node = instance;
        return;
    }

    lastNode = currentWS->node;

    while (lastNode->next)
        lastNode = lastNode->next;

    lastNode->next = instance;
}

/* remove Node (not window) */
void removeNode(struct Node* target) {
    struct Workspace* ws = workspace;
    struct Node* currentNode;
    struct Node* previousNode;
    struct Node* nextNode;

    if (focusedNode == target) {
        focusedNode = NULL;
    }

    while (ws) {
        if (ws->previous == target) {
            ws->previous = NULL;
        }

        currentNode = ws->node;
        previousNode = NULL;

        while (currentNode) {
            nextNode = currentNode->next;

            if (currentNode->window == target->window) {
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

void updateVisibility() {
    printf("Update visibility\n");
    struct Node* node = getCurrentWorkspace()->node;
    while (node) {
        node->isVisible = node->isMapped;
        node = node->next;
    }
}
