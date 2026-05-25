#pragma once

#include "reader.hpp"

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

using NegOne = Int<-1>;
using NegTwo = Int<-2>;
using NegThree = Int<-3>;
using NegFour = Int<-4>;
using NegFive = Int<-5>;

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

using TwoSumInput = List<Two, Seven, Eleven, Nat<15>>;
using TwoSumTarget = Nine;
using TwoSumExample = Normalize_t<Apply_t<TwoSum, TwoSumInput, TwoSumTarget>>;

using MaxSubarrayInput = List<NegTwo, Int<1>, NegThree, Int<4>, NegOne, Int<2>, Int<1>, NegFive, Int<4>>;
using MaxSubarrayExample = Normalize_t<Apply_t<MaxSubarraySum, MaxSubarrayInput>>;

using ThreeSumInput = List<NegFour, NegOne, NegOne, Int<0>, Int<1>, Int<2>>;
using ThreeSumExample = Normalize_t<Apply_t<ThreeSum, ThreeSumInput>>;

using LambdaString = String<'l', 'a', 'm', 'b', 'd', 'a'>;
using CalculusString = String<' ', 'c', 'a', 'l', 'c', 'u', 'l', 'u', 's'>;
using LambdaCalculusString = Normalize_t<Apply_t<StringConcat, LambdaString, CalculusString>>;
using LambdaCalculusLength = Normalize_t<Apply_t<StringLength, LambdaCalculusString>>;
using LambdaPrefix = Normalize_t<Apply_t<StringTake, Six, LambdaCalculusString>>;
using CalculusSuffix = Normalize_t<Apply_t<StringDrop, Seven, LambdaCalculusString>>;
using StringContainsCalc = Normalize_t<Apply_t<StringContains, LambdaCalculusString, String<'c', 'a', 'l', 'c'>>>;
using StringStartsWithLambda = Normalize_t<Apply_t<StringStartsWith, LambdaCalculusString, LambdaString>>;

using OddSet = Set<One, Three, Five>;
using MixedSet = Set<Three, Four, Five>;
using UnionSetExample = Normalize_t<Apply_t<SetUnion, OddSet, MixedSet>>;
using IntersectionSetExample = Normalize_t<Apply_t<SetIntersection, OddSet, MixedSet>>;
using SetContainsFourExample = Normalize_t<Apply_t<SetContains, UnionSetExample, Four>>;
using SetSizeExample = Normalize_t<Apply_t<SetSize, UnionSetExample>>;

using KeyAnswer = String<'a', 'n', 's', 'w', 'e', 'r'>;
using KeyLucky = String<'l', 'u', 'c', 'k', 'y'>;
using ExampleMapBase = Normalize_t<Apply_t<MapInsert, AssocMap<>, KeyAnswer, Int<42>>>;
using ExampleMap = Normalize_t<Apply_t<MapInsert, ExampleMapBase, KeyLucky, Int<7>>>;
using MapLookupAnswer = Normalize_t<Apply_t<MapFind, ExampleMap, KeyAnswer>>;
using MapContainsLucky = Normalize_t<Apply_t<MapContainsKey, ExampleMap, KeyLucky>>;
using MapSizeExample = Normalize_t<Apply_t<MapSize, ExampleMap>>;

using Lx = Sym<'x'>;
using Ly = Sym<'y'>;
using Lz = Sym<'z'>;
using Lmake = Sym<'m', 'a', 'k', 'e'>;

using LispIdentity = LambdaExpr<Params<Param<Lx, IntType>>, Ref<Lx>>;
using LispClosureProgram =
    LetExpr<
        Bindings<
            Binding<Lx, Int<10>>,
            Binding<
                Lmake,
                LambdaExpr<
                    Params<Param<Ly, IntType>>,
                    LambdaExpr<
                        Params<Param<Lz, IntType>>,
                        CallExpr<Add, CallExpr<Add, Ref<Lx>, Ref<Ly>>, Ref<Lz>>
                    >
                >
            >
        >,
        CallExpr<CallExpr<Ref<Lmake>, Int<5>>, Int<7>>
    >;
using LispClosureResult = EvalLisp_t<LispClosureProgram>;
using LispClosureResultType = TypeCheck_t<LispClosureProgram>;

using LispStringProgram =
    LetExpr<
        Bindings<Binding<Sym<'m', 's', 'g'>, LambdaString>>,
        CallExpr<StringConcat, Ref<Sym<'m', 's', 'g'>>, String<' ', 'r', 'u', 'n', 't', 'i', 'm', 'e'>>
    >;
using LispStringResult = EvalLisp_t<LispStringProgram>;

using LispBadProgram = CallExpr<Add, String<'o', 'o', 'p', 's'>, Int<1>>;
using LispBadProgramType = TypeCheck_t<LispBadProgram>;

using ReaderClosureProgram = ReadSource_t<"(let ((x 10) (make (lambda ((y Int)) (lambda ((z Int)) (+ (+ x y) z))))) ((make 5) 7))">;
using ReaderClosureResult = ReadSourceEval_t<"(let ((x 10) (make (lambda ((y Int)) (lambda ((z Int)) (+ (+ x y) z))))) ((make 5) 7))">;
using ReaderClosureType = ReadSourceTypeCheck_t<"(let ((x 10) (make (lambda ((y Int)) (lambda ((z Int)) (+ (+ x y) z))))) ((make 5) 7))">;

using ReaderStringProgram = ReadSource_t<"(let ((msg \"lambda\")) (string-append msg \" runtime\"))">;
using ReaderStringResult = ReadSourceEval_t<"(let ((msg \"lambda\")) (string-append msg \" runtime\"))">;

using ReaderQuotedListProgram = ReadSource_t<"'(1 2 3)">;
using ReaderQuotedListResult = ReadSourceEval_t<"'(1 2 3)">;

using ReaderUntypedIdentityProgram = ReadSource_t<"((lambda (x) x) 42)">;
using ReaderUntypedIdentityResult = ReadSourceEval_t<"((lambda (x) x) 42)">;
using ReaderUntypedIdentityType = ReadSourceTypeCheck_t<"((lambda (x) x) 42)">;

using ReaderCommentProgram = ReadSource_t<R"(
    (begin
      ; comments are ignored by the reader
      ((lambda (msg) (string-append msg " reader")) "lambda"))
)">;
using ReaderCommentResult = ReadSourceEval_t<R"(
    (begin
      ; comments are ignored by the reader
      ((lambda (msg) (string-append msg " reader")) "lambda"))
)">;

using ReaderEscapedStringResult = ReadSourceEval_t<"\"lambda\\nreader\"">;
using ReaderBoolProgram = ReadSourceEval_t<"(if #t 1 2)">;

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
