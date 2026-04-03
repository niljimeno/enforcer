/* drawing borders (needs optimizations) */
void drawBorder(struct Node* focusedNode) {
    if (focusedNode==NULL) return;


    struct Node* node = getCurrentWorkspace()->node;

    while (node) {
        XSetWindowBorderWidth(display, node->window, monocleMode ? 0 : 1);

        if (node == focusedNode && !monocleMode) {
            XSetWindowBorder(display, node->window, colorBorderSelected);
        } else {
            XSetWindowBorder(display, node->window, colorBorder);
        }

        node = node->next;
    }
}
