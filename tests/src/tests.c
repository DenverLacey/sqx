#include "tests.h"
#include "array.h"

#include <stdio.h>
#include <string.h>
#include <sqx.h>
#include <unistd.h>

const char *test_names[TestIdx_COUNT] = {
    #define X(test) [TestIdx_ ## test] = #test,
        LIST_OF_TESTS
    #undef X
};

const TestFn test_fns[TestIdx_COUNT] = {
    #define X(test) [TestIdx_ ## test] = test_ ## test,
        LIST_OF_TESTS
    #undef X
};

TestFn get_test_fn(const char *test) {
    for (int i = 0; i < TestIdx_COUNT; i++) {
        if (strcmp(test, test_names[i]) == 0) {
            return test_fns[i];
        }
    }
    return NULL;
}

void test_logging(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Log:\n");
    FLOG(out, "LOG", "This is a log");

    fprintf(out, "Info:\n");
    FINFO(out, "This is an info");

    fprintf(out, "Debug:\n");
    FDBG(out, "This is a debug");

    fprintf(out, "Error:\n");
    FERROR(out, "This is an error");
}

void test_alloc(FILE *out, UNUSED Config cgf) {
    tsinit(64 * sizeof(int));

    fprintf(out, "List:\n");
    List_int ints = {.allocator = tsallocator()};
    fprintf(out, "allocator = { %p, %s }\n", ints.allocator.allocator_data, (void*)ints.allocator.allocator_func ? "not null" : "null");

    list_prepend(&ints, 1);
    list_prepend(&ints, 2);
    list_prepend(&ints, 3);
    list_prepend(&ints, 4);
    list_prepend(&ints, 5);

    fprintf(out, "Values:\n");
    ListNode_int *iter = ints.first;
    int *n = NULL;
    while (list_iter(&iter, &n)) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    tsdeinit();
}

void test_array(FILE *out, UNUSED Config cfg) {
    // NOTE: This is non-gnu version that requires specifying the type
    // Array_int ns = arr_of(int, 1, 4, 9, 17, 28);

    Array_int ns = arr_of(1, 4, 9, 17, 28);

    fprintf(out, "Find:\n");
    fprintf(out, "find(1) = %d\n", arr_find(ns, 1));
    fprintf(out, "find(4) = %d\n", arr_find(ns, 4));
    fprintf(out, "find(7) = %d\n", arr_find(ns, 7));

    fprintf(out, "Last:\n");
    fprintf(out, "last = %d\n", arr_lst(ns));

    fprintf(out, "Foreach:\n");
    arr_foreach(n, ns) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Foreach Rev:\n");
    arr_foreach_rev(n, ns) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");
}

