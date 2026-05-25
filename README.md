# Lambda Calculus Metaprogramming in C++

This repository is a header-only C++20 library for doing computation through type-level lambda calculus machinery.

At its core, it has:

- a De Bruijn-indexed lambda AST
- capture-avoiding shift and substitution
- a normal-order reducer with bounded fuel
- an opt-in checked normalizer for exact cycle detection
- a hybrid standard library with intrinsic naturals, signed integers, booleans, and lists for practical compile-time performance

The result is a small compile-time language runtime built with templates. It still reduces programs by type substitution and application, but it avoids the worst Church-encoding blowups for arithmetic and collection-heavy code.

## Why This Exists

Pure lambda-calculus encodings are elegant, but they get expensive very quickly in C++ template metaprogramming. This project takes a pragmatic approach:

- lambda terms, substitution, and beta reduction remain the computational foundation
- performance-critical values use compact intrinsic forms like `Nat<5>`, `Int<-3>`, `Bool<true>`, and `List<...>`
- primitive operations such as `Add`, `Map`, `Filter`, and `Range` reduce directly when their arguments are intrinsic values

That gives you something that still feels like a tiny lambda-calculus computer, but can also run more interesting compile-time programs.

## Features

- Header-only library
- Variadic application spine via `Call<...>` to avoid deeply nested binary application trees
- Fast default normalization with `Normalize_t<T, Fuel>`
- Exact repeated-term cycle detection with `NormalizeChecked_t<T, Fuel>`
- Named surface syntax compiler with `var`, `lam`, `app`, and `free`
- Standard library for:
  - combinators: `I`, `K`, `KI`, `S`, `B`, `C`, `W`, `Y`
  - booleans: `True`, `False`, `If`, `Not`, `And`, `Or`
  - numbers: `Nat<N>`, `Int<N>`, `Zero` through `Twelve`, `NegOne` through `NegFive`, `Succ`, `Pred`, `Add`, `Sub`, `Mul`, `Div`, `Mod`, `Pow`, `Eq`, `Lt`, `Lte`, `Gt`, `Gte`, `IsZero`
  - lists: `Nil`, `Cons`, `Head`, `Tail`, `IsEmpty`, `Concat`, `Reverse`, `Length`, `Range`, `Map`, `Filter`, `Foldl`, `Foldr`, `Sum`, `Product`, `Any`, `All`
- Example algorithms: `Sieve`, `TwoSum`, `MaxSubarraySum`, `ThreeSum`
- Legacy pure encodings preserved under `lc::church`
- Runtime bridge helpers for turning compile-time results into ordinary values

## Repo Layout

```text
lambda.hpp                Umbrella public header
include/lc/core.hpp       Core AST, utilities, named syntax, public aliases
include/lc/intrinsics.hpp Intrinsic values and primitive reductions
include/lc/eval.hpp       Shift, substitution, stepping, normalization
include/lc/std.hpp        Standard library and example programs
include/lc/runtime.hpp    Bridge from type-level results to runtime values
main.cpp                  Demo program
tests.cpp                 Static-assert based test suite
Makefile                  Clang-focused build targets
```

## Build And Run

This project is currently tuned for `clang++` and expects a reasonably modern Clang. The `Makefile` uses aggressive optimization flags for compile-time-heavy code:

```bash
make demo
./bin/demo
```

```bash
make test
```

```bash
make trace
```

`trace` rebuilds with `-ftime-trace`, which is useful when you want to inspect where template-instantiation time is going.

## Demo

The demo computes a sieve of Eratosthenes at compile time over the range `2..50`, then prints the result at runtime:

```text
primes up to 50: [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
count: 15
sum: 328
two-sum indices for [2, 7, 11, 15], target 9: [0, 1]
maximum subarray sum for [-2, 1, -3, 4, -1, 2, 1, -5, 4]: 6
three-sum triplets for [-4, -1, -1, 0, 1, 2]: [[-1, -1, 2], [-1, 0, 1]]
```

The relevant aliases live in [include/lc/std.hpp](/home/ethan/dev/fun/metaprogramming/include/lc/std.hpp):

```cpp
template<int Limit>
using PrimesUpTo = Normalize_t<Apply_t<Sieve, Apply_t<Range, Two, Nat<Limit>>>, 1024>;

using PrimesUpTo50 = PrimesUpTo<50>;
using PrimeCountUpTo50 = Normalize_t<Apply_t<Length, PrimesUpTo50>>;
using PrimeSumUpTo50 = Normalize_t<Apply_t<Sum, PrimesUpTo50>>;
```

## Quick Examples

### 1. Reducing a lambda term

```cpp
#include "lambda.hpp"

using namespace lc;

static_assert(IsSame<Normalize_t<Apply_t<I, A>>, A>::value);
static_assert(IsSame<Normalize_t<Apply_t<S, K, K, A>>, A>::value);
```

### 2. Arithmetic on intrinsic naturals

```cpp
#include "lambda.hpp"

using namespace lc;

static_assert(IsSame<Normalize_t<Apply_t<Add, Two, Three>>, Five>::value);
static_assert(IsSame<Normalize_t<Apply_t<Mul, Three, Four>>, Twelve>::value);
static_assert(IsSame<Normalize_t<Apply_t<Pow, Three, Two>>, Nine>::value);
```

### 3. Functional list processing

