struct Dimensions {
    int x;
    int y;
    int w;
    int h;
};

struct Placeholder {
    struct Placeholder* next;
    struct Dimensions dimensions;
};

struct Placeholder* defaultPlaceholder() {
    struct Placeholder* p = malloc(sizeof(struct Placeholder));
    p->next = 0;
    p->dimensions.x = 20;
    p->dimensions.y = 20;
    p->dimensions.w = 400;
    p->dimensions.h = 200;
    return p;
}
