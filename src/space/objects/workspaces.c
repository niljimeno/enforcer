struct Workspace* getCurrentWorkspace() {
    struct Workspace* lastWS = workspace;

    for (int i=0; i<currentWorkspaceIndex; i++)
        lastWS = lastWS->next;

    return lastWS;
}

/* Append a workspace at the end of the list */
void createWorkspace() {
    struct Workspace* lastWS = workspace;
    struct Workspace* instance = malloc(sizeof(struct Workspace));
    instance->node = 0;
    instance->tag = 1;
    instance->next = NULL;
    instance->previous = NULL;

    if (lastWS==NULL) {
        workspace = instance;
        return;
    }

    (instance->tag)++;

    while (lastWS->next) {
        (instance->tag)++;
        lastWS = lastWS->next;
    }

    lastWS->next = instance;
}

void printWorkspaceInformation() {
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;

    printf("-- Printing workspace data --\n");

    if (ws->previous)
        printf("- Previous window: %ld\n\n", ws->previous->window);
    else
        printf("- Previous window: NA\n\n");

    while (node) {
        printf("== Node %ld ==\n", node->window);
        printf("- Alive: %d\n", node->isAlive);
        printf("- Visible: %d\n", node->isVisible);
        if (node->next != NULL)
            printf("- Next: %ld\n", node->next->window);
        else
            printf("- Next: ----\n");

        if (node == focusedNode) {
            printf("-= FOCUSED =-\n");
        }
        printf("\n");
        node = node->next;
    }
}
