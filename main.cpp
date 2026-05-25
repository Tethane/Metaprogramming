#include "lambda.hpp"

using namespace lc;

int main() {
    static_assert(IsSame<Normalize_t<App<I, A>>, A>::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<K, A, B>>,
        A
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<KI, A, B>>,
        B
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<S, K, K, A>>,
        A
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<True, A, B>>,
        A
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<False, A, B>>,
        B
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<If, True, A, B>>,
        A
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<If, False, A, B>>,
        B
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<Succ, Zero>>,
        One
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<Add, One, Two>>,
        Three
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<Mul, Two, Two>>,
        Four
    >::value);

    static_assert(IsSame<
        Normalize_t<O, 16>,
        CycleDetected<O>
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<Factorial, Zero>, 1024>,
        One
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<Factorial, Two>, 4096>,
        Two
    >::value);

    static_assert(IsSame<
        Normalize_t<Apply_t<Foldr, Add, Zero, ListOneTwoThree>, 20000>,
        Six
    >::value);

    return 0;
}