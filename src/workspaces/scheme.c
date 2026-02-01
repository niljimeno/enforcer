int countWorkspaceWindows(struct Workspace* ws) {
    struct Node* node = ws->node;
    int windowCount = 0;

    while (node) {
        if (!(node->isFloating) && node->isAlive && node->visible)
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
        if (node->isFloating || !(node->isAlive) || !(node->visible)) {
            node = node->next;
            continue;
        }
        printf("finished checks\n");

        node->transform.x = gap+(csize*i);
        node->transform.y = gap;
        node->transform.width = csize-gap;
        node->transform.height = rsize-gap;

        printf("c: %d, ", csize);
        printf("p: %d\n", csize*i);
        resizeWindow(node->window,
                     node->transform.x,
                     node->transform.y,
                     node->transform.width,
                     node->transform.height);


        node = node->next;
        ++i;
    }

    XFlush(display);
    printf("Terminate: resize windows\n");
}

void drawBorder(struct Node* focusedNode) {
    printf("Initialise: draw border\n");

    XClearWindow(display, root);

    printf("== IS IT NULL ==\n");
    if (focusedNode==NULL) return;

    GC gc = DefaultGC(display, 0);
    XSetForeground(display, gc, 0xFFFFFF);

    printf("%d, %d, %d, %d\n", focusedNode->transform.x,
                               focusedNode->transform.y,
                               focusedNode->transform.width,
                               focusedNode->transform.height);

    XFillRectangle(display, root, DefaultGC(display, 0), focusedNode->transform.x-1, focusedNode->transform.y-1, focusedNode->transform.width+1, 1);
    XFillRectangle(display, root, DefaultGC(display, 0), focusedNode->transform.x-1, focusedNode->transform.y-1, 1, focusedNode->transform.height+1);
    XFillRectangle(display, root, DefaultGC(display, 0), focusedNode->transform.x+focusedNode->transform.width, focusedNode->transform.y-1, 1, focusedNode->transform.height+1);
    XFillRectangle(display, root, DefaultGC(display, 0), focusedNode->transform.x, focusedNode->transform.y+focusedNode->transform.height, focusedNode->transform.width+1,1);

    printf("Terminate: draw border\n");
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
    printf("i updated, i promise\n");
    printf("%p and %d\n", (void*)lastNode, lastNode==NULL);
    printf("i updated, i promise\n");

    if (lastNode == NULL) return;
    printf("It's not null, somehow\n");
    while (lastNode) {
        if (!(lastNode->isAlive) || !(lastNode->visible)) {
            lastNode = lastNode->next;
            continue;
        }
        if (lastNode->window == currentWindow) {
            if (step == -1) {
                printf("it is -1\n");
                if (lastValid == NULL) {
                    printf("we're doing this??\n");
                    while (lastNode != NULL) {
                        printf("%p and %d\n", (void*)lastNode, lastNode==NULL);
                        if (lastNode->isAlive && lastNode->visible) {
                            lastValid = lastNode;
                        }
                        lastNode = lastNode->next;
                        printf("i'm okay\n");
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

        if (lastNode->isAlive && lastNode->visible) {
            lastValid = lastNode;
        }

        lastNode = lastNode->next;
    }

    if (!(newFocus) || !(newFocus->isAlive) || !(newFocus->visible)) {
        return;
    }

    focusWindow(newFocus->window);
    drawBorder(newFocus);
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
