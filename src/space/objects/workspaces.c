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
