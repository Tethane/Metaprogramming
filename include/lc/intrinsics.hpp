#pragma once

#include <cstddef>

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
struct TwoSum {};
struct MaxSubarraySum {};
struct ThreeSum {};
struct StringConcat {};
struct StringLength {};
struct StringEq {};
struct StringContains {};
struct StringStartsWith {};
struct StringTake {};
struct StringDrop {};
struct SetInsert {};
struct SetContains {};
struct SetErase {};
struct SetUnion {};
struct SetIntersection {};
struct SetSize {};
struct MapInsert {};
struct MapContainsKey {};
struct MapFind {};
struct MapErase {};
struct MapSize {};

namespace detail {

constexpr int pow_constexpr(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

template<typename Number>
struct ValueOf;

template<int N>
struct ValueOf<Nat<N>> : Nat<N> {};

template<int N>
struct ValueOf<Int<N>> : Int<N> {};

template<typename T>
inline constexpr int value_of_v = ValueOf<T>::value;

template<std::size_t N, std::size_t M>
constexpr bool string_starts_with_chars(const char (&text)[N], const char (&prefix)[M]) {
    if constexpr (M == 1) {
        return true;
    } else if constexpr (N < M) {
        return false;
    } else {
        for (std::size_t i = 0; i < M - 1; ++i) {
            if (text[i] != prefix[i]) {
                return false;
            }
        }
        return true;
    }
}

template<std::size_t N, std::size_t M>
constexpr bool string_contains_chars(const char (&text)[N], const char (&needle)[M]) {
    if constexpr (M == 1) {
        return true;
    } else if constexpr (N < M) {
        return false;
    } else {
        for (std::size_t offset = 0; offset + (M - 1) <= (N - 1); ++offset) {
            bool match = true;
            for (std::size_t i = 0; i < M - 1; ++i) {
                if (text[offset + i] != needle[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return true;
            }
        }
        return false;
    }
}

template<typename StrT, typename AccT = String<>>
struct StringReverseAcc;

template<char... AccChars>
struct StringReverseAcc<String<>, String<AccChars...>> {
    using type = String<AccChars...>;
};

template<char First, char... Rest, char... AccChars>
struct StringReverseAcc<String<First, Rest...>, String<AccChars...>> {
    using type = typename StringReverseAcc<String<Rest...>, String<First, AccChars...>>::type;
};

template<typename StrT>
using StringReverse_t = typename StringReverseAcc<StrT>::type;

template<typename StrT, typename TailT>
struct StringConcatT;

template<char... Left, char... Right>
struct StringConcatT<String<Left...>, String<Right...>> {
    using type = String<Left..., Right...>;
};

template<typename Left, typename Right>
using StringConcat_t = typename StringConcatT<Left, Right>::type;

template<int Count, typename StrT, bool Done = (Count == 0)>
struct StringTakeImpl;

template<int Count, bool Done>
struct StringTakeImpl<Count, String<>, Done> {
    using type = String<>;
};

template<int Count, char First, char... Rest>
struct StringTakeImpl<Count, String<First, Rest...>, true> {
    using type = String<>;
};

template<int Count, char First, char... Rest>
struct StringTakeImpl<Count, String<First, Rest...>, false> {
    using type = StringConcat_t<String<First>, typename StringTakeImpl<Count - 1, String<Rest...>>::type>;
};

template<int Count, typename StrT>
using StringTake_t = typename StringTakeImpl<Count, StrT>::type;

template<int Count, typename StrT, bool Done = (Count == 0)>
struct StringDropImpl;

template<int Count, bool Done>
struct StringDropImpl<Count, String<>, Done> {
    using type = String<>;
};

template<int Count, char... Chars>
struct StringDropImpl<Count, String<Chars...>, true> {
    using type = String<Chars...>;
};

template<int Count, char First, char... Rest>
struct StringDropImpl<Count, String<First, Rest...>, false> {
    using type = typename StringDropImpl<Count - 1, String<Rest...>>::type;
};

template<int Count, typename StrT>
using StringDrop_t = typename StringDropImpl<Count, StrT>::type;

template<typename Item, typename SetT>
struct SetHas;

template<typename Item>
struct SetHas<Item, Set<>> : std::false_type {};

template<typename Item, typename Head, typename... Tail>
struct SetHas<Item, Set<Head, Tail...>>
    : std::bool_constant<IsSame<Item, Head>::value || SetHas<Item, Set<Tail...>>::value> {};

template<typename Item, typename SetT>
struct SetInsertT;

template<typename Item, typename... Items>
struct SetInsertT<Item, Set<Items...>> {
    using type = IfType_t<SetHas<Item, Set<Items...>>::value, Set<Items...>, Set<Items..., Item>>;
};

template<typename Item, typename SetT>
using SetInsert_t = typename SetInsertT<Item, SetT>::type;

template<typename Item, typename SetT>
struct SetPrependT;

template<typename Item, typename... Items>
struct SetPrependT<Item, Set<Items...>> {
    using type = Set<Item, Items...>;
};

template<typename Item, typename SetT>
using SetPrepend_t = typename SetPrependT<Item, SetT>::type;

template<typename Item, typename SetT>
struct SetEraseT;

template<typename Item>
struct SetEraseT<Item, Set<>> {
    using type = Set<>;
};

template<typename Item, typename Head, typename... Tail>
struct SetEraseT<Item, Set<Head, Tail...>> {
private:
    using tail = typename SetEraseT<Item, Set<Tail...>>::type;

public:
    using type = IfType_t<IsSame<Item, Head>::value, tail, SetPrepend_t<Head, tail>>;
};

template<typename Item, typename SetT>
using SetErase_t = typename SetEraseT<Item, SetT>::type;

template<typename LeftSet, typename RightSet>
struct SetUnionT;

template<typename... LeftItems>
struct SetUnionT<Set<LeftItems...>, Set<>> {
    using type = Set<LeftItems...>;
};

template<typename... LeftItems, typename Item, typename... Rest>
struct SetUnionT<Set<LeftItems...>, Set<Item, Rest...>> {
    using type = typename SetUnionT<SetInsert_t<Item, Set<LeftItems...>>, Set<Rest...>>::type;
};

template<typename LeftSet, typename RightSet>
using SetUnion_t = typename SetUnionT<LeftSet, RightSet>::type;

template<typename LeftSet, typename RightSet, typename Acc = Set<>>
struct SetIntersectionAcc;

template<typename RightSet, typename... AccItems>
struct SetIntersectionAcc<Set<>, RightSet, Set<AccItems...>> {
    using type = Set<AccItems...>;
};

template<typename Head, typename... Tail, typename RightSet, typename... AccItems>
struct SetIntersectionAcc<Set<Head, Tail...>, RightSet, Set<AccItems...>> {
private:
    using next_acc = IfType_t<SetHas<Head, RightSet>::value, SetInsert_t<Head, Set<AccItems...>>, Set<AccItems...>>;

public:
    using type = typename SetIntersectionAcc<Set<Tail...>, RightSet, next_acc>::type;
};

template<typename LeftSet, typename RightSet>
using SetIntersection_t = typename SetIntersectionAcc<LeftSet, RightSet>::type;

template<typename Key, typename MapT>
struct AssocFindT;

template<typename Key>
struct AssocFindT<Key, AssocMap<>> {
    using type = None;
};

template<typename Key, typename Value, typename... Rest>
struct AssocFindT<Key, AssocMap<Entry<Key, Value>, Rest...>> {
    using type = Value;
};

template<typename Key, typename EntryT, typename... Rest>
struct AssocFindT<Key, AssocMap<EntryT, Rest...>> {
    using type = typename AssocFindT<Key, AssocMap<Rest...>>::type;
};

template<typename Key, typename MapT>
using AssocFind_t = typename AssocFindT<Key, MapT>::type;

template<typename Key, typename Value, typename MapT>
struct AssocInsertT;

template<typename Key, typename Value>
struct AssocInsertT<Key, Value, AssocMap<>> {
    using type = AssocMap<Entry<Key, Value>>;
};

template<typename Key, typename Value, typename ExistingValue, typename... Rest>
struct AssocInsertT<Key, Value, AssocMap<Entry<Key, ExistingValue>, Rest...>> {
    using type = AssocMap<Entry<Key, Value>, Rest...>;
};

template<typename Key, typename Value, typename EntryT, typename... Rest>
struct AssocInsertT<Key, Value, AssocMap<EntryT, Rest...>> {
private:
    using tail = typename AssocInsertT<Key, Value, AssocMap<Rest...>>::type;

public:
    template<typename... TailEntries>
    static AssocMap<EntryT, TailEntries...> rebuild(AssocMap<TailEntries...>);

    using type = decltype(rebuild(tail{}));
};

template<typename Key, typename Value, typename MapT>
using AssocInsert_t = typename AssocInsertT<Key, Value, MapT>::type;

template<typename Key, typename MapT>
struct AssocEraseT;

template<typename Key>
struct AssocEraseT<Key, AssocMap<>> {
    using type = AssocMap<>;
};

template<typename Key, typename Value, typename... Rest>
struct AssocEraseT<Key, AssocMap<Entry<Key, Value>, Rest...>> {
    using type = AssocMap<Rest...>;
};

template<typename Key, typename EntryT, typename... Rest>
struct AssocEraseT<Key, AssocMap<EntryT, Rest...>> {
private:
    using tail = typename AssocEraseT<Key, AssocMap<Rest...>>::type;

public:
    template<typename... TailEntries>
    static AssocMap<EntryT, TailEntries...> rebuild(AssocMap<TailEntries...>);

    using type = decltype(rebuild(tail{}));
};

template<typename Key, typename MapT>
using AssocErase_t = typename AssocEraseT<Key, MapT>::type;

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

template<typename T, typename ListT>
struct ListContains;

template<typename T>
struct ListContains<T, List<>> : std::false_type {};

template<typename T, typename Head, typename... Tail>
struct ListContains<T, List<Head, Tail...>>
    : std::bool_constant<IsSame<T, Head>::value || ListContains<T, List<Tail...>>::value> {};

template<typename Left, typename Right>
struct UniqueConcatLists;

template<typename... LeftItems>
struct UniqueConcatLists<List<LeftItems...>, List<>> {
    using type = List<LeftItems...>;
};

template<typename... LeftItems, typename Item, typename... Rest>
struct UniqueConcatLists<List<LeftItems...>, List<Item, Rest...>> {
private:
    using appended = IfType_t<
        ListContains<Item, List<LeftItems...>>::value,
        List<LeftItems...>,
        List<LeftItems..., Item>
    >;

public:
    using type = typename UniqueConcatLists<appended, List<Rest...>>::type;
};

template<typename Left, typename Right>
using UniqueConcatLists_t = typename UniqueConcatLists<Left, Right>::type;

template<int Needed, int Index, typename ListT>
struct FindValueIndex;

template<int Needed, int Index>
struct FindValueIndex<Needed, Index, List<>> {
    using type = List<>;
};

template<bool Match, int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndexStep;

template<int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndexStep<true, Needed, Index, Item, Rest...> {
    using type = List<Nat<Index>>;
};

template<int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndexStep<false, Needed, Index, Item, Rest...> {
    using type = typename FindValueIndex<Needed, Index + 1, List<Rest...>>::type;
};

template<int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndex<Needed, Index, List<Item, Rest...>> {
    using type = typename FindValueIndexStep<
        (value_of_v<Item> == Needed),
        Needed,
        Index,
        Item,
        Rest...
    >::type;
};

template<int Target, int Index, typename ListT>
struct TwoSumList;

template<int Target, int Index>
struct TwoSumList<Target, Index, List<>> {
    using type = List<>;
};

template<typename InnerResult, int CurrentIndex, int Target, int NextIndex, typename RestList>
struct TwoSumListAfterInner;

template<int CurrentIndex, int Target, int NextIndex, typename RestList>
struct TwoSumListAfterInner<List<>, CurrentIndex, Target, NextIndex, RestList> {
    using type = typename TwoSumList<Target, NextIndex, RestList>::type;
};

template<typename MatchIndex, int CurrentIndex, int Target, int NextIndex, typename RestList>
struct TwoSumListAfterInner {
    using type = ListPushFront_t<Nat<CurrentIndex>, MatchIndex>;
};

template<int Target, int Index, typename Item, typename... Rest>
struct TwoSumList<Target, Index, List<Item, Rest...>> {
private:
    using inner = typename FindValueIndex<Target - value_of_v<Item>, Index + 1, List<Rest...>>::type;

public:
    using type = typename TwoSumListAfterInner<inner, Index, Target, Index + 1, List<Rest...>>::type;
};

template<int Target, typename ListT>
using TwoSumList_t = typename TwoSumList<Target, 0, ListT>::type;

template<int CurrentBest, int Running, typename ListT>
struct MaxSubarrayScan;

template<int CurrentBest, int Running>
struct MaxSubarrayScan<CurrentBest, Running, List<>> {
    using type = Int<CurrentBest>;
};

template<int CurrentBest, int Running, typename Item, typename... Rest>
struct MaxSubarrayScan<CurrentBest, Running, List<Item, Rest...>> {
private:
    static constexpr int item_value = value_of_v<Item>;
    static constexpr int next_running = (Running + item_value > item_value) ? (Running + item_value) : item_value;
    static constexpr int next_best = (CurrentBest > next_running) ? CurrentBest : next_running;

public:
    using type = typename MaxSubarrayScan<next_best, next_running, List<Rest...>>::type;
};

template<typename ListT>
struct MaxSubarrayList;

template<>
struct MaxSubarrayList<List<>> {
    using type = Int<0>;
};

template<typename Item, typename... Rest>
struct MaxSubarrayList<List<Item, Rest...>> {
    using type = typename MaxSubarrayScan<value_of_v<Item>, value_of_v<Item>, List<Rest...>>::type;
};

template<typename ListT>
using MaxSubarrayList_t = typename MaxSubarrayList<ListT>::type;

template<int First, int Second, typename ListT>
struct ThirdSumMatches;

template<int First, int Second>
struct ThirdSumMatches<First, Second, List<>> {
    using type = List<>;
};

template<int First, int Second, typename Item, typename... Rest>
struct ThirdSumMatches<First, Second, List<Item, Rest...>> {
private:
    using tail = typename ThirdSumMatches<First, Second, List<Rest...>>::type;
    using triplet = List<Int<First>, Int<Second>, Int<value_of_v<Item>>>;

public:
    using type = IfType_t<
        (First + Second + value_of_v<Item> == 0),
        UniqueConcatLists_t<List<triplet>, tail>,
        tail
    >;
};

template<int First, typename ListT>
struct ThreeSumSecondLoop;

template<int First>
struct ThreeSumSecondLoop<First, List<>> {
    using type = List<>;
};

template<int First, typename Item, typename... Rest>
struct ThreeSumSecondLoop<First, List<Item, Rest...>> {
private:
    using with_second = typename ThirdSumMatches<First, value_of_v<Item>, List<Rest...>>::type;
    using without_second = typename ThreeSumSecondLoop<First, List<Rest...>>::type;

public:
    using type = UniqueConcatLists_t<with_second, without_second>;
};

template<typename ListT>
struct ThreeSumList;

template<>
struct ThreeSumList<List<>> {
    using type = List<>;
};

template<typename Item>
struct ThreeSumList<List<Item>> {
    using type = List<>;
};

template<typename First, typename Second>
struct ThreeSumList<List<First, Second>> {
    using type = List<>;
};

template<typename Item, typename... Rest>
struct ThreeSumList<List<Item, Rest...>> {
private:
    using with_first = typename ThreeSumSecondLoop<value_of_v<Item>, List<Rest...>>::type;
    using without_first = typename ThreeSumList<List<Rest...>>::type;

public:
    using type = UniqueConcatLists_t<with_first, without_first>;
};

template<typename ListT>
using ThreeSumList_t = typename ThreeSumList<ListT>::type;

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
struct IntrinsicStep<Call<Add, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<Left + Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Sub, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Left >= Right ? Left - Right : 0)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Sub, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<Left - Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mul, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<Left * Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mul, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<Left * Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Div, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Right == 0 ? 0 : Left / Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Div, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<(Right == 0 ? 0 : Left / Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mod, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Right == 0 ? 0 : Left % Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mod, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<(Right == 0 ? 0 : Left % Right)>, true>;
};

template<int Base, int Exp>
struct IntrinsicStep<Call<Pow, Nat<Base>, Nat<Exp>>> {
    using type = ReductionResult<Nat<pow_constexpr(Base, Exp)>, true>;
};

template<int Base, int Exp>
struct IntrinsicStep<Call<Pow, Int<Base>, Nat<Exp>>> {
    using type = ReductionResult<Int<pow_constexpr(Base, Exp)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Eq, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left == Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Eq, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left == Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lt, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left < Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lt, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left < Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lte, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left <= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lte, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left <= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gt, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left > Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gt, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left > Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gte, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left >= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gte, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left >= Right)>, true>;
};

template<int N>
struct IntrinsicStep<Call<IsZero, Nat<N>>> {
    using type = ReductionResult<Bool<(N == 0)>, true>;
};

template<int N>
struct IntrinsicStep<Call<IsZero, Int<N>>> {
    using type = ReductionResult<Bool<(N == 0)>, true>;
};

template<char... LeftChars, char... RightChars>
struct IntrinsicStep<Call<StringConcat, String<LeftChars...>, String<RightChars...>>> {
    using type = ReductionResult<String<LeftChars..., RightChars...>, true>;
};

template<char... Chars>
struct IntrinsicStep<Call<StringLength, String<Chars...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Chars))>, true>;
};

