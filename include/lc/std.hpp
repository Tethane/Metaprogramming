#pragma once

#include "eval.hpp"

namespace lc {

using True = Bool<true>;
using False = Bool<false>;

using Zero = Nat<0>;
using One = Nat<1>;
using Two = Nat<2>;
using Three = Nat<3>;
using Four = Nat<4>;
using Five = Nat<5>;
using Six = Nat<6>;
using Seven = Nat<7>;
using Eight = Nat<8>;
using Nine = Nat<9>;
using Ten = Nat<10>;
using Eleven = Nat<11>;
using Twelve = Nat<12>;

using Nil = List<>;

using I = Lambda<Var<0>>;
using K = Lambda<Lambda<Var<1>>>;
using KI = Lambda<Lambda<Var<0>>>;
using S = Lambda<Lambda<Lambda<Apply_t<Var<2>, Var<0>, Apply_t<Var<1>, Var<0>>>>>>;
using B = Lambda<Lambda<Lambda<Apply_t<Var<2>, Apply_t<Var<1>, Var<0>>>>>>;
using C = Lambda<Lambda<Lambda<Apply_t<Var<2>, Var<0>, Var<1>>>>>;
using W = Lambda<Lambda<Apply_t<Var<1>, Var<0>, Var<0>>>>;
using Y =
    Lambda<
        Apply_t<
            Lambda<Apply_t<Var<1>, Apply_t<Var<0>, Var<0>>>>,
            Lambda<Apply_t<Var<1>, Apply_t<Var<0>, Var<0>>>>
        >
    >;
using O = Apply_t<Lambda<Apply_t<Var<0>, Var<0>>>, Lambda<Apply_t<Var<0>, Var<0>>>>;

using FactF =
    Lambda<
        Lambda<
            Apply_t<
                If,
                Apply_t<IsZero, Var<0>>,
                One,
                Apply_t<Mul, Var<0>, Apply_t<Var<1>, Apply_t<Pred, Var<0>>>>
            >
        >
    >;

using Factorial = Apply_t<Y, FactF>;

template<int Limit>
using PrimesUpTo = Normalize_t<Apply_t<Sieve, Apply_t<Range, Two, Nat<Limit>>>, 1024>;

using PrimesUpTo50 = PrimesUpTo<50>;
using PrimeCountUpTo50 = Normalize_t<Apply_t<Length, PrimesUpTo50>>;
using PrimeSumUpTo50 = Normalize_t<Apply_t<Sum, PrimesUpTo50>>;

namespace church {

using True = Lambda<Lambda<Var<1>>>;
using False = Lambda<Lambda<Var<0>>>;
using If = Lambda<Lambda<Lambda<Apply_t<Var<2>, Var<1>, Var<0>>>>>;

using Zero = Lambda<Lambda<Var<0>>>;
using One = Lambda<Lambda<Apply_t<Var<1>, Var<0>>>>;
using Two = Lambda<Lambda<Apply_t<Var<1>, Var<1>, Var<0>>>>;
using Three = Lambda<Lambda<Apply_t<Var<1>, Var<1>, Var<1>, Var<0>>>>;

using Succ =
    Lambda<
        Lambda<
            Lambda<
                Apply_t<Var<1>, Apply_t<Var<2>, Var<1>, Var<0>>>
            >
        >
    >;

using Add =
    Lambda<
        Lambda<
            Lambda<
                Lambda<
                    Apply_t<Var<3>, Var<1>, Apply_t<Var<2>, Var<1>, Var<0>>>
                >
            >
        >
    >;

using Mul =
    Lambda<
        Lambda<
            Lambda<
                Apply_t<Var<2>, Apply_t<Var<1>, Var<0>>>
            >
        >
    >;

using ListNil = Lambda<Lambda<Var<0>>>;
using ListCons =
    Lambda<
        Lambda<
            Lambda<
                Lambda<
                    Apply_t<Var<1>, Var<3>, Apply_t<Var<2>, Var<1>, Var<0>>>
                >
            >
        >
    >;

} // namespace church

struct a;
struct b;
struct c;
struct x;
struct y;
struct z;

using A = Free<a>;
using Bv = Free<b>;
using Cc = Free<c>;

} // namespace lc
