#pragma once

namespace lc {

// ============================================================
// tiny metaprogramming prelude, no std. (PURITY)
// ============================================================

template<bool Cond, typename Then, typename Else>
struct IfType;

template<typename Then, typename Else>
struct IfType<true, Then, Else> {
    using type = Then;
};

template<typename Then, typename Else>
struct IfType<false, Then, Else> {
    using type = Else;
};

template<bool Cond, typename Then, typename Else>
using IfType_t = typename IfType<Cond, Then, Else>::type;


template<typename A, typename B>
struct IsSame {
    static constexpr bool value = false;
};

template<typename A>
struct IsSame<A, A> {
    static constexpr bool value = true;
};


template<bool A, bool B>
struct OrBool {
    static constexpr bool value = true;
};

template<>
struct OrBool<false, false> {
    static constexpr bool value = false;
};


template<typename...>
struct TypeList {};

template<typename X, typename List>
struct Contains;

template<typename X>
struct Contains<X, TypeList<>> {
    static constexpr bool value = false;
};

template<typename X, typename Head, typename... Tail>
struct Contains<X, TypeList<Head, Tail...>> {
    static constexpr bool value =
        OrBool<IsSame<X, Head>::value, Contains<X, TypeList<Tail...>>::value>::value;
};

template<typename X, typename List>
struct PushFront;

template<typename X, typename... Ts>
struct PushFront<X, TypeList<Ts...>> {
    using type = TypeList<X, Ts...>;
};

template<typename X, typename List>
using PushFront_t = typename PushFront<X, List>::type;


// ============================================================
// Core pure untyped lambda calculus AST
// ============================================================

template<int N>
struct Var {};

template<typename Body>
struct Lambda {};

template<typename F, typename A>
struct App {};

// Optional explicit free variables/constants for testing.
template<typename Name>
struct Free {};


// ============================================================
// Left-associative application helper
//
// Apply_t<A, B, C, D> means (((A B) C) D)
// ============================================================

template<typename... Terms>
struct Apply;

template<typename F>
struct Apply<F> {
    using type = F;
};

template<typename F, typename A, typename... Rest>
struct Apply<F, A, Rest...> {
    using type = typename Apply<App<F, A>, Rest...>::type;
};

template<typename... Terms>
using Apply_t = typename Apply<Terms...>::type;


// ============================================================
// Classic combinators
// ============================================================

using I = Lambda<Var<0>>;
using K = Lambda<Lambda<Var<1>>>;
using KI = Lambda<Lambda<Var<0>>>;

using S = Lambda<Lambda<Lambda<App<App<Var<2>, Var<0>>,App<Var<1>, Var<0>>>>>>;
using O = App<Lambda<App<Var<0>, Var<0>>>, Lambda<App<Var<0>, Var<0>>>>; // Omega
using Y = Lambda<App<Lambda<App<Var<1>, App<Var<0>, Var<0>>>>, Lambda<App<Var<1>, App<Var<0>, Var<0>>>>>>; // Y-combinator (plzzz)

// ============================================================
// De Bruijn shifting
//
// Shift<D, Cutoff, Term> adds D to every Var<N> where N >= Cutoff.
// Going under Lambda increments the cutoff.
// ============================================================

template<int D, int Cutoff, typename Term>
struct Shift;

template<int D, int Cutoff, int N>
struct Shift<D, Cutoff, Var<N>> {
    using type = IfType_t<
        (N >= Cutoff),
        Var<N + D>,
        Var<N>
    >;
};

template<int D, int Cutoff, typename Body>
struct Shift<D, Cutoff, Lambda<Body>> {
    using type = Lambda<typename Shift<D, Cutoff + 1, Body>::type>;
};

template<int D, int Cutoff, typename F, typename A>
struct Shift<D, Cutoff, App<F, A>> {
    using type = App<
        typename Shift<D, Cutoff, F>::type,
        typename Shift<D, Cutoff, A>::type
    >;
};

template<int D, int Cutoff, typename Name>
struct Shift<D, Cutoff, Free<Name>> {
    using type = Free<Name>;
};

template<int D, int Cutoff, typename Term>
using ShiftFrom_t = typename Shift<D, Cutoff, Term>::type;

template<int D, typename Term>
using Shift_t = ShiftFrom_t<D, 0, Term>;


// ============================================================
// De Bruijn substitution
//
// Subst<J, Replacement, Term> replaces Var<J> with Replacement.
// Under Lambda, J increases and Replacement is shifted upward.
// ============================================================

template<int J, typename Replacement, typename Term>
struct Subst;

template<int J, typename Replacement, int N>
struct Subst<J, Replacement, Var<N>> {
    using type = IfType_t<
        (N == J),
        Replacement,
        Var<N>
    >;
};

template<int J, typename Replacement, typename Body>
struct Subst<J, Replacement, Lambda<Body>> {
    using type = Lambda<
        typename Subst<
            J + 1,
            Shift_t<1, Replacement>,
            Body
        >::type
    >;
};

template<int J, typename Replacement, typename F, typename A>
struct Subst<J, Replacement, App<F, A>> {
    using type = App<
        typename Subst<J, Replacement, F>::type,
        typename Subst<J, Replacement, A>::type
    >;
};

template<int J, typename Replacement, typename Name>
struct Subst<J, Replacement, Free<Name>> {
    using type = Free<Name>;
};

template<int J, typename Replacement, typename Term>
using Subst_t = typename Subst<J, Replacement, Term>::type;


// ============================================================
// One-step normal-order beta reduction
//
// Step<Term> performs exactly one leftmost-outermost beta step.
// It returns StepResult<NewTerm, Changed>.
//
// This Changed flag is important because Omega reduces to itself.
// ============================================================

template<typename Term, bool Changed>
struct StepResult {
    using term = Term;
    static constexpr bool changed = Changed;
};

template<typename Term>
struct Step;

template<int N>
struct Step<Var<N>> {
    using type = StepResult<Var<N>, false>;
};

template<typename Name>
struct Step<Free<Name>> {
    using type = StepResult<Free<Name>, false>;
};

template<typename Body>
struct Step<Lambda<Body>> {
private:
    using body_step = typename Step<Body>::type;

public:
    using type = StepResult<
        Lambda<typename body_step::term>,
        body_step::changed
    >;
};

// Direct beta redex: (Lambda Body) Arg
//
// (lambda. body) arg
//   => shift(-1, subst(0, shift(+1, arg), body))
template<typename Body, typename Arg>
struct Step<App<Lambda<Body>, Arg>> {
    using type = StepResult<
        Shift_t<
            -1,
            Subst_t<0, Shift_t<1, Arg>, Body>
        >,
        true
    >;
};

// Helper for stepping applications whose function side is not already Lambda.
template<typename OriginalF, typename OriginalA, typename FStep, bool FChanged>
struct StepAppAfterFunction;

// If the function part changed, return the changed function with the original argument.
template<typename OriginalF, typename OriginalA, typename FStep>
struct StepAppAfterFunction<OriginalF, OriginalA, FStep, true> {
    using type = StepResult<App<typename FStep::term, OriginalA>, true>;
};

// If the function part did not change, try reducing the argument.
template<typename OriginalF, typename OriginalA, typename FStep>
struct StepAppAfterFunction<OriginalF, OriginalA, FStep, false> {
private:
    using astep = typename Step<OriginalA>::type;

public:
    using type = StepResult<
        App<OriginalF, typename astep::term>,
        astep::changed
    >;
};

// General application case.
template<typename F, typename A>
struct Step<App<F, A>> {
private:
    using fstep = typename Step<F>::type;

public:
    using type = typename StepAppAfterFunction<
        F,
        A,
        fstep,
        fstep::changed
    >::type;
};

template<typename Term>
using Step_t = typename Step<Term>::type;


// ============================================================
// Bounded normalization + optional exact cycle detection
//
// Normalize<Term, Fuel> repeatedly calls Step until:
//   - no beta redex remains, giving a normal form
//   - Fuel reaches zero, giving OutOfFuel<Term>
//   - a term repeats exactly, giving CycleDetected<Term>
//
// Exact cycle detection catches Omega, but it cannot solve the
// halting problem in general.
// ============================================================

template<typename Term>
struct OutOfFuel {
    using term = Term;
};

template<typename Term>
struct CycleDetected {
    using term = Term;
};

template<typename Term, int Fuel = 256, typename Seen = TypeList<>>
struct Normalize;

template<typename Term, typename Seen, int Fuel>
struct NormalizeImplSeen;

template<typename Term, typename Seen, int Fuel>
struct NormalizeImplStep;

template<typename Term, int Fuel, typename Seen>
struct Normalize {
    using type = typename NormalizeImplSeen<
        Term,
        Seen,
        Fuel
    >::type;
};

template<typename Term, typename Seen>
struct Normalize<Term, 0, Seen> {
    using type = OutOfFuel<Term>;
};

// If the term has appeared before, report a syntactic cycle.
template<typename Term, typename Seen, int Fuel>
struct NormalizeImplSeen {
    using type = typename IfType<
        Contains<Term, Seen>::value,
        CycleDetected<Term>,
        typename NormalizeImplStep<Term, Seen, Fuel>::type
    >::type;
};

// Step the term once and decide whether to stop or continue.
template<typename StepResultT, typename Term, typename Seen, int Fuel>
struct NormalizeAfterStep;

template<typename NextTerm, typename Term, typename Seen, int Fuel>
struct NormalizeAfterStep<StepResult<NextTerm, false>, Term, Seen, Fuel> {
    using type = Term;
};

template<typename NextTerm, typename Term, typename Seen, int Fuel>
struct NormalizeAfterStep<StepResult<NextTerm, true>, Term, Seen, Fuel> {
    using type = typename Normalize<
        NextTerm,
        Fuel - 1,
        PushFront_t<Term, Seen>
    >::type;
};

template<typename Term, typename Seen, int Fuel>
struct NormalizeImplStep {
private:
    using step = typename Step<Term>::type;

public:
    using type = typename NormalizeAfterStep<step, Term, Seen, Fuel>::type;
};

template<typename Term, int Fuel = 256>
using Normalize_t = typename Normalize<Term, Fuel>::type;


// ============================================================
// Church booleans
// ============================================================

using True = Lambda<Lambda<Var<1>>>;
using False = Lambda<Lambda<Var<0>>>;

// If = lambda b. lambda t. lambda f. b t f
using If =
    Lambda<
        Lambda<
            Lambda<
                Apply_t<Var<2>, Var<1>, Var<0>>
            >
        >
    >;


// ============================================================
// Small Church numerals and arithmetic
// ============================================================

using Zero = Lambda<Lambda<Var<0>>>;

using One = Lambda<Lambda<
    App<Var<1>, Var<0>>
>>;

using Two = Lambda<Lambda<
    App<Var<1>, App<Var<1>, Var<0>>>
>>;

using Three = Lambda<Lambda<
    App<Var<1>, App<Var<1>, App<Var<1>, Var<0>>>>
>>;

// IsZero = lambda n. n (lambda x. False) True
using IsZero = Lambda<Apply_t<Var<0>, Lambda<False>, True>>;

// Succ = lambda n. lambda f. lambda x. f (n f x)
using Succ = 
    Lambda<
        Lambda<
            Lambda<
                App<
                    Var<1>, 
                    Apply_t<
                        Var<2>, 
                        Var<1>, 
                        Var<0>
                    >
                >
            >
        >
    >;

using Pred = 
    Lambda<
        Lambda<
            Lambda<
                Apply_t<
                    Var<2>, 
                    Lambda<
                        Lambda<
                            App<
                                Var<0>, 
                                App<
                                    Var<1>, 
                                    Var<3>
                                >
                            >
                        >
                    >, 
                    Lambda<Var<1>>, 
                    Lambda<Var<0>>
                >
            >
        >
    >;


// Add = lambda m. lambda n. lambda f. lambda x. m f (n f x)
using Add = 
    Lambda<
        Lambda<
            Lambda<
                Lambda<
                    Apply_t<
                        Var<3>, 
                        Var<1>, 
                        Apply_t<
                            Var<2>, 
                            Var<1>, 
                            Var<0>
                        >
                    >
                >
            >
        >
    >;

// Mul = lambda m. lambda n. lambda f. m (n f)
using Mul = 
    Lambda<
        Lambda<
            Lambda<
                App<
                    Var<2>,
                    App<
                        Var<1>, 
                        Var<0>
                    >
                >
            >
        >
    >;

using FactF = 
    Lambda<
        Lambda<
            Apply_t<
                If, 
                Apply_t<IsZero, Var<0>>, 
                One, 
                Apply_t<
                    Mul, 
                    Var<0>, 
                    Apply_t<
                        Var<1>, 
                        Apply_t<
                            Pred, 
                            Var<0>
                        >
                    >
                >
            >
        >
    >;

using Factorial = Apply_t<Y, FactF>;


using Four = Normalize_t<App<Succ, Three>>;
using Five = Normalize_t<App<Succ, Four>>;
using Six = Normalize_t<App<Succ, Five>>;
using Seven = Normalize_t<App<Succ, Six>>;
using Eight = Normalize_t<App<Succ, Seven>>;
using Nine = Normalize_t<App<Succ, Eight>>;
using Ten = Normalize_t<App<Succ, Nine>>;

//============================================================
// Church Lists, foldr, map, and sum 
// ============================================================ 

// ListNil = lambda c. lambda n. n
//
// A Church list is a fold. It receives:
//   c : function used for each element
//   n : base value
// and reduces to the folded result.
using ListNil = Lambda<Lambda<Var<0>>>;

// ListCons = lambda h. lambda t. lambda c. lambda n.
//              c h (t c n)
//
// h = head
// t = tail
// c = fold step
// n = fold base
using ListCons =
    Lambda<
        Lambda<
            Lambda<
                Lambda<
                    Apply_t<
                        Var<1>,
                        Var<3>,
                        Apply_t<Var<2>, Var<1>, Var<0>>
                    >
                >
            >
        >
    >;
using Foldr =
    Lambda<
        Lambda<
            Lambda<
                Apply_t<Var<0>, Var<2>, Var<1>>
            >
        >
    >;

// Map = lambda f. lambda xs.
//         xs
//           (lambda h. lambda t. ListCons (f h) t)
//           ListNil
using Map =
    Lambda<
        Lambda<
            Apply_t<
                Var<0>,
                Lambda<
                    Lambda<
                        Apply_t<
                            ListCons,
                            Apply_t<Var<3>, Var<1>>,
                            Var<0>
                        >
                    >
                >,
                ListNil
            >
        >
    >;

// Sum = lambda xs. xs Add Zero
// This expects xs to be a list of Church numerals.
using Sum =
    Lambda<
        Apply_t<Var<0>, Add, Zero>
    >;

// Some useful example lists.
using ListOneTwoThree =
    Apply_t<
        ListCons,
        One,
        Apply_t<
            ListCons,
            Two,
            Apply_t<
                ListCons,
                Three,
                ListNil
            >
        >
    >;

using ListOneTwo =
    Apply_t<
        ListCons,
        One,
        Apply_t<
            ListCons,
            Two,
            ListNil
        >
    >;

// ============================================================
// Named surface syntax compiled to De Bruijn core
//
// Surface syntax:
//   var<x>
//   lam<x, Body>
//   app<F, A>
//
// Compilation:
//   surface named AST -> core De Bruijn AST
// ============================================================

template<typename Name>
struct var {};

template<typename Name, typename Body>
struct lam {};

template<typename F, typename A>
struct app {};

template<typename Name>
struct free {};

// Name lookup in compile-time context.
// The context is TypeList<nearest, next, next, ...>.
template<typename Name, typename Ctx>
struct IndexOf;

template<typename Name>
struct IndexOf<Name, TypeList<>>;

template<typename Name, typename... Tail>
struct IndexOf<Name, TypeList<Name, Tail...>> {
    static constexpr int value = 0;
};

template<typename Name, typename Head, typename... Tail>
struct IndexOf<Name, TypeList<Head, Tail...>> {
    static constexpr int value = 1 + IndexOf<Name, TypeList<Tail...>>::value;
};


template<typename Expr, typename Ctx = TypeList<>>
struct Compile;

template<typename Name, typename Ctx>
struct Compile<var<Name>, Ctx> {
    using type = Var<IndexOf<Name, Ctx>::value>;
};

template<typename Name, typename Ctx>
struct Compile<free<Name>, Ctx> {
    using type = Free<Name>;
};

template<typename Name, typename Body, typename Ctx>
struct Compile<lam<Name, Body>, Ctx> {
    using type = Lambda<
        typename Compile<Body, PushFront_t<Name, Ctx>>::type
    >;
};

template<typename F, typename A, typename Ctx>
struct Compile<app<F, A>, Ctx> {
    using type = App<
        typename Compile<F, Ctx>::type,
        typename Compile<A, Ctx>::type
    >;
};

template<typename Expr>
using Compile_t = typename Compile<Expr>::type;

template<typename Expr, int Fuel = 256>
using Run_t = Normalize_t<Compile_t<Expr>, Fuel>;


// ============================================================
// Demo names and test helpers
// ============================================================

struct a;
struct b;
struct c;
struct x;
struct y;
struct z;

using A = Free<a>;
using B = Free<b>;
using C = Free<c>;

} // namespace lc