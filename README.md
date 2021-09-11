# bitcoin-ewallet-thesis

My bachelor's thesis: "Building an experimental e-wall for bitcoin cryptocurrency".

**Disclaimer:** This project never meant to be used in real-life. This project main goal was to explore the
land of cryptocurrency, specifically bitcoin, and to build an e-wallet for demonstrative purposes. This
project also was done in a 4-month timespan, which is relatively short compare to the 6-month standard time
frame for a bachelor's thesis at my university, so it's still missing a lot of quality-of-life features.

**Note:** The `rest` branch was used to test the REST API library `oatp`. The branch was never merged because
a REST API was not suitable for this kind of project. This is also the reason this feature was never
introduced nor mentioned in the defense and the final submit version of my thesis.

## Repository structure

This repository contains multiple directories:

- `src/` this directory contains the source code.
- `test_inputs/` contains tests used for demonstration and testing purposes.
- `docs/` contains the thesis report.

## Key features

1. Build from scratch.
   
    The project was built using little help from bitcoin development libraries (libbitcoin in this case) to
    demonstrate the level of understanding of the subject to the council.
    Libbitcoin usage is only limited to: containers and operations for locking and unlocking scripts, and
    cryptographic functions like: SHA256, RIPEMD160, ECDSA, ...

2. HD wallet structure.
3. A simpler version of BIP-044 applied (only support 1 account).
4. Automatically generate change output, the user only need to specify the fee per byte for the transaction.

## How to use

### Prerequisites

#### Libraries

This project is tested with libbitcoin-system version 3.6.0 built with
`--with-icu --with-png --with-qrencode --with-gnu-ld` configure options.

This project also need `libcurl` and `jsoncpp`.

#### Platforms

This project was developed and tested with 2 linux distro: Arch and Ubuntu 20.04 LTS.

### Build process

#### Configure cmake build type

```shell
$ cmake -DCMAKE_BUILD_TYPE=<build_type> -B ./build/
```

`build_type` could be `Debug` or `Release` for the corresponding build type (debug or release build).

**Note:** debug build enables the ability to run the program in test mode which is used for testing the
correctness of the code. The debug build is also much slower than release build because there are no
optimizations performed for the debug build type.

#### Compile

After cmake was configured successfully, one can compile the project by easily run

```shell
$ make -C build
```

#### Run

After compilation, the program could be run using the command below

```shell
$ ./build/bitcoin-ewallet-thesis [chain]
```

with `chain` could be `mainnet` or `testnet`. If the `chain` parameter is not given, the program will
default to `mainnet`.

### Run built-in tests

To run built-in tests, the program must be built using the debug build type. After that, one just need to run
the command below to run the built-in tests

```shell
$ ./build/bitcoin-ewallet-thesis test
```

### How to use

Run the `help` command in the command shell for instructions. 
