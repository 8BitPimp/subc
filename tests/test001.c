char text1[] = "Hello World!";

char text2[] = "Hello World!";

int match(char *a, char *b) {
    int i = 0;

    for (i = 0; ; ++i) {
        if (a[i] != b[i]) {
            return 1;
        }
        if (a[i] == 0) {
            return 0;
        }
    }
}

int Start() {
    return match(text1, text2);
}