template<char... LeftChars, char... RightChars>
struct IntrinsicStep<Call<StringEq, String<LeftChars...>, String<RightChars...>>> {
    using type = ReductionResult<Bool<IsSame<String<LeftChars...>, String<RightChars...>>::value>, true>;
};

template<char... TextChars, char... NeedleChars>
struct IntrinsicStep<Call<StringContains, String<TextChars...>, String<NeedleChars...>>> {
private:
    inline static constexpr char text[] = {TextChars..., '\0'};
    inline static constexpr char needle[] = {NeedleChars..., '\0'};

public:
    using type = ReductionResult<Bool<string_contains_chars(text, needle)>, true>;
};

template<char... TextChars, char... PrefixChars>
struct IntrinsicStep<Call<StringStartsWith, String<TextChars...>, String<PrefixChars...>>> {
private:
    inline static constexpr char text[] = {TextChars..., '\0'};
    inline static constexpr char prefix[] = {PrefixChars..., '\0'};

public:
    using type = ReductionResult<Bool<string_starts_with_chars(text, prefix)>, true>;
};

template<int Count, char... Chars>
struct IntrinsicStep<Call<StringTake, Nat<Count>, String<Chars...>>> {
    using type = ReductionResult<StringTake_t<Count, String<Chars...>>, true>;
};

