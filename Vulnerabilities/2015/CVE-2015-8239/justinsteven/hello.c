#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    uid_t uid = getuid();
    printf("Hello uid=%d\n", uid);
}
