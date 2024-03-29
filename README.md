# coinflip

> elincyt<br>
>  — <br>
> 08/24/2023 12:59 PM<br>
> make C program that flip coin<br>

A friend of mine who is learning Python made a program that flips an imaginary coin,
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

### My immediate response was to rewrite it in C...<br>without the standard library

I've been experimenting with libc-less programs for
[some time now](https://github.com/unsubtract/Programming-Language-nolibc),
and used this as an opportunity to create a simple, buffered
and formatted output library using only Linux/BSD syscalls.

Building without libc (try `NOLIBC=1 ./build.sh`, only Linux x86_64 supported)
creates an impressively small binary (0.98 KiB on my system),
far smaller than the source code itself.
