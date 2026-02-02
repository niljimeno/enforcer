int countWorkspaceWindows(struct Workspace* ws) {
    struct Node* node = ws->node;
    int windowCount = 0;

    while (node) {
        if (!(node->isFloating) && node->isAlive && node->isVisible)
            ++windowCount;

        node = node->next;
    }

    return windowCount;
}

/* apply resizing to all windows in the workspace */
void resizeWindows() {
    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen)-gap;
    int height = DisplayHeight(display, screen)-gap;

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;
    if (node == NULL) return;

    int windowCount = countWorkspaceWindows(ws);
    if (windowCount == 0) return;

    int columns = windowCount;
    int rows = 1;

    int csize = width/columns;
    int rsize = height/rows;

    node = ws->node;

    int i = 0;
    while (node) {
        if (node->isFloating || !(node->isAlive) || !(node->isVisible)) {
            node = node->next;
            continue;
        }

        node->transform.x = gap+(csize*i);
        node->transform.y = gap;
        node->transform.width = csize-gap;
        node->transform.height = rsize-gap;

        resizeWindow(node->window,
                     node->transform.x,
                     node->transform.y,
                     node->transform.width,
                     node->transform.height);


        node = node->next;
        ++i;
    }

    XFlush(display);
}

void drawBorder(struct Node* focusedNode) {
    if (focusedNode==NULL) return;


    struct Node* node = getCurrentWorkspace()->node;

    while (node) {
        XSetWindowBorderWidth(display, focusedNode->window, 1);
        if (node == focusedNode) {
            XSetWindowBorder(display, node->window, colorBorderSelected);
        } else {
            XSetWindowBorder(display, node->window, colorBorder);
        }

        node = node->next;
    }
}


void changeFocus(int step) {
    Window currentWindow = getCurrentWindow();

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    struct Node* lastValid = NULL;
    struct Node* newFocus = NULL;

    if (lastNode == NULL) return;
    while (lastNode) {
        if (!(lastNode->isAlive) || !(lastNode->isVisible)) {
            lastNode = lastNode->next;
            continue;
        }
        if (lastNode->window == currentWindow) {
            if (step == -1) {
                if (lastValid == NULL) {
                    while (lastNode != NULL) {
                        if (lastNode->isAlive && lastNode->isVisible) {
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

        if (lastNode->isAlive && lastNode->isVisible) {
            lastValid = lastNode;
        }

        lastNode = lastNode->next;
    }

    if (!(newFocus) || !(newFocus->isAlive) || !(newFocus->isVisible)) {
        return;
    }

    focusWindow(newFocus->window);
    drawBorder(newFocus);
}
