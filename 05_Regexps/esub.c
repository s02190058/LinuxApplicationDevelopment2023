#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define BUF_SIZE 256

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("Usage: %s REGEXP SUBSTITUTION STRING\n", argv[0]);
        return 0;
    }

    char *regexp = argv[1];
    char *substitution = argv[2];
    char *string = argv[3];

    regex_t regex;
    int code = regcomp(&regex, regexp, REG_EXTENDED);
    if (code != 0) {
        char buf[BUF_SIZE];
        regerror(code, &regex, buf, BUF_SIZE);
        fprintf(stderr, "%s: %s\n", argv[0], buf);
        return -2;
    }

    size_t nmatch = regex.re_nsub + 1;

    regmatch_t bags[nmatch];
    code = regexec(&regex, string, nmatch, bags, 0);
    if (code != 0) {
        char buf[BUF_SIZE];
        regerror(code, &regex, buf, BUF_SIZE);
        fprintf(stderr, "%s: %s\n", argv[0], buf);
        return -2;
    }

    regfree(&regex);

    size_t result_length = strlen(string) - (bags[0].rm_eo - bags[0].rm_so);
    size_t substitution_length = strlen(substitution);
    for (int i = 0; i < substitution_length; ++i) {
        if (substitution[i] == '\\') {
            if (i + 1 < substitution_length) {
                if (substitution[i + 1] >= '0' && substitution[i + 1] <= '9') {
                    int bag_number = substitution[i + 1] - '0';
                    if (bag_number >= nmatch) {
                        fprintf(stderr, "%s: invalid link to \\%d\n", argv[0], bag_number);
                        return 1;
                    }
                    result_length += bags[bag_number].rm_eo - bags[bag_number].rm_so;
                    ++i;
                    continue;
                }
                if (substitution[i + 1] == '\\') {
                    ++i;
                }
            }
        }
        ++result_length;
    }

    char *result = (char *) calloc(result_length + 1, sizeof(char));

    size_t current_length = bags[0].rm_so;

    strncpy(result, string, current_length);

    for (int i = 0; i < substitution_length; ++i) {
        if (substitution[i] == '\\') {
            if (i + 1 < substitution_length) {
                if (substitution[i + 1] >= '0' && substitution[i + 1] <= '9') {
                    int bag_number = substitution[i + 1] - '0';
                    size_t bag_length = bags[bag_number].rm_eo - bags[bag_number].rm_so;
                    strncpy(result + current_length, string + bags[bag_number].rm_so, bag_length);
                    current_length += bag_length;
                    ++i;
                    continue;
                }
                if (substitution[i + 1] == '\\') {
                    ++i;
                }
            }
        }
        result[current_length] = substitution[i];
        ++current_length;
    }

    strncpy(result+current_length, string + bags[0].rm_eo, result_length - current_length);

    printf("%s\n", result);

    free(result);

    return 0;
}