bool isValid(struct Node* node) {
    return (node && node->isAlive/* && node->isVisible*/);
}

void setFocus(struct Node* focus) {
    if (!(isValid(focus))) return;
    if (focusedNode) {
        getCurrentWorkspace()->previous = focusedNode;
    }

    focusedNode = focus;
    focusWindow(focus->window);
    drawBorder(focus);
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
