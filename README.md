# Efficient 4-way Vectorizations of GF(2^256-2^32-977) Arithmetic

Vectorized implementation of arithmetic on Galois field GF(2^256-2^32-977) for use on the Secp256k1 elliptic curve.

---

### Compilation and execution of test program

* Note that these implementations require Intel ADX and AVX2 support.
* These are available with Intel Broadwell, AMD Zen, or newer microarchitectures.

Build test file with:

    $ make -f curve256k1.mak

Run tests with:

    $ ./curve256k1_test

---

### Test configuration

* Configure which tests are run by editing ```config.h```.
* Tests can be configured to use default values or user-input values.
* Each arithmetic operation can be configured individually or all enabled at once.
* CPU-cycle measurements are configured as one block.

* **Default config:** default test values, include all function tests, include speed tests.

---
