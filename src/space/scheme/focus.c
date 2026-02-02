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

/* Changes focus for the last window in the workspace */
void restoreFocus(struct Node* lastValid) {
    if (lastValid != NULL)
        focusWindow(lastValid->window);
}


