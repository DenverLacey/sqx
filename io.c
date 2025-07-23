#include "io.h"

#include <assert.h>
#include <errno.h>
#include "dstring.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DString read_entire_file(const char *filepath) {
    DString contents = {0};

    FILE *f = fopen(filepath, "r");
    ensure(f, "Failed to open file: %s", strerror(errno));

    ensure(fseek(f, 0, SEEK_END) == 0, strerror(errno));

    size_t len = ftell(f);

    rewind(f);

    contents.capacity = len;
    contents.chars = malloc((len + 1) * sizeof(char));
    assert(contents.chars);

    fread(contents.chars, sizeof(char), len, f);
    ensure(ferror(f) == 0, "Failed to read file: %d", ferror(f));

    contents.chars[len] = '\0';
    contents.length = len;

catch:
    if (erred()) dsfree(&contents);
    if (f) fclose(f);
    return contents;
}

