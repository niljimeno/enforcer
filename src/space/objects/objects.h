struct Transform {
    int x;
    int y;
    int width;
    int height;
};

struct Node {
    Window window;
    struct Node* next;
    bool isFloating;
    bool isAlive; // if it's about to get closed
    bool isVisible;
    bool isMapped;
    bool isNew;
    bool isTransitioning;
    struct Transform transform;
};

struct Workspace {
    struct Workspace* next;
    struct Node* node;
    struct Node* previous;
    int tag;
};

struct Workspace *workspace;
int currentWorkspaceIndex = 0;
struct Node* focusedNode;
