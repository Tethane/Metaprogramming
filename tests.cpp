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
using LambdaCalculusExpected = String<'l', 'a', 'm', 'b', 'd', 'a', ' ', 'c', 'a', 'l', 'c', 'u', 'l', 'u', 's'>;
using CalculusOnlyExpected = String<'c', 'a', 'l', 'c', 'u', 'l', 'u', 's'>;
using PrettyLambdaExpected = String<'(', 'l', 'a', 'm', 'b', 'd', 'a', ' ', '(', '(', 'x', ' ', 'I', 'n', 't', ')', ')', ' ', 'x', ')'>;
using PrettyCoreIdentityExpected = String<'(', 'l', 'a', 'm', 'b', 'd', 'a', ' ', 'v', '0', ')'>;

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
static_assert(IsSame<LambdaCalculusString, LambdaCalculusExpected>::value);
static_assert(IsSame<LambdaCalculusLength, Nat<15>>::value);
static_assert(IsSame<Normalize_t<Apply_t<Length, LambdaCalculusString>>, Nat<15>>::value);
static_assert(IsSame<LambdaPrefix, LambdaString>::value);
static_assert(IsSame<CalculusSuffix, CalculusOnlyExpected>::value);
static_assert(IsSame<Normalize_t<Apply_t<StringEq, LambdaPrefix, LambdaString>>, True>::value);
static_assert(IsSame<StringContainsCalc, True>::value);
static_assert(IsSame<StringStartsWithLambda, True>::value);
static_assert(IsSame<UnionSetExample, Set<One, Three, Five, Four>>::value);
static_assert(IsSame<IntersectionSetExample, Set<Three, Five>>::value);
static_assert(IsSame<SetContainsFourExample, True>::value);
static_assert(IsSame<SetSizeExample, Four>::value);
static_assert(IsSame<Normalize_t<Apply_t<Length, UnionSetExample>>, Four>::value);
static_assert(IsSame<MapLookupAnswer, Int<42>>::value);
static_assert(IsSame<MapContainsLucky, True>::value);
static_assert(IsSame<MapSizeExample, Two>::value);
static_assert(IsSame<Normalize_t<Apply_t<MapFind, Normalize_t<Apply_t<MapErase, ExampleMap, KeyLucky>>, KeyLucky>>, None>::value);
static_assert(IsSame<Normalize_t<Apply_t<MapContainsKey, ExampleMap, String<'m', 'i', 's', 's', 'i', 'n', 'g'>>>, False>::value);
static_assert(IsSame<LispClosureResult, Int<22>>::value);
static_assert(IsSame<LispClosureResultType, IntType>::value);
static_assert(IsSame<LispStringResult, String<'l', 'a', 'm', 'b', 'd', 'a', ' ', 'r', 'u', 'n', 't', 'i', 'm', 'e'>>::value);
static_assert(IsSame<LispBadProgramType, TypeError<msg_call_type_error>>::value);
static_assert(IsSame<ReaderClosureProgram, LispClosureProgram>::value);
static_assert(IsSame<ReaderClosureResult, Int<22>>::value);
static_assert(IsSame<ReaderClosureType, IntType>::value);
static_assert(IsSame<ReaderStringResult, String<'l', 'a', 'm', 'b', 'd', 'a', ' ', 'r', 'u', 'n', 't', 'i', 'm', 'e'>>::value);
static_assert(IsSame<ReaderQuotedListResult, List<Int<1>, Int<2>, Int<3>>>::value);
static_assert(IsSame<ReaderUntypedIdentityResult, Int<42>>::value);
static_assert(IsSame<ReaderUntypedIdentityType, AnyType>::value);
static_assert(IsSame<ReaderCommentResult, String<'l', 'a', 'm', 'b', 'd', 'a', ' ', 'r', 'e', 'a', 'd', 'e', 'r'>>::value);
static_assert(IsSame<ReaderEscapedStringResult, String<'l', 'a', 'm', 'b', 'd', 'a', '\n', 'r', 'e', 'a', 'd', 'e', 'r'>>::value);
static_assert(IsSame<ReaderBoolProgram, Int<1>>::value);
static_assert(IsSame<Pretty_t<LispIdentity>, PrettyLambdaExpected>::value);
static_assert(IsSame<Pretty_t<I>, PrettyCoreIdentityExpected>::value);
static_assert(IsSame<TypeCheck_t<IfExpr<True, Int<1>, Int<2>>>, IntType>::value);
static_assert(IsSame<EvalLisp_t<IfExpr<True, Int<1>, Int<2>>>, Int<1>>::value);

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
static_assert(to_string_view_v<LambdaCalculusString>.size() == 15);
static_assert(to_string_view_v<LambdaPrefix> == "lambda");
static_assert(to_string_view_v<CalculusSuffix> == "calculus");
static_assert(pretty_string_view_v<LispIdentity> == "(lambda ((x Int)) x)");
static_assert(pretty_string_view_v<LispClosureProgram>.size() > 10);
static_assert(pretty_string_view_v<TypeCheck_t<LispIdentity>> == "(-> ((x Int)) Int)");
static_assert(pretty_string_view_v<ReaderClosureProgram> == "(let ((x 10) (make (lambda ((y Int)) (lambda ((z Int)) (+ (+ x y) z))))) ((make 5) 7))");
static_assert(pretty_string_view_v<ReadSource_t<"(+ 1 2">> == "#<reader-error unterminated-list>");
static_assert(to_array_v<UnionSetExample>[3] == 4);
static_assert(to_array_v<TwoSumExample>[0] == 0);
static_assert(to_array_v<TwoSumExample>[1] == 1);
static_assert(to_matrix_v<ThreeSumExample>[0][0] == -1);
static_assert(to_matrix_v<ThreeSumExample>[1][2] == 1);

int main() {
    return 0;
}
