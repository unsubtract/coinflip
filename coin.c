/* coinflip.c - flip a coin
 * by unsubtract, MIT License */
#ifdef NOLIBC
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
extern long read(int fd, void* buf, unsigned long count);
extern long write(int fd, void* buf, unsigned long count);
extern long getrandom(void* buf, unsigned long buflen, unsigned int flags);

#else /* NOLIBC */
#include <unistd.h>
#include <sys/random.h>
#endif

#define OBUF_SIZE (1<<4)
#define OBUF_MASK (OBUF_SIZE-1)
typedef struct {
    unsigned p;
    unsigned char b[OBUF_SIZE];
} output_buffer;

static unsigned char randbyte(void);
static unsigned digits(unsigned n, unsigned addend);
static void oappend(const char* buf);
static void oappendi(unsigned n);
static void oflush(void);

static output_buffer o = {0};
static unsigned char randbuf[256], randp = 0;

/* use getrandom() (basically wrapper around /dev/urandom using fewer syscalls)
 * Write into a buffer and only call getrandom() again once the buffer has been
 * fully read, in order to reduce the number of syscalls made.
 * A buffer of 256 bytes is the largest where getrandom(2) may never fail. */
static unsigned char randbyte(void) {
    unsigned char n;
    if (randp == 0) (void)!getrandom(randbuf, 256, 0);
    n = randbuf[randp];
    randp = (randp + 1) & 255;
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

static void oappend(const char* buf) {
    unsigned i = 0;
    while (buf[i] != '\0') {
        o.b[o.p] = buf[i];
        if (o.p > OBUF_MASK || buf[i] == '\n') oflush();
        ++o.p; ++i;
    }
}

static void oappendi(unsigned n) {
    unsigned d = digits(n, 0), i = 1;
    while (d--) i *= 10;
    for (; i > 0; i /= 10) {
        o.b[o.p++] = '0' + ((n / i) % 10);
        if (o.p > OBUF_MASK) oflush();
    }
}

static void oflush(void) {
    unsigned i = 0;
    while (i < o.p) i += write(STDOUT_FILENO, o.b + i, o.p-i);
    o.p = 0;
}


int main(void) {
    unsigned heads = 0, tails = 0;

    for (;;) {
        if (randbyte() & 1) {
            oappend("Heads! ");
            ++heads;
        } else {
            oappend("Tails! ");
            ++tails;
        }

        oappendi((heads * 100 + ((heads + tails) >> 1)) / (heads + tails));
        oappend(" / ");
        oappendi((tails * 100 + ((heads + tails) >> 1)) / (heads + tails));
        oappend("\n");

        /* terrible hack to pause until Enter is pressed */
        do (void)!read(STDIN_FILENO, o.b, 1); while (o.b[0] != '\n');
    }
}
