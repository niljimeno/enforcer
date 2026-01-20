int countWorkspaceWindows(struct Workspace* ws) {
    struct Node* node = ws->node;
    int windowCount = 0;

    while (node) {
        if (!(node->isFloating) && node->isAlive)
            ++windowCount;

        node = node->next;
    }

    return windowCount;
}

/* apply resizing to all windows in the workspace */
void resizeWindows() {
    printf("Initiate: resize windows\n");
    int gap = 15;

    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen)-gap;
    int height = DisplayHeight(display, screen)-gap;

    struct Workspace* ws = getCurrentWorkspace();
    printf("Trying to resize here. Workspace tag: %d\n", ws->tag);
    struct Node* node = ws->node;
    if (node == NULL) return;

    int windowCount = countWorkspaceWindows(ws);
    if (windowCount == 0) return;

    int columns = windowCount;
    int rows = 1;

    int csize = width/columns;
    int rsize = height/rows;
    printf("count: %d\n", windowCount);
    printf("columns: %d\n", columns);
    printf("colsize: %d\n", csize);

    node = ws->node;

    int i = 0;
    while (node) {
        printf("doing checks\n");
        if (node->isFloating || !(node->isAlive)) {
            node = node->next;
            continue;
        }
        printf("finished checks\n");

        printf("c: %d, ", csize);
        printf("p: %d\n", csize*i);
        resizeWindow(node->window,
                     gap+(csize*i),
                     gap,// (rsize*(rows > 1 ? ((i-2)%2) : 0)),
                     csize-gap,
                     rsize-gap);

        node = node->next;
        ++i;
    }
    printf("Terminate: resize windows\n");
}

void changeFocus(int step) {
    printf("Initiate: change focus\n");

    Window currentWindow = getCurrentWindow();

    struct Workspace* ws = getCurrentWorkspace();
    printf("Trying to refocus here. Workspace tag: %d\n", ws->tag);
    printf("Number of windows: %d\n", countWorkspaceWindows(ws));
    struct Node* lastNode = ws->node;
    struct Node* lastValid = NULL;
    struct Node* newFocus = NULL;

    printf("recieved %d\n", step);

    if (lastNode == NULL) return;
    while (lastNode) {
        if (lastNode->window == currentWindow) {
            if (step == -1) {
                if (lastValid == NULL) {
                    while (lastNode != NULL) {
                        if (lastNode->isAlive) {
                            lastValid = lastNode;
                        }
                        lastNode = lastNode->next;
                    }
                    newFocus = lastValid;
                } else {
                    newFocus = lastValid;
                }
            } else if (step == +1) {
                if (lastNode->next == NULL) {
                    newFocus = ws->node;
                } else {
                    newFocus = lastNode->next;
                }
            }

            break;
        }

        if (lastNode->isAlive) {
            lastValid = lastNode;
        }

        lastNode = lastNode->next;
    }

    if (!(newFocus->isAlive)) {
        return;
    }

    focusWindow(newFocus->window);
    printf("Terminate: change focus\n");
}

/*
struct Dimensions {
    int x;
    int y;
    int w;
    int h;
};

struct Placeholder {
    struct Placeholder* next;
    struct Dimensions dimensions;
};

struct Placeholder* defaultPlaceholder() {
    struct Placeholder* p = malloc(sizeof(struct Placeholder));
    p->next = 0;
    p->dimensions.x = 20;
    p->dimensions.y = 20;
    p->dimensions.w = 400;
    p->dimensions.h = 200;
    return p;
}

struct Placeholder* getSizes(int windowCount) {
    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    int columns = 1;
    int rows = 1;

    if (windowCount > 1)
        columns = 2;

    if (windowCount > 2)
        rows = 2;

    int csize = width/columns;
    int rsize = height/rows;

    struct Placeholder* p = NULL;
    struct Placeholder* last;

    for (int i = 0; i<windowCount; ++i) {
        if (p == NULL) {
            p = malloc(sizeof(struct Placeholder));
            last = p;
        } else {
            last->next = malloc(sizeof(struct Placeholder));
        }

        last->next = 0;
        last->dimensions.x = 0+(csize*(i%2));
        last->dimensions.y = 0+(rsize*(rows > 1 ? ((i-2)%2) : 0));
        last->dimensions.w = csize;
        last->dimensions.h = rsize;

        last = p->next;
    }

    return p;
}
*/
