#include "lambda.hpp"

using namespace lc;

using PrimeListExpected =
    List<
        Nat<2>, Nat<3>, Nat<5>, Nat<7>, Nat<11>,
        Nat<13>, Nat<17>, Nat<19>, Nat<23>, Nat<29>,
        Nat<31>, Nat<37>, Nat<41>, Nat<43>, Nat<47>
    >;

using Inc = Lambda<Apply_t<Succ, Var<0>>>;
using IsEven = Lambda<Apply_t<IsZero, Apply_t<Mod, Var<0>, Two>>>;
using AddFold = Lambda<Lambda<Apply_t<Add, Var<1>, Var<0>>>>;
using ThreeSumExpected = List<List<NegOne, NegOne, Int<2>>, List<NegOne, Int<0>, Int<1>>>;

static_assert(IsSame<Normalize_t<Apply_t<I, A>>, A>::value);
static_assert(IsSame<Normalize_t<Apply_t<K, A, Bv>>, A>::value);
static_assert(IsSame<Normalize_t<Apply_t<KI, A, Bv>>, Bv>::value);
static_assert(IsSame<Normalize_t<Apply_t<S, K, K, A>>, A>::value);

static_assert(IsSame<Shift_t<1, Lambda<Apply_t<Var<1>, Var<0>>>>, Lambda<Apply_t<Var<2>, Var<0>>>>::value);
static_assert(IsSame<Subst_t<0, A, Apply_t<Var<0>, Var<1>>>, Apply_t<A, Var<1>>>::value);
static_assert(IsSame<Compile_t<lam<x, var<x>>>, I>::value);
static_assert(IsSame<Compile_t<app<lam<x, var<x>>, free<a>>>, Apply_t<I, A>>::value);

static_assert(IsSame<Normalize_t<Apply_t<If, True, A, Bv>>, A>::value);
static_assert(IsSame<Normalize_t<Apply_t<If, False, A, Bv>>, Bv>::value);
static_assert(IsSame<Normalize_t<Apply_t<Not, False>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<And, True, False>>, False>::value);
static_assert(IsSame<Normalize_t<Apply_t<Or, False, True>>, True>::value);

static_assert(IsSame<Normalize_t<Apply_t<Succ, Zero>>, One>::value);
static_assert(IsSame<Normalize_t<Apply_t<Pred, Zero>>, Zero>::value);
static_assert(IsSame<Normalize_t<Apply_t<Add, Two, Three>>, Five>::value);
static_assert(IsSame<Normalize_t<Apply_t<Sub, Three, Five>>, Zero>::value);
static_assert(IsSame<Normalize_t<Apply_t<Mul, Three, Four>>, Twelve>::value);
static_assert(IsSame<Normalize_t<Apply_t<Div, Eight, Two>>, Four>::value);
static_assert(IsSame<Normalize_t<Apply_t<Mod, Seven, Four>>, Three>::value);
static_assert(IsSame<Normalize_t<Apply_t<Pow, Three, Two>>, Nine>::value);
static_assert(IsSame<Normalize_t<Apply_t<Eq, Four, Four>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<Lt, Four, Five>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<Lte, Five, Five>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<Gt, Five, Four>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<Gte, Five, Five>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<IsZero, Zero>>, True>::value);

static_assert(IsSame<Normalize_t<Apply_t<Range, Two, Five>>, List<Two, Three, Four, Five>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Cons, One, List<Two, Three>>>, List<One, Two, Three>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Head, List<One, Two, Three>>>, One>::value);
static_assert(IsSame<Normalize_t<Apply_t<Tail, List<One, Two, Three>>>, List<Two, Three>>::value);
static_assert(IsSame<Normalize_t<Apply_t<IsEmpty, Nil>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<Concat, List<One, Two>, List<Three, Four>>>, List<One, Two, Three, Four>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Reverse, List<One, Two, Three>>>, List<Three, Two, One>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Length, List<One, Two, Three>>>, Three>::value);
static_assert(IsSame<Normalize_t<Apply_t<Map, Inc, List<One, Two, Three>>>, List<Two, Three, Four>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Filter, IsEven, List<One, Two, Three, Four>>>, List<Two, Four>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Foldl, AddFold, Zero, List<One, Two, Three>>>, Six>::value);
static_assert(IsSame<Normalize_t<Apply_t<Foldr, AddFold, Zero, List<One, Two, Three>>>, Six>::value);
static_assert(IsSame<Normalize_t<Apply_t<Sum, List<One, Two, Three>>>, Six>::value);
static_assert(IsSame<Normalize_t<Apply_t<Product, List<Two, Three, Four>>>, Nat<24>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Any, List<False, True, False>>>, True>::value);
static_assert(IsSame<Normalize_t<Apply_t<All, List<True, True, False>>>, False>::value);
static_assert(IsSame<Normalize_t<Apply_t<Add, Int<4>, NegOne>>, Int<3>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Sub, Int<4>, NegOne>>, Int<5>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Gt, Int<4>, NegOne>>, True>::value);

static_assert(IsSame<Normalize_t<Apply_t<Factorial, Three>, 4096>, Six>::value);
static_assert(IsSame<Normalize_t<O, 8>, OutOfFuel<O>>::value);
static_assert(IsSame<NormalizeChecked_t<O, 16>, CycleDetected<O>>::value);

static_assert(IsSame<PrimesUpTo50, PrimeListExpected>::value);
static_assert(IsSame<PrimeCountUpTo50, Nat<15>>::value);
static_assert(IsSame<PrimeSumUpTo50, Nat<328>>::value);
static_assert(IsSame<TwoSumExample, List<Nat<0>, Nat<1>>>::value);
static_assert(IsSame<MaxSubarrayExample, Int<6>>::value);
static_assert(IsSame<ThreeSumExample, ThreeSumExpected>::value);
static_assert(to_int_v<PrimeCountUpTo50> == 15);
static_assert(to_int_v<PrimeSumUpTo50> == 328);
static_assert(to_int_v<MaxSubarrayExample> == 6);
static_assert(to_array_v<PrimesUpTo50>.size() == 15);
static_assert(to_array_v<TwoSumExample>[0] == 0);
static_assert(to_array_v<TwoSumExample>[1] == 1);
static_assert(to_matrix_v<ThreeSumExample>[0][0] == -1);
static_assert(to_matrix_v<ThreeSumExample>[1][2] == 1);

int main() {
    return 0;
}
