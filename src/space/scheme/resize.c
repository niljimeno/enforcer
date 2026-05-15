Bool eligibleForResize(struct Node* node) {
    return (/*!(node->isFloating) &&*/ node->isAlive && node->isVisible);
}

int countWorkspaceWindows(struct Workspace* ws) {
    struct Node* node = ws->node;
    int windowCount = 0;

    while (node) {
        if (eligibleForResize(node))
            ++windowCount;

        node = node->next;
    }

    return windowCount;
}

/* apply resizing to all windows in the workspace */
void resizeWindows() {
    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* node = ws->node;
    if (node == NULL) return;

    int windowCount = countWorkspaceWindows(ws);
    if (windowCount == 0) return;

    int g;
    if (monocleMode) {
        g = 0;
    } else if (windowCount == 1) {
        g = gap;
    } else {
        g = 1;
    };

    int columns = windowCount > 1 ? 2 : 1;
    int rows = windowCount > 2 ? windowCount - 1 : 1;
    // int columns = (windowCount > 1) ? 2 : 1;
    // int rows = (1 + windowCount) / 2;

    int csize = (width-g)/columns;
    int hsize = height-g;
    int rsize = (height-g)/rows;

    node = ws->node;

    int col = 0;
    int row = 0;

    while (node) {
        if (!eligibleForResize(node)) {
            node = node->next;
            continue;
        }

        if (col == 0) {
            node->transform.x = csize*col;
            node->transform.y = 0;
            node->transform.width = csize;
            node->transform.height = hsize;
        } else {
            node->transform.x = csize*col;
            node->transform.y = rsize*row;
            node->transform.width = csize;
            node->transform.height = rsize;
        }


        if (monocleMode) {
            node->transform.x = 0;
            node->transform.y = 0;
            node->transform.width = width;
            node->transform.height = height;
        }

        int borderMultiplier = (g > 1) ? 1 : 2;

        resizeWindow(node->window,
                     node->transform.x + g,
                     node->transform.y + g,
                     node->transform.width - g*borderMultiplier,
                     node->transform.height - g*borderMultiplier);


        node = node->next;

        if (col == 0) {
            col = 1;
        } else {
            ++row;
        }
        // if (columns == col) {
        //     col = 0;
        //     ++row;

        //     if (row == 1 && extra) {
        //         printf("row %d\n", row);
        //         ++col;
        //     }
        // }
    }

    XFlush(display);
}
