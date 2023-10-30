#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

#define ARGS_ERROR 1
#define FOPEN_ERROR 2
#define FREAD_ERROR 3
#define FWRITE_ERROR 4
#define MALLOC_ERROR 5
#define UNLINK_ERROR 6

void closeFiles(FILE *inputFile, FILE *outputFile) {
    fclose(inputFile);
    fclose(outputFile);
}

int checkFiles(FILE *inputFile, FILE *outputFile) {
    int statusCode = 0;

    if (!inputFile) {
        fprintf(stderr, "Failed to open input file: %s\n", strerror(errno));
        statusCode = 1;
    }
    if (!outputFile) {
        fprintf(stderr, "Failed to open output file: %s\n", strerror(errno));
        statusCode = 1;
    }

    return statusCode;
}

int readContent(FILE *file, char *buf, size_t *count) {
    *count = fread(buf, sizeof(char), BUF_SIZE, file);

    if (*count != BUF_SIZE && !feof(file)) {
        return 1;
    }
    return 0;
}

int writeContent(FILE *file, char *buf, size_t *count) {
    size_t written = fwrite(buf, sizeof(char), *count, file);

    if (*count != written) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    if (argc == 1) {
        printf("move - moves file1 to file2\n");
        printf("usage: move file1 file2\n");
        return 0;
    } else if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return ARGS_ERROR;
    }

    char *inputFileName = argv[1], *outputFileName = argv[2];

    FILE *inputFile = fopen(inputFileName, "r"),
            *outputFile = fopen(outputFileName, "w");

    if (checkFiles(inputFile, outputFile)) {
        return FOPEN_ERROR;
    }

    char *buf = (char *)malloc(BUF_SIZE * sizeof(char));
    if (buf == NULL) {
        fprintf(stderr, "Allocation error: %s\n", strerror(errno));

        closeFiles(inputFile, outputFile);
        remove(outputFileName);
        return MALLOC_ERROR;
    }

    size_t count;
    while (!feof(inputFile)) {
        if (readContent(inputFile, buf, &count)) {
            fprintf(stderr,
                    "Something went wrong while reading from input file: %s\n",
                    strerror(errno));
            closeFiles(inputFile, outputFile);
            free(buf);
            remove(outputFileName);
            return FREAD_ERROR;
        }

        if (writeContent(outputFile, buf, &count)) {
            fprintf(stderr, "Something went wront while writing to output file: %s\n",
                    strerror(errno));
            closeFiles(inputFile, outputFile);
            free(buf);
            remove(outputFileName);
            return FWRITE_ERROR;
        }
    }

    if (ferror(inputFile)) {
        fprintf(stderr, "Something went wrong while reading input file: %s\n",
                strerror(errno));
        closeFiles(inputFile, outputFile);
        free(buf);
        remove(outputFileName);
        return FREAD_ERROR;
    } else if (feof(inputFile)) {
        fprintf(stdout, "Contents of input file successfully moved.\n");
    }

    free(buf);
    closeFiles(inputFile, outputFile);

    if (unlink(inputFileName)) {
        fprintf(stderr, "Something went wrong while remove input file: %s\n",
                strerror(errno));
        return UNLINK_ERROR;
    }

    return 0;
}
