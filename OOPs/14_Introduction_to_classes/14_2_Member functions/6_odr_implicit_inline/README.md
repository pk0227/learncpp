# 6 — In-class member functions are implicitly `inline` (ODR)

This is a **multi-file** example because the point can only be proven by linking
**two translation units** that both include the same class definition.

## Files
- `counter.h` — defines `class Counter` with member functions defined **inside** the class.
- `file_a.cpp` — includes `counter.h` and uses `Counter`.
- `main.cpp` — also includes `counter.h` and uses `Counter`.

## Why it matters
Both `.cpp` files include `counter.h`, so the definitions of
`Counter::increment()` and `Counter::get()` textually appear in **both**
translation units. Normally, defining the same function in two TUs violates the
**One Definition Rule (ODR)** and the linker rejects it with a
*"multiple definition"* error.

It links cleanly here because **member functions defined inside the class
definition are implicitly `inline`**, which exempts them from the "one
definition per program" part of the ODR — the linker de-duplicates the copies.

## Build & run
```bash
g++ -std=c++20 -Wall -Wextra main.cpp file_a.cpp -o odr_demo
./odr_demo
```

## Try the contrast (optional)
In `counter.h`, remove the `inline` keyword from the non-member `label()`
function and rebuild. Now `label()` is defined in two TUs **without** being
inline, so the link **fails** with a multiple-definition error — exactly what
`inline` (and the implicit inline on in-class members) prevents.
