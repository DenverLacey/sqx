#include <argiter.h>
#include <config.h>
#include <errno.h>
#include <limits.h>
#include <macro_ops.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tests.h>
#include <unistd.h>

void usage(void) {
    fprintf(stderr, "Usage: test [-h] [record] [<tests...>]\n");
    fprintf(stderr, "    -h, --help: Prints this help message.\n");
    fprintf(stderr, "    record:     Records output of tests and saves it as the new.\n");
    fprintf(stderr, "                expected ouput for that test.\n");
    fprintf(stderr, "    tests...:   Name of the tests you wish to run.\n");

    #define X(test) fprintf(stderr, "        %s\n", STRINGIFY(test));
        LIST_OF_TESTS
    #undef X
}

bool file_does_not_exist(const char *path) {
    return access(path, F_OK) != 0;
}

void file_copy(FILE *dst, FILE *src) {
    char *line = NULL;
    size_t cap = 0;
    ssize_t len = 0;

    while ((len = getline(&line, &cap, src)) != -1) {
        fwrite(line, sizeof(char), len, dst);
    }

    free(line);
}

bool file_compare_and_report(FILE *exp, FILE *run) {
    char *exp_line = NULL;
    size_t exp_sz = 0;

    char *run_line = NULL;
    size_t run_sz = 0;

    bool result = true;

    int lineno = 1;
    for (;;) {
        if (feof(exp) || feof(run)) {
            break;
        }

        // TODO: handle errors
        getline(&exp_line, &exp_sz, exp);
        getline(&run_line, &run_sz, run);

        if (strcmp(exp_line, run_line) != 0) {
            fprintf(stderr, "Error: Line %d does not match.\n", lineno);
            fprintf(stderr, "    Expected: %s", exp_line);
            fprintf(stderr, "    Found:    %s", run_line);
            result = false;
        }

        lineno++;
    }

    free(exp_line);
    free(run_line);

    return result;
}

int main(int argc, const char **argv) {
    Args args = args_create(argc, argv);
    args_next(&args); // skip program name

    if (args_match(&args, "-h") || args_match(&args, "--help") || args_match(&args, "help")) {
        usage();
        return 0;
    }

    Config cfg = {0};
    if (args_match(&args, "record")) {
        cfg.recording = true;
    }

    if (args.length == 0) {
        // Run all tests if none specified
        args = args_create(TestIdx_COUNT, test_names);
    }

    int ntests = args.length;
    int npassed = 0;
    for (const char *arg = args_next(&args); arg != NULL; arg = args_next(&args)) {
        TestFn test_fn = get_test_fn(arg);
        if (test_fn == NULL) {
            fprintf(stderr, "Error: '%s' is not a known test.\n", arg);
            continue;
        }

        char run_filepath[PATH_MAX] = {0};
        snprintf(run_filepath, sizeof(run_filepath), "runs/%s.txt", arg);

        FILE *run_out = fopen(run_filepath, "w");
        if (!run_out) {
            const char *err = strerror(errno);
            fprintf(stderr, "Error: Failed to open file '%s': %s.\n", run_filepath, err);
            continue;
        }

        test_fn(run_out, cfg);
        fclose(run_out);

        char exp_filepath[PATH_MAX] = {0};
        snprintf(exp_filepath, sizeof(exp_filepath), "expected/%s.txt", arg);

        if (cfg.recording || file_does_not_exist(exp_filepath)) {
            FILE *run_in = NULL;
            FILE *exp_out = NULL;

            run_in = fopen(run_filepath, "r");
            if (!run_in) {
                const char *err = strerror(errno);
                fprintf(stderr, "Error: Failed to open file '%s': %s.\n", run_filepath, err);
                goto DONE_RECORD;
            }

            exp_out = fopen(exp_filepath, "w");
            if (!exp_out) {
                const char *err = strerror(errno);
                fprintf(stderr, "Error: Failed to open file '%s': %s.\n", exp_filepath, err);
                goto DONE_RECORD;
            }

            file_copy(exp_out, run_in);

        DONE_RECORD:
            if (run_in) fclose(run_in);
            if (exp_out) fclose(exp_out);
        } else {
            FILE *run_in = NULL;
            FILE *exp_in = NULL;

            run_in = fopen(run_filepath, "r");
            if (!run_in) {
                const char *err = strerror(errno);
                fprintf(stderr, "Error: Failed to open file '%s': %s.\n", run_filepath, err);
                goto DONE_COMPARE;
            }

            exp_in = fopen(exp_filepath, "r");
            if (!exp_in) {
                const char *err = strerror(errno);
                fprintf(stderr, "Error: Failed to open file '%s': %s.\n", exp_filepath, err);
                goto DONE_COMPARE;
            }

            char test_name[80] = {0};
            snprintf(test_name, sizeof(test_name), "\033[36m%s\033[m:", arg);

            bool ok = file_compare_and_report(exp_in, run_in);
            if (ok) {
                fprintf(stderr, "Test %-20s \033[32mPASSED!\033[m\n", test_name);
                npassed++;
            } else {
                fprintf(stderr, "Test %-20s \033[31mFAILED!\033[m\n", test_name);
            }

        DONE_COMPARE:
            if (run_in) fclose(run_in);
            if (exp_in) fclose(exp_in);
        }
    }

    fprintf(stderr, "%d/%d tests passed.\n", npassed, ntests);
}

