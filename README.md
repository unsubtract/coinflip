# coinflip

> elincyt<br>
>  — <br>
> 08/24/2023 12:59 PM<br>
> make C program that flip coin<br>

A friend of mine who is learning Python made a program that will flip an imaginary coin,
printing the result but also the ratio between the number of times each side has landed (expressed as a percentage):
```py
import random
heads = tails = 0
while True:
    input()
    if random.randrange(1,3) == 1: 
        print("Heads!")
        heads += 1
    else: 
        print("Tails!")
        tails += 1
    print(int(heads/(heads + tails)*100), "/", int(tails/(heads + tails)*100))
```

---

### [Naturally, my immediate response was to rewrite it in C... without the standard library.](./coin.c)

I had been experimenting with libc-less programs for
a [while now](https://github.com/unsubtract/Programming-Language-nolibc), and used this as
an opportunity to end up writing a simple buffered, formatted output library using only POSIX syscalls.

Building without libc (try `NOLIBC=1 ./build.sh`, only Linux x86_64 supported)
creates an impressively small binary (1.04 KiB on my system), far smaller than 
the source code itself (as it should be).
