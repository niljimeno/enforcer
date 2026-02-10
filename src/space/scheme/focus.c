bool isValid(struct Node* current) {
    return (current && current->isAlive && current->isVisible);
}

struct Node* getPrevious(struct Node* current, struct Node* final) {
    if (current == NULL) {
        current = getCurrentWorkspace()->node;
    }

    if (current == final) {
        return NULL;
    }

    struct Node* next = getPrevious(current->next, final);
    return isValid(next) ? next : current;
}

struct Node* getNext(struct Node* current, struct Node* final) {
    if (current == NULL) {
        current = getCurrentWorkspace()->node;
    }

    if (current == final) {
        return NULL;
    }

    return isValid(current) ? current : getNext(current->next, final);
}

void changeFocus(int step) {
    Window currentWindow = getCurrentWindow();

    struct Workspace* ws = getCurrentWorkspace();
    struct Node* currentNode = getLocalNode(currentWindow);
    struct Node* lastNode = ws->node;
    struct Node* newFocus = NULL;

    if (!currentNode) {
        lastNode = ws->node;
        while (lastNode) {
            if (isValid(lastNode)) {
                newFocus = lastNode;
            }
            lastNode = lastNode->next;
        }
    } else if (step == +1) {
        newFocus = getNext(currentNode->next, currentNode);
    } else if (step == -1) {
        newFocus = getPrevious(currentNode->next, currentNode);
    }

    if (!isValid(newFocus)) {
        return;
    }

    focusWindow(newFocus->window);
    drawBorder(newFocus);
}
/* Changes focus for the last window in the workspace */
void restoreFocus(struct Node* lastValid) {
    if (lastValid != NULL) {
        focusWindow(lastValid->window);
    }
}
