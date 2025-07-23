#include "errors.h"
#include "dstring.h"
#include <stdarg.h>
#include "keywords.h"

thread_local DString current_error_message = {0};

void reseterror(void) {
    dsclear(&current_error_message);
}

void seterror(const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vseterror(fmt, args);
    va_end(args);
}

void vseterror(const char *restrict fmt, va_list args) {
    reseterror();
    vdsprintf(&current_error_message, fmt, args);
}

bool erred(void) {
    return current_error_message.length != 0;
}

const char *geterror(void) {
    return current_error_message.chars;
}

