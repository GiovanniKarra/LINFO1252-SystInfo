#include <stdio.h>


void lock(int *verrou) {
    while (test_and_set(verrou, 1));
}

void lock2(int *verrou) {
    while (test_and_set(verrou, 1)) {
        while (*verrou);
    }
}

void unlock(int *verrou) {
    test_and_set(verrou, 0);
}

int test_and_set(int *verrou, int value) {
    int to_return = value;
    __asm__(
        "xchgl %0, %1\n\t"
        : "+r" (to_return), "+r" (*verrou)
    );
    return to_return == value;
}


int main(int argc, char const *argv[]) {
    int v = 0;
    printf("%d\n", v);
    test_and_set(&v, 1);
    printf("%d\n", v);
    return 0;
}
