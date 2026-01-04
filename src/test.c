union A{
    int x;
    char y;
};

union B{
    int arr[10];
    char y;
};

void test() {
    union A a;
    a.x = 5;
    printf("%d\n", a.x);
    setUpWorkspaces();
}
