#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef int (*real_unlink_t)(const char *);

int base_unlink(const char *path) {
    return ((real_unlink_t)dlsym(RTLD_NEXT, "unlink"))(path);
}

int unlink(const char *path) {
    if (strstr(path, "PROTECT") == NULL) {
        return base_unlink(path);
    }

    return 0;
}
