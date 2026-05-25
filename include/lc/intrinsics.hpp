#pragma once

#include "core.hpp"

namespace lc {

struct If {};
struct Not {};
struct And {};
struct Or {};

struct Succ {};
struct Pred {};
struct Add {};
struct Sub {};
struct Mul {};
struct Div {};
struct Mod {};
struct Pow {};
struct Eq {};
struct Lt {};
struct Lte {};
struct Gt {};
struct Gte {};
struct IsZero {};

struct Cons {};
struct Head {};
struct Tail {};
struct IsEmpty {};
struct Concat {};
struct Reverse {};
struct Length {};
struct Range {};
struct Map {};
struct Filter {};
struct Foldl {};
struct Foldr {};
struct Sum {};
struct Product {};
struct Any {};
struct All {};
struct Sieve {};

namespace detail {

constexpr int pow_constexpr(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

template<typename T, typename ListT>
struct ListPushFront;

template<typename T, typename... Ts>
struct ListPushFront<T, List<Ts...>> {
    using type = List<T, Ts...>;
};

template<typename T, typename ListT>
using ListPushFront_t = typename ListPushFront<T, ListT>::type;

template<typename ListT, typename T>
struct ListPushBack;

template<typename... Ts, typename T>
struct ListPushBack<List<Ts...>, T> {
    using type = List<Ts..., T>;
};

template<typename ListT, typename T>
using ListPushBack_t = typename ListPushBack<ListT, T>::type;

template<int Begin, int End, bool Done = (Begin > End)>
struct RangeList;

template<int Begin, int End>
struct RangeList<Begin, End, true> {
    using type = List<>;
};

template<int Begin, int End>
struct RangeList<Begin, End, false> {
    using type = ListPushFront_t<Nat<Begin>, typename RangeList<Begin + 1, End>::type>;
};

template<int Begin, int End>
using RangeList_t = typename RangeList<Begin, End>::type;

template<typename Func, typename ListT>
struct MapList;

template<typename Func>
struct MapList<Func, List<>> {
    using type = List<>;
};

template<typename Func, typename Item, typename... Rest>
struct MapList<Func, List<Item, Rest...>> {
    using mapped_item = Normalize_t<Apply_t<Func, Item>, 4096>;
    using type = ListPushFront_t<mapped_item, typename MapList<Func, List<Rest...>>::type>;
};

template<typename Func, typename ListT>
using MapList_t = typename MapList<Func, ListT>::type;

template<typename Predicate, typename ListT>
struct FilterList;

template<typename Predicate>
struct FilterList<Predicate, List<>> {
    using type = List<>;
};

template<typename Predicate, typename Item, typename... Rest>
struct FilterList<Predicate, List<Item, Rest...>> {
    using keep = Normalize_t<Apply_t<Predicate, Item>, 4096>;
    using tail = typename FilterList<Predicate, List<Rest...>>::type;
    using type = IfType_t<keep::value, ListPushFront_t<Item, tail>, tail>;
};

template<typename Predicate, typename ListT>
using FilterList_t = typename FilterList<Predicate, ListT>::type;

template<typename Func, typename Acc, typename ListT>
struct FoldlList;

template<typename Func, typename Acc>
struct FoldlList<Func, Acc, List<>> {
    using type = Acc;
};

template<typename Func, typename Acc, typename Item, typename... Rest>
struct FoldlList<Func, Acc, List<Item, Rest...>> {
    using next_acc = Normalize_t<Apply_t<Func, Acc, Item>, 4096>;
    using type = typename FoldlList<Func, next_acc, List<Rest...>>::type;
};

template<typename Func, typename Acc, typename ListT>
using FoldlList_t = typename FoldlList<Func, Acc, ListT>::type;

template<typename Func, typename Acc, typename ListT>
struct FoldrList;

template<typename Func, typename Acc>
struct FoldrList<Func, Acc, List<>> {
    using type = Acc;
};

template<typename Func, typename Acc, typename Item, typename... Rest>
struct FoldrList<Func, Acc, List<Item, Rest...>> {
    using rest_value = typename FoldrList<Func, Acc, List<Rest...>>::type;
    using type = Normalize_t<Apply_t<Func, Item, rest_value>, 4096>;
};

template<typename Func, typename Acc, typename ListT>
using FoldrList_t = typename FoldrList<Func, Acc, ListT>::type;

template<typename Left, typename Right>
struct ConcatLists;

template<typename... LeftItems, typename... RightItems>
struct ConcatLists<List<LeftItems...>, List<RightItems...>> {
    using type = List<LeftItems..., RightItems...>;
};

template<typename Left, typename Right>
using ConcatLists_t = typename ConcatLists<Left, Right>::type;

template<typename Remaining, typename Acc = List<>>
struct ReverseAcc;

template<typename... AccItems>
struct ReverseAcc<List<>, List<AccItems...>> {
    using type = List<AccItems...>;
};

template<typename Item, typename... Rest, typename... AccItems>
struct ReverseAcc<List<Item, Rest...>, List<AccItems...>> {
    using type = typename ReverseAcc<List<Rest...>, List<Item, AccItems...>>::type;
};

template<typename ListT>
using ReverseList_t = typename ReverseAcc<ListT>::type;

template<typename ListT>
struct SumList;

template<>
struct SumList<List<>> {
    using type = Nat<0>;
};

template<int N, typename... Rest>
struct SumList<List<Nat<N>, Rest...>> {
    using rest = typename SumList<List<Rest...>>::type;
    using type = Nat<N + rest::value>;
};

template<typename ListT>
using SumList_t = typename SumList<ListT>::type;

template<typename ListT>
struct ProductList;

template<>
struct ProductList<List<>> {
    using type = Nat<1>;
};

template<int N, typename... Rest>
struct ProductList<List<Nat<N>, Rest...>> {
    using rest = typename ProductList<List<Rest...>>::type;
    using type = Nat<N * rest::value>;
};

template<typename ListT>
using ProductList_t = typename ProductList<ListT>::type;

template<typename ListT>
struct AnyList;

template<>
struct AnyList<List<>> {
    using type = Bool<false>;
};

template<bool B, typename... Rest>
struct AnyList<List<Bool<B>, Rest...>> {
    using rest = typename AnyList<List<Rest...>>::type;
    using type = Bool<B || rest::value>;
};

template<typename ListT>
using AnyList_t = typename AnyList<ListT>::type;

template<typename ListT>
struct AllList;

template<>
struct AllList<List<>> {
    using type = Bool<true>;
};

template<bool B, typename... Rest>
struct AllList<List<Bool<B>, Rest...>> {
    using rest = typename AllList<List<Rest...>>::type;
    using type = Bool<B && rest::value>;
};

template<typename ListT>
using AllList_t = typename AllList<ListT>::type;

template<int Prime, typename ListT>
struct RemoveMultiples;

template<int Prime>
struct RemoveMultiples<Prime, List<>> {
    using type = List<>;
};

template<int Prime, int N, typename... Rest>
struct RemoveMultiples<Prime, List<Nat<N>, Rest...>> {
    using tail = typename RemoveMultiples<Prime, List<Rest...>>::type;
    using type = IfType_t<(N % Prime != 0), ListPushFront_t<Nat<N>, tail>, tail>;
};

template<int Prime, typename ListT>
using RemoveMultiples_t = typename RemoveMultiples<Prime, ListT>::type;

template<typename ListT>
struct SieveList;

template<>
struct SieveList<List<>> {
    using type = List<>;
};

template<int Prime, typename... Rest>
struct SieveList<List<Nat<Prime>, Rest...>> {
    using filtered_tail = RemoveMultiples_t<Prime, List<Rest...>>;
    using tail_primes = typename SieveList<filtered_tail>::type;
    using type = ListPushFront_t<Nat<Prime>, tail_primes>;
};

template<typename ListT>
using SieveList_t = typename SieveList<ListT>::type;

template<typename Term>
struct IntrinsicStep {
    using type = ReductionResult<Term, false>;
};

template<bool Cond, typename ThenTerm, typename ElseTerm>
struct IntrinsicStep<Call<If, Bool<Cond>, ThenTerm, ElseTerm>> {
    using type = ReductionResult<IfType_t<Cond, ThenTerm, ElseTerm>, true>;
};

template<bool Value>
struct IntrinsicStep<Call<Not, Bool<Value>>> {
    using type = ReductionResult<Bool<!Value>, true>;
};

template<bool Left, bool Right>
struct IntrinsicStep<Call<And, Bool<Left>, Bool<Right>>> {
    using type = ReductionResult<Bool<Left && Right>, true>;
};

template<bool Left, bool Right>
struct IntrinsicStep<Call<Or, Bool<Left>, Bool<Right>>> {
    using type = ReductionResult<Bool<Left || Right>, true>;
};

template<int N>
struct IntrinsicStep<Call<Succ, Nat<N>>> {
    using type = ReductionResult<Nat<N + 1>, true>;
};

template<int N>
struct IntrinsicStep<Call<Pred, Nat<N>>> {
    using type = ReductionResult<Nat<(N > 0 ? N - 1 : 0)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Add, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<Left + Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Sub, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Left >= Right ? Left - Right : 0)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mul, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<Left * Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Div, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Right == 0 ? 0 : Left / Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mod, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Right == 0 ? 0 : Left % Right)>, true>;
};

template<int Base, int Exp>
struct IntrinsicStep<Call<Pow, Nat<Base>, Nat<Exp>>> {
    using type = ReductionResult<Nat<pow_constexpr(Base, Exp)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Eq, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left == Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lt, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left < Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lte, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left <= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gt, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left > Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gte, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left >= Right)>, true>;
};

