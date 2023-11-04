#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "rhash.h"

#ifdef READLINE_ENABLED
#include "readline/readline.h"
#endif

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

#define MAX_HASH_LEN 1024 // взял с запасом


void repl() {
    char *cmd = NULL;
    size_t cmd_size = 0;

    unsigned char *hashed_string = calloc(MAX_HASH_LEN, sizeof(unsigned char));
    char *output = calloc(MAX_HASH_LEN, sizeof(char));

#ifdef READLINE_ENABLED
    while (cmd = readline(NULL)) {
        cmd_size = strlen(cmd);
#else
    while (getline(&cmd, &cmd_size, stdin) > 0) {
#endif
        char *hash_name = strtok(cmd, " ");

        if (hash_name == NULL) {
            fprintf(stderr, "Program input should have 2 arguments separated by space (got 0)\n");
            continue;
        }


        char *data = strtok(NULL, "\n ");

        if (data == NULL) {
            fprintf(stderr, "Program input should have 2 arguments separated by space (got 1)\n");
            continue;
        }

        enum rhash_ids rhash_id = RHASH_HASH_COUNT;

        for (int i = 0; i < RHASH_HASH_COUNT; ++i) {
            if (!strcasecmp(hash_name, rhash_get_name(1 << i))) {
                rhash_id = (enum rhash_ids) 1 << i;
            }
        }

        if (rhash_id == RHASH_HASH_COUNT) {
            fprintf(stderr, "Hash type %s is not supported\n", hash_name);
            continue;
        }

#ifndef NDEBUG
        printf("hash_name - [%s]\nhash_id - [%d]\n", rhash_get_name(rhash_id), rhash_id);
#endif
        int hash_print_flags = RHPR_DEFAULT;

        if (isupper(hash_name[0])) {
            hash_print_flags |= RHPR_HEX;
        } else {
            hash_print_flags |= RHPR_BASE64;
        }

        if (data[0] != '"') {
            // handle data as input string for hash
            int res = rhash_file(rhash_id, data, hashed_string);

            if (res == -1) {
                fprintf(stderr, "LibRHash error: string - %s, error - %s\n", data, strerror(errno));
                continue;
            }
        } else {
            // handle data as file to hash
            data += 1;
            int res = rhash_msg(rhash_id, data, strlen(data), hashed_string);

            if (res == -1) {
                fprintf(stderr, "LibRHash error: file - %s, error - %s\n", data - 1, strerror(errno));
                continue;
            }
        }

        rhash_print_bytes(output, hashed_string, rhash_get_digest_size(rhash_id), hash_print_flags);
        printf("%s\n", output);

#ifdef READLINE_ENABLED
        free(cmd); // readline has no information about reallocation if string len is not sufficient so free memory and next time allocate again (it is bad but fast solution)
        cmd = NULL;
#endif
    }

    free(cmd);
    free(hashed_string);
    free(output);
}


int main() {
#ifdef READLINE_ENABLED
    printf("Program with readline\n");
#else
    printf("Program without readline\n");
#endif

    rhash_library_init();
    repl();
    return 0;
}