template<int Count, char... Chars>
struct IntrinsicStep<Call<StringDrop, Nat<Count>, String<Chars...>>> {
    using type = ReductionResult<StringDrop_t<Count, String<Chars...>>, true>;
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

template<char... Chars>
struct IntrinsicStep<Call<Length, String<Chars...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Chars))>, true>;
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

template<typename Item, typename... Items>
struct IntrinsicStep<Call<SetContains, Set<Items...>, Item>> {
    using type = ReductionResult<Bool<SetHas<Item, Set<Items...>>::value>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<SetInsert, Set<Items...>, Item>> {
    using type = ReductionResult<SetInsert_t<Item, Set<Items...>>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<SetErase, Set<Items...>, Item>> {
    using type = ReductionResult<SetErase_t<Item, Set<Items...>>, true>;
};

template<typename... LeftItems, typename... RightItems>
struct IntrinsicStep<Call<SetUnion, Set<LeftItems...>, Set<RightItems...>>> {
    using type = ReductionResult<SetUnion_t<Set<LeftItems...>, Set<RightItems...>>, true>;
};

template<typename... LeftItems, typename... RightItems>
struct IntrinsicStep<Call<SetIntersection, Set<LeftItems...>, Set<RightItems...>>> {
    using type = ReductionResult<SetIntersection_t<Set<LeftItems...>, Set<RightItems...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<SetSize, Set<Items...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Items))>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Length, Set<Items...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Items))>, true>;
};