template<int N>
struct IntrinsicStep<Call<IsZero, Nat<N>>> {
    using type = ReductionResult<Bool<(N == 0)>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<Cons, Item, List<Items...>>> {
    using type = ReductionResult<List<Item, Items...>, true>;
};

template<typename Item, typename... Rest>
struct IntrinsicStep<Call<Head, List<Item, Rest...>>> {
    using type = ReductionResult<Item, true>;
};

template<>
struct IntrinsicStep<Call<Head, List<>>> {
    using type = ReductionResult<List<>, false>;
};

template<typename Item, typename... Rest>
struct IntrinsicStep<Call<Tail, List<Item, Rest...>>> {
    using type = ReductionResult<List<Rest...>, true>;
};

template<>
struct IntrinsicStep<Call<Tail, List<>>> {
    using type = ReductionResult<List<>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<IsEmpty, List<Items...>>> {
    using type = ReductionResult<Bool<(sizeof...(Items) == 0)>, true>;
};

template<typename... LeftItems, typename... RightItems>
struct IntrinsicStep<Call<Concat, List<LeftItems...>, List<RightItems...>>> {
    using type = ReductionResult<List<LeftItems..., RightItems...>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Reverse, List<Items...>>> {
    using type = ReductionResult<ReverseList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Length, List<Items...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Items))>, true>;
};

template<int Begin, int End>
struct IntrinsicStep<Call<Range, Nat<Begin>, Nat<End>>> {
    using type = ReductionResult<RangeList_t<Begin, End>, true>;
};

template<typename Func, typename... Items>
struct IntrinsicStep<Call<Map, Func, List<Items...>>> {
    using type = ReductionResult<MapList_t<Func, List<Items...>>, true>;
};

template<typename Predicate, typename... Items>
struct IntrinsicStep<Call<Filter, Predicate, List<Items...>>> {
    using type = ReductionResult<FilterList_t<Predicate, List<Items...>>, true>;
};

template<typename Func, typename Acc, typename... Items>
struct IntrinsicStep<Call<Foldl, Func, Acc, List<Items...>>> {
    using type = ReductionResult<FoldlList_t<Func, Acc, List<Items...>>, true>;
};

template<typename Func, typename Acc, typename... Items>
struct IntrinsicStep<Call<Foldr, Func, Acc, List<Items...>>> {
    using type = ReductionResult<FoldrList_t<Func, Acc, List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Sum, List<Items...>>> {
    using type = ReductionResult<SumList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Product, List<Items...>>> {
    using type = ReductionResult<ProductList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Any, List<Items...>>> {
    using type = ReductionResult<AnyList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<All, List<Items...>>> {
    using type = ReductionResult<AllList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Sieve, List<Items...>>> {
    using type = ReductionResult<SieveList_t<List<Items...>>, true>;
};

} // namespace detail

} // namespace lc
