#include "argiter.h"
#include <string.h>
#include <stdio.h>

Args args_create(int argc, const char **argv) {
    return (Args) {
        .length = argc,
        .args = argv
    };
}

const char *args_peek(Args *iter) {
    if (iter->length == 0) {
        return NULL;
    }

    return iter->args[0];
}

const char *args_next(Args *iter) {
    if (iter->length == 0) {
        return NULL;
    }

    iter->length--;
    return *iter->args++;
}

void args_back(Args *iter) {
    iter->length++;
    iter->args--;
}

bool args_match(Args *iter, const char *arg) {
    if (iter->length == 0) {
        return false;
    }

    if (strcmp(iter->args[0], arg) != 0) {
        return false;
    }

    args_next(iter);
    return true;
}

