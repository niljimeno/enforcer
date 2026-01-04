void resizeWindows() {
    int snum = DefaultScreen(display);
    int width = DisplayWidth(display, snum);
    int height = DisplayHeight(display, snum);

    struct Node* node = (getCurrentWorkspace())->node;
    if (node == NULL) return;

    int i = 0;
    while (true) {
        ++i;
        resizeWindow(node->window, 50+(i*10), 50+(i*10), width - 200, height - 200);
        if (node->next == NULL) {
            return;
        }

        node = node->next;
    }
}
