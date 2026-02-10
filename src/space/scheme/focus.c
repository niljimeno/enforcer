void setFocus(struct Node* focus) {
    focusedNode = focus;
    focusWindow(focus->window);
    drawBorder(focus);
}

bool isValid(struct Node* node) {
    return (node && node->isAlive/* && node->isVisible*/);
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
    struct Workspace* ws = getCurrentWorkspace();
    struct Node* lastNode = ws->node;
    struct Node* newFocus = NULL;

    if (!focusedNode) {
        printf("No focused node\n");
        lastNode = ws->node;
        while (lastNode) {
            if (isValid(lastNode)) {
                newFocus = lastNode;
            }
            lastNode = lastNode->next;
        }
    } else if (step == +1) {
        newFocus = getNext(focusedNode->next, focusedNode);
    } else if (step == -1) {
        newFocus = getPrevious(focusedNode->next, focusedNode);
    }

    if (!isValid(newFocus)) {
        return;
    }

    setFocus(newFocus);
}

/* Changes focus for the last window in the workspace */
void restoreFocus(struct Node* lastValid) {
    if (lastValid != NULL) {
        setFocus(lastValid);
    }
}
