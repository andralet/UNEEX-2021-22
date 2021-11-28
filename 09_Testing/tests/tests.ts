#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <check.h>

/*
static jmp_buf escape;

static void
test_abort(void)
{
    longjmp(escape, 1);
}

#define BUF_ABORT test_abort()
*/
#include "../src/buf.h"

/*
static uint32_t
pcg32(uint64_t *s)
{
    uint64_t m = 0x5851f42d4c957f2d;
    uint64_t a = 0x8b260b70b8e98891;
    uint64_t p = *s;
    uint32_t x = ((p >> 18) ^ p) >> 27;
    uint32_t r = p >> 59;
    *s = p * m + a;
    return (x >> r) | (x << (-r & 31u));
}

// If you want to run it, don't forget to increase timeout: it takes huge time to run
static unsigned long
bench(uint64_t *rng)
{
    unsigned long r = 0;
    uint32_t n = 1000000 + pcg32(rng) % 4000000;
    float *buf = 0;

    for (uint32_t i = 0; i < n; i++)
        buf_push(buf, pcg32(rng) / (double)UINT32_MAX);

    float threshold = pcg32(rng) / (double)UINT32_MAX;
    for (uint32_t i = 0; i < n; i++)
        r += buf[i] > threshold;

    buf_free(buf);
    return r;
}

#ifdef _WIN32
#include <windows.h>
uint64_t
uepoch(void)
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t tt = ft.dwHighDateTime;
    tt <<= 32;
    tt |= ft.dwLowDateTime;
    tt /=10;
    tt -= UINT64_C(11644473600000000);
    return tt;
}
#else
#include <sys/time.h>
uint64_t
uepoch(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return 1000000LL * tv.tv_sec + tv.tv_usec;
}
#endif
*/

/* My near_equal function */
#define EQ_EPS 0.0001
_Bool near_equal(float a, float b) {
	return (a > b - EQ_EPS) && (a < b + EQ_EPS);
}

/* initialization, buf_free() */
#tcase Basic_Tests
	#test capacity_and_size_initializing
		float *a = 0;
		ck_assert_int_eq(buf_capacity(a), 0);
		ck_assert_int_eq(buf_size(a), 0);
	#test pushing
	    float *a = 0;
    	buf_push(a, 1.3f);
		ck_assert_int_eq(buf_size(a), 1);
		ck_assert(near_equal(a[0], (float)1.3f));
	#test clearing
		float *a = 0;
    	buf_push(a, 1.3f);
	    buf_clear(a);
		ck_assert_int_eq(buf_size(a), 0);
		ck_assert_ptr_ne(a, (float *)0);
	#test freeing
		float *a = 0;
	    buf_push(a, 1.3f);
	    buf_clear(a);
	    buf_free(a);
		ck_assert_ptr_eq(a, (float *)0);
	#test clearing_null
	    /* Clearing an NULL pointer is a no-op */
		float *a = 0;
	    buf_clear(a);
		ck_assert_int_eq(buf_size(a), 0);
		ck_assert_ptr_eq(a, (float *)0);

#tcase Access_and_Resizing
	#test push_and_array_access
	    long *ai = 0;
    	for (int i = 0; i < 10000; i++)
        	buf_push(ai, i);
		ck_assert_int_eq(buf_size(ai), 10000);
	    int match = 0;
    	for (int i = 0; i < (int)(buf_size(ai)); i++)
        	match += ai[i] == i;
		ck_assert_int_eq(match, 10000);
	    buf_free(ai);
	#test grow_and_trunc
	    long *a = 0;
	    buf_grow(a, 1000);
		ck_assert_int_eq(buf_capacity(a), 1000);
		ck_assert_int_eq(buf_size(a), 0);
	    buf_trunc(a, 100);
		ck_assert_int_eq(buf_capacity(a), 100);
	    buf_free(a);
	#test popping
		float *a = 0;
	    buf_push(a, 1.1);
	    buf_push(a, 1.2);
    	buf_push(a, 1.3);
	    buf_push(a, 1.4);
		ck_assert_int_eq(buf_size(a), 4);
		ck_assert(near_equal(buf_pop(a), (float)1.4f));
    	buf_trunc(a, 3);
		ck_assert_int_eq(buf_size(a), 3);
		ck_assert(near_equal(buf_pop(a), (float)1.3f));
		ck_assert(near_equal(buf_pop(a), (float)1.2f));
		ck_assert(near_equal(buf_pop(a), (float)1.1f));
		ck_assert_int_eq(buf_size(a), 0);
	    buf_free(a);

/*
// TODO: figure out what's wrong with setjmp and why we get SIGABRT
tcase Memory_Allocation_Failures
	test out_of_memory
		ck_assert(!setjmp(escape));
	    volatile int aborted = 0;
		int *volatile p = 0;
	    if (!setjmp(escape)) {
            size_t max = (PTRDIFF_MAX - sizeof(struct buf)) / sizeof(*p) + 1;
            buf_grow(p, max);
            buf_grow(p, max);
        } else {
            aborted = 1;
        }
        buf_free(p);
		ck_assert(aborted);
	test overflow_init
		ck_assert(!setjmp(escape));
	    volatile int aborted = 0;
	    int *volatile p = 0;
    	if (!setjmp(escape)) {
        	buf_trunc(p, PTRDIFF_MAX);
        } else {
            aborted = 1;
        }
        buf_free(p);
		ck_assert(aborted);
	test overflow_grow
		ck_assert(!setjmp(escape));
	    volatile int aborted = 0;
        int *volatile p = 0;
        if (!setjmp(escape)) {
            buf_trunc(p, 1); // force realloc() use next
            buf_trunc(p, PTRDIFF_MAX);
        } else {
            aborted = 1;
        }
        buf_free(p);
		ck_assert(aborted);
*/

/*
tcase Benchtest
	test rng_1
        uint64_t rng = 1; // strtoull(argv[1], 0, 16);
        unsigned long r = 0;
        uint64_t start = uepoch();
        for (int i = 0; i < 300; i++)
            r += bench(&rng);
        //double t = (uepoch() - start) / 1e6;
        //printf("%.6gs : acutal %lu, expect 428859598\n", t, r);
		ck_assert_int_eq(r, 428859598); // no idea, how to get this magic number
*/

int
main(int argc, char **argv)
{
    return 0;
}