```cpp
#include "lambda.hpp"

using namespace lc;

using Inc = Lambda<Apply_t<Succ, Var<0>>>;
using IsEven = Lambda<Apply_t<IsZero, Apply_t<Mod, Var<0>, Two>>>;

static_assert(IsSame<
    Normalize_t<Apply_t<Map, Inc, List<One, Two, Three>>>,
    List<Two, Three, Four>
>::value);

static_assert(IsSame<
    Normalize_t<Apply_t<Filter, IsEven, List<One, Two, Three, Four>>>,
    List<Two, Four>
>::value);
```

### 4. Named surface syntax

```cpp
#include "lambda.hpp"

using namespace lc;

static_assert(IsSame<Compile_t<lam<x, var<x>>>, I>::value);
static_assert(IsSame<Run_t<app<lam<x, var<x>>, free<a>>>, A>::value);
```

### 5. Classic interview problems

```cpp
#include "lambda.hpp"

using namespace lc;

static_assert(IsSame<
    Normalize_t<Apply_t<TwoSum, List<Two, Seven, Eleven, Nat<15>>, Nine>>,
    List<Nat<0>, Nat<1>>
>::value);

static_assert(IsSame<
    Normalize_t<Apply_t<MaxSubarraySum, List<NegTwo, Int<1>, NegThree, Int<4>, NegOne, Int<2>, Int<1>, NegFive, Int<4>>>>,
    Int<6>
>::value);

static_assert(IsSame<
    Normalize_t<Apply_t<ThreeSum, List<NegFour, NegOne, NegOne, Int<0>, Int<1>, Int<2>>>>,
    List<List<NegOne, NegOne, Int<2>>, List<NegOne, Int<0>, Int<1>>>
>::value);
```

## Core Mental Model

There are three layers to keep in mind:

### 1. Core lambda terms

The evaluator understands:

- `Var<N>`
- `Lambda<Body>`
- `App<F, A>`
- `Call<Head, Args...>`
- `Free<Name>`

The reducer canonicalizes application into `Call<...>`, then performs normal-order reduction. If the head of a call is a lambda, it beta-reduces through substitution. If the head is an intrinsic primitive like `Add` or `Map`, it can collapse directly when the arguments are intrinsic values.

### 2. Intrinsic values

These are compact data forms that make the system practical:

- `Nat<N>`
- `Int<N>`
- `Bool<B>`
- `List<Ts...>`

They are still manipulated by the evaluator, but they avoid the AST explosion of large Church-encoded data.

### 3. Surface syntax

For readability, named expressions can be written as:

- `var<x>`
- `lam<x, Body>`
- `app<F, A>`
- `free<a>`

They compile into the De Bruijn core with `Compile_t`.

## Normalization Modes

### Fast default

```cpp
using Result = Normalize_t<MyTerm, 1024>;
```

This is fuel-bounded and optimized for getting useful work done.

If reduction does not finish before fuel runs out, the result is:

```cpp
OutOfFuel<Term>
```

### Checked normalization

```cpp
using Result = NormalizeChecked_t<MyTerm, 1024>;
```

This mode tracks previously seen terms and can detect exact syntactic cycles:

```cpp
static_assert(IsSame<NormalizeChecked_t<O, 16>, CycleDetected<O>>::value);
```

Use this when debugging nontermination or verifying pathological terms. It is intentionally more expensive.

## Runtime Bridge Helpers

The library includes a small bridge for moving useful compile-time results into normal runtime code:

```cpp
to_int_v<Nat<42>>
to_int_v<Int<-7>>
to_bool_v<Bool<true>>
to_array_v<List<Nat<1>, Nat<2>, Nat<3>>>
to_matrix_v<List<List<Int<-1>, Int<0>, Int<1>>>>
```

That bridge is what powers the demo output in [main.cpp](/home/ethan/dev/fun/metaprogramming/main.cpp).

## Legacy Church Encodings

The original pure lambda-calculus flavor is still present under `lc::church`.

Examples:

- `lc::church::True`
- `lc::church::False`
- `lc::church::Zero`
- `lc::church::Succ`
- `lc::church::Add`
- `lc::church::Mul`
- `lc::church::ListNil`
- `lc::church::ListCons`

These are useful if you want to experiment with the pure encodings or compare behavior against the hybrid model.

## Testing

The test suite in [tests.cpp](/home/ethan/dev/fun/metaprogramming/tests.cpp) uses `static_assert` heavily and covers:

- classic combinator reductions
- shift and substitution behavior
- surface-syntax compilation
- intrinsic boolean and arithmetic operations
- list transformations and folds
- recursive programs such as factorial
- fuel exhaustion vs checked cycle detection
- compile-time sieve results

Because the project is mostly type-level computation, successful compilation is the primary proof that the library works.

## Notes And Tradeoffs

- This is intentionally Clang-first right now.
- The library is header-only because the evaluator is template-based.
- The project is not trying to be a pure theorem of lambda calculus at all costs; it is trying to be an enjoyable and capable compile-time metaprogramming playground.
- Intrinsic data and primitive reducers are the main pragmatic compromise.

## Where To Start

If you are new to the codebase, a good reading order is:

1. [include/lc/core.hpp](/home/ethan/dev/fun/metaprogramming/include/lc/core.hpp)
2. [include/lc/eval.hpp](/home/ethan/dev/fun/metaprogramming/include/lc/eval.hpp)
3. [include/lc/intrinsics.hpp](/home/ethan/dev/fun/metaprogramming/include/lc/intrinsics.hpp)
4. [include/lc/std.hpp](/home/ethan/dev/fun/metaprogramming/include/lc/std.hpp)
5. [tests.cpp](/home/ethan/dev/fun/metaprogramming/tests.cpp)

That path gives you the core model first, then the practical extensions, then concrete examples.
