#ifndef _ARGITER_H_
#define _ARGITER_H_

#include <stdbool.h>

typedef struct Args {
	int length;
	const char **args;
} Args;

Args args_create(int argc, const char **argv);
const char *args_peek(Args *iter);
const char *args_next(Args *iter);
void args_back(Args *iter);
bool args_match(Args *iter, const char *arg);

#endif // _ARGITER_H_

