#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf(">>> Uruchomiono program!\n");
    printf("PID procesu: %d\n", getpid());

    if (argc > 1) {
        printf("Otrzymane argumenty:\n");
        for (int i = 0; i < argc; i++) {
            printf("  argv[%d] = %s\n", i, argv[i]);
        }
    } else {
        printf("Brak argumentow.\n");
    }

    printf("------------------------------\n");
    return 0;
}