void test_args(FILE *out, UNUSED Config cfg) {
    const char *argv[] = {
        "program", "hello", "world", "123", "456", NULL
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    Args args = args_create(argc, argv);

    fprintf(out, "Next:\n");
    fprintf(out, "%s\n", args_next(&args));
    fprintf(out, "%s\n", args_next(&args));

    fprintf(out, "Back:\n");
    args_back(&args);
    args_back(&args);
    fprintf(out, "%s\n", args_next(&args));
    fprintf(out, "%s\n", args_next(&args));

    fprintf(out, "Peek:\n");
    fprintf(out, "%s\n", args_peek(&args));
    fprintf(out, "%s\n", args_peek(&args));
    fprintf(out, "%s\n", args_peek(&args));

    fprintf(out, "Match:\n");
    fprintf(out, "match('foo') = %d\n", args_match(&args, "foo"));
    fprintf(out, "match('bar') = %d\n", args_match(&args, "bar"));
    fprintf(out, "match('world') = %d\n", args_match(&args, "world"));
    fprintf(out, "match('123') = %d\n", args_match(&args, "123"));
    fprintf(out, "match('456') = %d\n", args_match(&args, "456"));

    fprintf(out, "Null:\n");
    fprintf(out, "peek = %s\n", args_peek(&args));
    fprintf(out, "next = %s\n", args_next(&args));
    fprintf(out, "peek = %s\n", args_peek(&args));
    fprintf(out, "next = %s\n", args_next(&args));
}

void test_dstring(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Copy:\n");
    DString s1 = dscopy("This string was copied.");
    fprintf(out, "s1 = '%s'\n", s1.chars);

    fprintf(out, "Copy N:\n");
    DString s2 = dscopyn("This string has been cut short.", 27);
    fprintf(out, "s2 = '%s'\n", s2.chars);

    fprintf(out, "Dup:\n");
    DString s3 = dsdup(s1);
    fprintf(out, "s3 = '%s'\n", s3.chars);

    fprintf(out, "Push:\n");
    dspush(&s3, '.');
    fprintf(out, "s3 = '%s'\n", s3.chars);

    fprintf(out, "Append:\n");
    dsappend(&s3, ". But we've added to it.");
    fprintf(out, "s3 = '%s'\n", s3.chars);

    fprintf(out, "Extend:\n");
    dsextend(&s3, s2);
    fprintf(out, "s3 = '%s'\n", s3.chars);

    fprintf(out, "Format:\n");
    DString s4 = dsformat("The length of s3 is %zu.", s3.length);
    fprintf(out, "s4 = '%s'\n", s4.chars);

    fprintf(out, "Free:\n");
    dsfree(&s1);
    dsfree(&s2);
    dsfree(&s3);
    dsfree(&s4);
    fprintf(out, "s1 = '%s'\n", s1.chars);
    fprintf(out, "s2 = '%s'\n", s2.chars);
    fprintf(out, "s3 = '%s'\n", s3.chars);
    fprintf(out, "s4 = '%s'\n", s4.chars);
}

typedef enum Enum {
    Enum_ZERO,
    Enum_ONE,
    Enum_TWO,
    Enum_THREE,
} Enum;

void test_eflags(FILE *out, UNUSED Config cfg) {
    EFlags(Enum) flags = 0;
    fprintf(out, "Initialize:\n");
    fprintf(out, "flags = %d\n", flags);

    fprintf(out, "Set:\n");
    efset(&flags, Enum_ZERO);
    efset(&flags, Enum_TWO);
    fprintf(out, "flags = 0x%x\n", flags);

    fprintf(out, "Clear:\n");
    efclear(&flags, Enum_ZERO);
    efclear(&flags, Enum_ONE);
    fprintf(out, "flags = 0x%x\n", flags);

    fprintf(out, "Toggle:\n");
    eftoggle(&flags, Enum_ONE);
    eftoggle(&flags, Enum_TWO);
    eftoggle(&flags, Enum_THREE);
    fprintf(out, "flags = 0x%x\n", flags);

    fprintf(out, "Check:\n");
    fprintf(out, "Enum_ZERO = %d\n", efcheck(flags, Enum_ZERO));
    fprintf(out, "Enum_ONE = %d\n", efcheck(flags, Enum_ONE));
    fprintf(out, "Enum_TWO = %d\n", efcheck(flags, Enum_TWO));
    fprintf(out, "Enum_THREE = %d\n", efcheck(flags, Enum_THREE));

    fprintf(out, "Count:\n");
    fprintf(out, "#flags = %d\n", efcount(flags));
}

static void test_errors_fail(void) throws {
    fail("Error occured in %s", __func__);
catch:
    return;
}

static void test_errors_try_void_func(void) throws {
    seterror("%s failed", __func__);
}

static int test_errors_try_int_func(void) throws {
    seterror("%s failed", __func__);
    return 0;
}

static void test_errors_try_void(void) throws {
    try(test_errors_try_void_func());
catch:
    return;
}

static void test_errors_try_non_void(void) throws {
    UNUSED int n = try(test_errors_try_int_func());
catch:
    return;
}

static void test_errors_ensure(void) throws {
    ensure(false, "Oh no! This has gone so badly wrong!");
catch:
    return;
}

void test_errors(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Fail:\n");
    test_errors_fail();
    if (erred()) fprintf(out, "%s\n", geterror());
    else         fprintf(out, "No error occured\n");

    fprintf(out, "Try - Void:\n");
    test_errors_try_void();
    if (erred()) fprintf(out, "%s\n", geterror());
    else         fprintf(out, "No error occured\n");

    fprintf(out, "Try - Non-Void:\n");
    test_errors_try_non_void();
    if (erred()) fprintf(out, "%s\n", geterror());
    else         fprintf(out, "No error occured\n");

    fprintf(out, "Ensure:\n");
    test_errors_ensure();
    if (erred()) fprintf(out, "%s\n", geterror());
    else         fprintf(out, "No error occured\n");
}

void test_hashmap(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Initialize:\n");
    HashMap map = hmnew(int);
    fprintf(out, "length = %zu\n", map.length);
    fprintf(out, "capacity = %zu\n", map.capacity);
    fprintf(out, "data is%s null\n", (map.data == NULL ? "" : " not"));

    fprintf(out, "Put:\n");
    hmput(&map, "one", 1);
    hmput(&map, "two", 2);
    hmput(&map, "three", 3);
    // hmput(&map, "one", 99);
    fprintf(out, "length = %zu\n", map.length);
    fprintf(out, "capacity = %zu\n", map.capacity);
    fprintf(out, "entries = ");
    const char **keys = hmkeys(map);
    int *values = (int*)hmvalues(map);
    for (size_t i = 0; i < map.length; i++) {
        const char *key = keys[i];
        int value = values[i];
        fprintf(out, "('%s': %d) ", key, value);
    }
    fprintf(out, "\n");

    fprintf(out, "Get:\n");
    int *g1 = (int*)hmget(map, "one");
    int *g2 = (int*)hmget(map, "two");
    int *g9 = (int*)hmget(map, "nine");
    if (g1) fprintf(out, "'one' = %d\n", *g1);  else fprintf(out, "'one' = (null)\n");
    if (g2) fprintf(out, "'two' = %d\n", *g2);  else fprintf(out, "'two' = (null)\n");
    if (g9) fprintf(out, "'nine' = %d\n", *g9); else fprintf(out, "'nine' = (null)\n");

    fprintf(out, "Contains:\n");
    bool c1 = hmcontains(map, "one");
    bool c2 = hmcontains(map, "two");
    bool c9 = hmcontains(map, "nine");
    fprintf(out, "'one' = %d\n", c1);
    fprintf(out, "'two' = %d\n", c2);
    fprintf(out, "'nine' = %d\n", c9);

    fprintf(out, "Remove:\n");
    bool r1 = hmremove(&map, "one");
    bool r2 = hmremove(&map, "nine");
    fprintf(out, "remove(one) = %d\n", r1);
    fprintf(out, "remove(nine) = %d\n", r2);
    fprintf(out, "entries = ");
    for (size_t i = 0; i < map.length; i++) {
        const char *key = keys[i];
        int value = values[i];
        fprintf(out, "('%s': %d) ", key, value);
    }
    fprintf(out, "\n");

    fprintf(out, "Iter:\n");
    const char *key = NULL;
    int *value = NULL;
    HashMapIter iter = hmiter(map);
    while (hmnext(&iter, &key, (void*)&value)) {
        fprintf(out, "%s: %d\n", key, *value);
    }

    fprintf(out, "Free:\n");
    hmfree(&map);
    fprintf(out, "length = %zu\n", map.length);
    fprintf(out, "capacity = %zu\n", map.capacity);
    fprintf(out, "data = %p\n", (void*)map.data);
}

void test_io(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Reading File:\n");
    DString f = read_entire_file("resources/test_io.txt");
    fprintf(out, "%s\n", f.chars);
}

void test_list(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Initialise:\n");
    List_int list = {0};
    fprintf(out, "length = %zu\n", list.length);
    fprintf(out, "first = %p\n", (void*)list.first);

    fprintf(out, "Prepend:\n");
    ListNode_int *n1 = list_prepend(&list, 1);
    ListNode_int *n2 = list_prepend(&list, 2);
    ListNode_int *n3 = list_prepend(&list, 3);
    ListNode_int *n4 = list_prepend(&list, 4);
    ListNode_int *n5 = list_prepend(&list, 5);
    fprintf(out, "n1 = %d\n", n1->item);
    fprintf(out, "n2 = %d\n", n2->item);
    fprintf(out, "n3 = %d\n", n3->item);
    fprintf(out, "n4 = %d\n", n4->item);
    fprintf(out, "n5 = %d\n", n5->item);

    fprintf(out, "Get:\n");
    int i0 = list_get(list, 0);
    int i2 = list_get(list, 2);
    int i4 = list_get(list, 4);
    fprintf(out, "i0 = %d\n", i0);
    fprintf(out, "i2 = %d\n", i2);
    fprintf(out, "i4 = %d\n", i4);

    fprintf(out, "Iter:\n");
    ListNode_int *iter = list.first;
    int *n = NULL;
    while (list_iter(&iter, &n)) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Free:\n");
    list_free(&list);
    fprintf(out, "length = %zu\n", list.length);
    fprintf(out, "first = %p\n", (void*)list.first);
}

void test_sstring(FILE *out, UNUSED Config cfg) {
    SString s1 = {0};

    fprintf(out, "Initialize:\n");
    fprintf(out, "sslen = %zu\n", sslen(&s1));
    fprintf(out, "ssempty = %d\n", ssempty(&s1));

    fprintf(out, "Copy:\n");
    SString s2 = sscopy("Copied from C string.");
    fprintf(out, "s2 = '%s'\n", s2.chars);
    SString s3 = sscopy("This string is too long to fit in an SString.");
    fprintf(out, "s3 = '%s'\n", s3.chars);

    fprintf(out, "Format:\n");
    SString s4 = ssformat("A number: %d", 99);
    fprintf(out, "s4 = '%s'\n", s4.chars);

    fprintf(out, "Printf:\n");
    SString s5 = {0};
    ssprintf(&s5, "A string: '%s'", "wow!");
    fprintf(out, "s5 = '%s'\n", s5.chars);

    fprintf(out, "Clear:\n");
    ssclear(&s1);
    ssclear(&s2);
    ssclear(&s3);
    ssclear(&s4);
    ssclear(&s5);
    fprintf(out, "s1 = '%s'\n", s1.chars);
    fprintf(out, "s2 = '%s'\n", s2.chars);
    fprintf(out, "s3 = '%s'\n", s3.chars);
    fprintf(out, "s4 = '%s'\n", s4.chars);
    fprintf(out, "s5 = '%s'\n", s5.chars);
}

void test_temp(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Initialize:\n");
    tsinit(32);
    fprintf(out, "buffer = %s\n", tsgetbuffer() == NULL ? "null" : "not null");
    fprintf(out, "size = %zu\n", tsgetsize());
    fprintf(out, "allocated = %zu\n", tsgetallocated());
    fprintf(out, "high_water_mark = %zu\n", tsgetwatermark());

    fprintf(out, "Allocation:\n");
    int *n1 = tsmake(int, 1);
    int *n2 = tsmake(int, 2);
    int *n3 = tsmake(int, 3);
    fprintf(out, "n1 = %d\n", *n1);
    fprintf(out, "n2 = %d\n", *n2);
    fprintf(out, "n3 = %d\n", *n3);

    fprintf(out, "Mark:\n");
    size_t mark = tsmark();
    fprintf(out, "mark = %zu\n", mark);
    fprintf(out, "allocated = %zu\n", tsgetallocated());

    fprintf(out, "Allocation - Array:\n");
    int *ns = tsnewn(int, 5);
    for (int i = 0; i < 5; i++) ns[i] = i + 3;
    fprintf(out, "ns = ");
    for (int i = 0; i < 5; i++) fprintf(out, "%d ", ns[i]);
    fprintf(out, "\n");

    fprintf(out, "Allocating - At Capacity:\n");
    fprintf(out, "size = %zu\n", tsgetsize());
    fprintf(out, "allocated = %zu\n", tsgetallocated());
    int *nn = tsnew(int);
    fprintf(out, "nn = %p\n", (void*)nn);

    fprintf(out, "Rollback:\n");
    tsrollback(mark);
    fprintf(out, "mark = %zu\n", mark);
    fprintf(out, "allocated = %zu\n", tsgetallocated());

    fprintf(out, "Reset:\n");
    tsreset();
    fprintf(out, "allocated = %zu\n", tsgetallocated());

    fprintf(out, "Deinit:\n");
    tsdeinit();
    fprintf(out, "buffer = %s\n", tsgetbuffer() == NULL ? "null" : "not null");
    fprintf(out, "size = %zu\n", tsgetsize());
    fprintf(out, "allocated = %zu\n", tsgetallocated());
    fprintf(out, "high_water_mark = %zu\n", tsgetwatermark());
}

void test_thin_string(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Initialize:\n");
    ThinString s = ths_new();
    fprintf(out, "length = %zu\n", s->length);
    fprintf(out, "capacity = %zu\n", s->capacity);
    fprintf(out, "allocator = %p\n", (void*)s->allocator.allocator_func);
    fprintf(out, "bytes = %s\n", s->bytes);

    fprintf(out, "Append:\n");
    ths_append(&s, "Hello from test_thin_string");
    fprintf(out, "length = %zu\n", s->length);
    fprintf(out, "capacity = %zu\n", s->capacity);
    fprintf(out, "bytes = %s\n", s->bytes);

    fprintf(out, "Clear:\n");
    ths_clear(s);
    fprintf(out, "length = %zu\n", s->length);
    fprintf(out, "capacity = %zu\n", s->capacity);
    fprintf(out, "bytes = %s\n", s->bytes);

    fprintf(out, "Free:\n");
    ths_free(&s);
    fprintf(out, "s = %p\n", (void*)s);
}

void test_vec(FILE *out, UNUSED Config cfg) {
    fprintf(out, "Initialize:\n");
    Vec_int vec = {0};
    fprintf(out, "length = %zu\n", vec.length);
    fprintf(out, "capacity = %zu\n", vec.capacity);
    fprintf(out, "items = %p\n", (void*)vec.items);

    fprintf(out, "Reserve:\n");
    vec_reserve(&vec, 5);
    fprintf(out, "length = %zu\n", vec.length);
    fprintf(out, "capacity = %zu\n", vec.capacity);
    fprintf(out, "items = %s\n", vec.items == NULL ? "null" : "not null");

    fprintf(out, "Append:\n");
    for (int i = 0; i < 5; i++) {
        vec_append(&vec, i + 1);
    }
    fprintf(out, "length = %zu\n", vec.length);
    fprintf(out, "capacity = %zu\n", vec.capacity);
    fprintf(out, "items = %s\n", vec.items == NULL ? "null" : "not null");

    fprintf(out, "Foreach:\n");
    fprintf(out, "vec = ");
    vec_foreach(n, vec) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Insert:\n");
    vec_insert(&vec, 1, 9);
    fprintf(out, "vec = ");
    vec_foreach(n, vec) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Insert - Ordered:\n");
    vec_insert_ordered(&vec, 2, 8);
    fprintf(out, "vec = ");
    vec_foreach(n, vec) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Remove:\n");
    vec_remove(&vec, 3);
    fprintf(out, "vec = ");
    vec_foreach(n, vec) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Remove - Ordered:\n");
    vec_remove_ordered(&vec, 4);
    fprintf(out, "vec = ");
    vec_foreach(n, vec) {
        fprintf(out, "%d ", *n);
    }
    fprintf(out, "\n");

    fprintf(out, "Find:\n");
    int f4 = vec_find(vec, 4);
    int f8 = vec_find(vec, 8);
    fprintf(out, "f4 = %d\n", f4);
    fprintf(out, "f8 = %d\n", f8);
    if (f4 != -1) fprintf(out, "vec[f4] = %d\n", vec.items[f4]);
    if (f8 != -1) fprintf(out, "vec[f8] = %d\n", vec.items[f8]);

    fprintf(out, "Clear:\n");
    vec_clear(&vec);
    fprintf(out, "length = %zu\n", vec.length);
    fprintf(out, "capacity = %zu\n", vec.capacity);
    fprintf(out, "items = %s\n", vec.items == NULL ? "null" : "not null");

    fprintf(out, "Free:\n");
    vec_free(&vec);
    fprintf(out, "length = %zu\n", vec.length);
    fprintf(out, "capacity = %zu\n", vec.capacity);
    fprintf(out, "items = %s\n", vec.items == NULL ? "null" : "not null");
}

