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