template<typename Key, typename Value, typename... Entries>
struct IntrinsicStep<Call<MapInsert, AssocMap<Entries...>, Key, Value>> {
    using type = ReductionResult<AssocInsert_t<Key, Value, AssocMap<Entries...>>, true>;
};

template<typename Key, typename... Entries>
struct IntrinsicStep<Call<MapFind, AssocMap<Entries...>, Key>> {
    using type = ReductionResult<AssocFind_t<Key, AssocMap<Entries...>>, true>;
};

template<typename Key, typename... Entries>
struct IntrinsicStep<Call<MapContainsKey, AssocMap<Entries...>, Key>> {
    using type = ReductionResult<Bool<!IsSame<AssocFind_t<Key, AssocMap<Entries...>>, None>::value>, true>;
};

template<typename Key, typename... Entries>
struct IntrinsicStep<Call<MapErase, AssocMap<Entries...>, Key>> {
    using type = ReductionResult<AssocErase_t<Key, AssocMap<Entries...>>, true>;
};

template<typename... Entries>
struct IntrinsicStep<Call<MapSize, AssocMap<Entries...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Entries))>, true>;
};

template<typename... Entries>
struct IntrinsicStep<Call<Length, AssocMap<Entries...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Entries))>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Sieve, List<Items...>>> {
    using type = ReductionResult<SieveList_t<List<Items...>>, true>;
};

template<typename... Items, typename Target>
struct IntrinsicStep<Call<TwoSum, List<Items...>, Target>> {
    using type = ReductionResult<TwoSumList_t<value_of_v<Target>, List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<MaxSubarraySum, List<Items...>>> {
    using type = ReductionResult<MaxSubarrayList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<ThreeSum, List<Items...>>> {
    using type = ReductionResult<ThreeSumList_t<List<Items...>>, true>;
};

} // namespace detail

} // namespace lc
