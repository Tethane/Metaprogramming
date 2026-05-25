#pragma once

#include <type_traits>

namespace lc {

template<bool Cond, typename Then, typename Else>
struct IfType {
    using type = Then;
};

template<typename Then, typename Else>
struct IfType<false, Then, Else> {
    using type = Else;
};

template<bool Cond, typename Then, typename Else>
using IfType_t = typename IfType<Cond, Then, Else>::type;

template<typename A, typename B>
struct IsSame : std::bool_constant<std::is_same_v<A, B>> {};

template<bool A, bool B>
struct OrBool : std::bool_constant<A || B> {};

template<typename... Ts>
struct TypeList {};

template<typename X, typename List>
struct Contains;

template<typename X>
struct Contains<X, TypeList<>> : std::false_type {};

template<typename X, typename Head, typename... Tail>
struct Contains<X, TypeList<Head, Tail...>>
    : std::bool_constant<IsSame<X, Head>::value || Contains<X, TypeList<Tail...>>::value> {};

template<typename X, typename List>
struct PushFront;

template<typename X, typename... Ts>
struct PushFront<X, TypeList<Ts...>> {
    using type = TypeList<X, Ts...>;
};

template<typename X, typename List>
using PushFront_t = typename PushFront<X, List>::type;

template<typename Term, bool Changed>
struct ReductionResult {
    using term = Term;
    static constexpr bool changed = Changed;
};

template<int N>
struct Var {};

template<typename Body>
struct Lambda {};

template<typename F, typename A>
struct App {};

template<typename Head, typename... Args>
struct Call {};

template<typename Name>
struct Free {};

template<int N>
struct Nat {
    static constexpr int value = N;
};

template<int N>
struct Int {
    static constexpr int value = N;
};

template<bool B>
struct Bool {
    static constexpr bool value = B;
};

template<typename... Ts>
struct List {};

template<typename... Terms>
struct Apply;

template<typename Head, typename... Args>
struct MakeCall {
    using type = Call<Head, Args...>;
};

template<typename Head, typename... ExistingArgs, typename... Args>
struct MakeCall<Call<Head, ExistingArgs...>, Args...> {
    using type = Call<Head, ExistingArgs..., Args...>;
};

template<typename Head, typename... Args>
using MakeCall_t = typename MakeCall<Head, Args...>::type;

template<typename Term>
struct Apply<Term> {
    using type = Term;
};

template<typename Head, typename Arg, typename... Rest>
struct Apply<Head, Arg, Rest...> {
    using type = typename Apply<MakeCall_t<Head, Arg>, Rest...>::type;
};

template<typename... Terms>
using Apply_t = typename Apply<Terms...>::type;

template<typename Term>
struct Canonicalize {
    using type = Term;
};

template<typename Body>
struct Canonicalize<Lambda<Body>> {
    using type = Lambda<typename Canonicalize<Body>::type>;
};

template<typename F, typename A>
struct Canonicalize<App<F, A>> {
    using type = Apply_t<typename Canonicalize<F>::type, typename Canonicalize<A>::type>;
};

template<typename Head, typename... Args>
struct Canonicalize<Call<Head, Args...>> {
    using type = Apply_t<typename Canonicalize<Head>::type, typename Canonicalize<Args>::type...>;
};

template<typename Term>
using Canonicalize_t = typename Canonicalize<Term>::type;

template<int D, int Cutoff, typename Term>
struct Shift;

template<int D, typename Term>
using Shift_t = typename Shift<D, 0, Term>::type;

template<int J, typename Replacement, typename Term>
struct Subst;

template<int J, typename Replacement, typename Term>
using Subst_t = typename Subst<J, Replacement, Term>::type;

template<typename Term>
struct Step;

template<typename Term, int Fuel = 256>
struct Normalize;

template<typename Term, int Fuel = 256, typename Seen = TypeList<>>
struct NormalizeChecked;

template<typename Term, int Fuel = 256>
using Normalize_t = typename Normalize<Term, Fuel>::type;

template<typename Term, int Fuel = 256>
using NormalizeChecked_t = typename NormalizeChecked<Term, Fuel>::type;

template<typename Name>
struct var {};

template<typename Name, typename Body>
struct lam {};

template<typename F, typename A>
struct app {};

template<typename Name>
struct free {};

template<typename Name, typename Ctx>
struct IndexOf;

template<typename Name>
struct IndexOf<Name, TypeList<>> {
    static_assert(!std::is_same_v<Name, Name>, "Unbound variable in named lambda expression");
};

template<typename Name, typename... Tail>
struct IndexOf<Name, TypeList<Name, Tail...>> : std::integral_constant<int, 0> {};

template<typename Name, typename Head, typename... Tail>
struct IndexOf<Name, TypeList<Head, Tail...>>
    : std::integral_constant<int, 1 + IndexOf<Name, TypeList<Tail...>>::value> {};

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
    using type = Lambda<typename Compile<Body, PushFront_t<Name, Ctx>>::type>;
};

template<typename F, typename A, typename Ctx>
struct Compile<app<F, A>, Ctx> {
    using type = Apply_t<typename Compile<F, Ctx>::type, typename Compile<A, Ctx>::type>;
};

template<typename Expr>
using Compile_t = typename Compile<Expr>::type;

template<typename Expr, int Fuel = 256>
using Run_t = Normalize_t<Compile_t<Expr>, Fuel>;

template<typename Expr, int Fuel = 256>
using RunChecked_t = NormalizeChecked_t<Compile_t<Expr>, Fuel>;

template<typename Term>
struct OutOfFuel {
    using term = Term;
};

template<typename Term>
struct CycleDetected {
    using term = Term;
};

} // namespace lc
