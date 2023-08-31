/* coinflip.c - flip a coin
 * by unsubtract, MIT License */
#ifdef NOLIBC
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
extern void* syscall3(void* n, void* a, void* b, void* c);

/* syscall function definitions + ugly X macro spaghetti because
 * I thought it would be cooler than writing out identical functions by hand.
 * https://en.wikipedia.org/wiki/X_macro */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#define X(name, n, ta, a, tb, b, tc, c) \
    static inline long name(ta a, tb b, tc c) {\
        return (long)syscall3((void*)n, (void*)a, (void*)b, (void*)c);\
    }
X(read, 0, int, fd, void*, buf, long, count)
X(write, 1, int, fd, void*, buf, long, count)
X(getrandom, 318, void*, buf, long, buflen, unsigned, flags)
#undef X
#pragma GCC diagnostic pop

#else /* NOLIBC */
#include <unistd.h>
#include <sys/random.h>
#endif

#define OBUF_SIZE (1<<4)
#define OBUF_MASK (OBUF_SIZE-1)
typedef struct {
    unsigned char p;
    unsigned char b[OBUF_SIZE];
} output_buffer;

static unsigned char randbyte(void);
static unsigned digits(unsigned n, unsigned addend);
static void oappend(output_buffer* o, const char* buf, unsigned len);
static void oappendi(output_buffer* o, unsigned n);
static void oflush(output_buffer* o);

static output_buffer o = {.p = 0};

/* use getrandom() (basically wrapper around /dev/urandom using fewer syscalls)
 * Write into a buffer and only call getrandom() again once the buffer has been
 * fully read, in order to reduce the number of syscalls made.
 * A buffer of 256 bytes is the largest where getrandom(2) may never fail. */
static unsigned char randbyte(void) {
    static unsigned char randbuffer[256], p = 0;
    unsigned char n;
    if (p == 0) (void)getrandom(randbuffer, 256, 0);
    n = randbuffer[p];
    p = (p + 1) & 255;
    return n;
}

/* uses simple tail recursion
 * The addend (which should normally start as 1) would be abstracted away if
 * it weren't for the fact that manipulating it is useful in the
 * implementation of oappendi() (it will add or subtract from the result) */
static unsigned digits(unsigned n, unsigned addend) {
    if (n < 10) return addend;
    return digits(n / 10, addend + 1);
}

static void oappend(output_buffer* o, const char* buf, unsigned len) {
    unsigned i = 0;
    while (i < len) {
        o->b[o->p++] = buf[i++];
        if (o->p > OBUF_MASK || buf[i-1] == '\n') oflush(o);
    }
}

static void oappendi(output_buffer* o, unsigned n) {
    unsigned d = digits(n, 0), i = 1;
    while (d--) i *= 10;
    for (; i > 0; i /= 10) {
        o->b[o->p++] = '0' + ((n / i) % 10);
        if (o->p > OBUF_MASK) oflush(o);
    }
}

static void oflush(output_buffer* o) {
    long i = 0;
    while (i < o->p) i += write(STDOUT_FILENO, o->b + i, o->p-i);
    o->p = 0;
}


int main(void) {
    unsigned heads = 0, tails = 0;

    for (;;) {
        if (randbyte() & 1) {
            oappend(&o, "Heads! ", 7);
            ++heads;
        } else {
            oappend(&o, "Tails! ", 7);
            ++tails;
        }

        oappendi(&o, (heads * 100 + ((heads + tails) >> 1)) / (heads + tails));
        oappend(&o, " / ", 3);
        oappendi(&o, (tails * 100 + ((heads + tails) >> 1)) / (heads + tails));
        oappend(&o, "\n", 1);

        /* terrible hack to pause until Enter is pressed */
        do (void)read(STDIN_FILENO, o.b, 1); while (o.b[0] != '\n');
    }
}
