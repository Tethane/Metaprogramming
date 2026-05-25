#pragma once

#include <utility>

#include "runtime.hpp"
#include "lisp.hpp"

namespace lc {

namespace pretty_detail {

template<typename... Strings>
struct StringCatMany;

template<>
struct StringCatMany<> {
    using type = String<>;
};

template<typename S>
struct StringCatMany<S> {
    using type = S;
};

template<typename A, typename B, typename... Rest>
struct StringCatMany<A, B, Rest...> {
    using type = typename StringCatMany<detail::StringConcat_t<A, B>, Rest...>::type;
};

template<typename... Strings>
using StringCatMany_t = typename StringCatMany<Strings...>::type;

template<auto Storage, typename Seq>
struct BigIntDigitsString;

template<auto Storage, std::size_t... Indices>
struct BigIntDigitsString<Storage, std::index_sequence<Indices...>> {
    using type = String<Storage.digits[Indices]...>;
};

template<auto Storage>
struct BigIntString {
private:
    inline static constexpr auto normalized = detail::normalize_bigint_storage(Storage);
    using digits = typename BigIntDigitsString<normalized, std::make_index_sequence<normalized.size>>::type;

public:
    using type = IfType_t<normalized.negative, StringCatMany_t<String<'-'>, digits>, digits>;
};

template<int N, bool Small = (N < 10)>
struct NatDigits;

template<int N>
struct NatDigits<N, true> {
    using type = String<static_cast<char>('0' + N)>;
};

template<int N>
struct NatDigits<N, false> {
    using type = StringCatMany_t<typename NatDigits<N / 10>::type, String<static_cast<char>('0' + (N % 10))>>;
};

template<int N>
struct IntString {
private:
    static constexpr int abs_value = (N < 0) ? -N : N;
    using digits = typename NatDigits<abs_value>::type;

public:
    using type = IfType_t<(N < 0), StringCatMany_t<String<'-'>, digits>, digits>;
};

template<typename T>
struct PrettyMessage {
    using type = typename T::type;
};

template<char... Chars>
struct PrettyMessage<String<Chars...>> {
    using type = String<Chars...>;
};

} // namespace pretty_detail

template<typename T>
struct Pretty {
    using type = String<'?'>;
};

template<typename T>
using Pretty_t = typename Pretty<T>::type;

template<typename Sep, typename... Items>
struct JoinPretty;

template<typename Sep>
struct JoinPretty<Sep> {
    using type = String<>;
};

template<typename Sep, typename First>
struct JoinPretty<Sep, First> {
    using type = Pretty_t<First>;
};

template<typename Sep, typename First, typename Second, typename... Rest>
struct JoinPretty<Sep, First, Second, Rest...> {
    using type = pretty_detail::StringCatMany_t<
        Pretty_t<First>,
        Sep,
        typename JoinPretty<Sep, Second, Rest...>::type
    >;
};

template<typename Name>
struct Pretty<Symbol<Name>> {
    using type = typename pretty_detail::PrettyMessage<Name>::type;
};

template<int N>
struct Pretty<Nat<N>> {
    using type = typename pretty_detail::IntString<N>::type;
};

template<int N>
struct Pretty<Int<N>> {
    using type = typename pretty_detail::IntString<N>::type;
};

template<auto Storage>
struct Pretty<BigInt<Storage>> {
    using type = typename pretty_detail::BigIntString<Storage>::type;
};

template<typename Num, typename Den>
struct Pretty<Rational<Num, Den>> {
    using type = pretty_detail::StringCatMany_t<Pretty_t<Num>, String<'/'>, Pretty_t<Den>>;
};

template<>
struct Pretty<Bool<true>> {
    using type = String<'t', 'r', 'u', 'e'>;
};

template<>
struct Pretty<Bool<false>> {
    using type = String<'f', 'a', 'l', 's', 'e'>;
};

template<char... Chars>
struct Pretty<String<Chars...>> {
    using type = pretty_detail::StringCatMany_t<String<'"'>, String<Chars...>, String<'"'>>;
};

template<typename... Items>
struct Pretty<List<Items...>> {
    using type = pretty_detail::StringCatMany_t<String<'['>, typename JoinPretty<String<',', ' '>, Items...>::type, String<']'>>;
};

template<typename... Items>
struct Pretty<Set<Items...>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '{'>, typename JoinPretty<String<',', ' '>, Items...>::type, String<'}'>>;
};

template<typename Key, typename Value>
struct Pretty<Entry<Key, Value>> {
    using type = pretty_detail::StringCatMany_t<Pretty_t<Key>, String<':', ' '>, Pretty_t<Value>>;
};

template<typename... Entries>
struct Pretty<AssocMap<Entries...>> {
    using type = pretty_detail::StringCatMany_t<String<'{'>, typename JoinPretty<String<',', ' '>, Entries...>::type, String<'}'>>;
};

template<>
struct Pretty<None> {
    using type = String<'n', 'o', 'n', 'e'>;
};

template<>
struct Pretty<Add> {
    using type = String<'+'>;
};

template<>
struct Pretty<Sub> {
    using type = String<'-'>;
};

template<>
struct Pretty<Mul> {
    using type = String<'*'>;
};

template<>
struct Pretty<Div> {
    using type = String<'/'>;
};

template<>
struct Pretty<Abs> {
    using type = String<'a', 'b', 's'>;
};

template<>
struct Pretty<IsZero> {
    using type = String<'z', 'e', 'r', 'o', '?'>;
};

template<>
struct Pretty<Eq> {
    using type = String<'='>;
};

template<>
struct Pretty<Lt> {
    using type = String<'<'>;
};

template<>
struct Pretty<Gt> {
    using type = String<'>'>;
};

template<>
struct Pretty<StringConcat> {
    using type = String<'s', 't', 'r', 'i', 'n', 'g', '-', 'a', 'p', 'p', 'e', 'n', 'd'>;
};

template<>
struct Pretty<StringLength> {
    using type = String<'s', 't', 'r', 'i', 'n', 'g', '-', 'l', 'e', 'n', 'g', 't', 'h'>;
};

template<>
struct Pretty<StringContains> {
    using type = String<'s', 't', 'r', 'i', 'n', 'g', '-', 'c', 'o', 'n', 't', 'a', 'i', 'n', 's'>;
};

template<>
struct Pretty<Cons> {
    using type = String<'c', 'o', 'n', 's'>;
};

template<>
struct Pretty<Head> {
    using type = String<'h', 'e', 'a', 'd'>;
};

template<>
struct Pretty<Tail> {
    using type = String<'t', 'a', 'i', 'l'>;
};

template<>
struct Pretty<IsEmpty> {
    using type = String<'e', 'm', 'p', 't', 'y', '?'>;
};

template<>
struct Pretty<Concat> {
    using type = String<'c', 'o', 'n', 'c', 'a', 't'>;
};

template<>
struct Pretty<Reverse> {
    using type = String<'r', 'e', 'v', 'e', 'r', 's', 'e'>;
};

template<>
struct Pretty<Length> {
    using type = String<'l', 'e', 'n', 'g', 't', 'h'>;
};

template<>
struct Pretty<SetUnion> {
    using type = String<'s', 'e', 't', '-', 'u', 'n', 'i', 'o', 'n'>;
};

template<>
struct Pretty<MapFind> {
    using type = String<'m', 'a', 'p', '-', 'g', 'e', 't'>;
};

template<int N>
struct Pretty<Var<N>> {
    using type = pretty_detail::StringCatMany_t<String<'v'>, typename pretty_detail::IntString<N>::type>;
};

template<typename Body>
struct Pretty<Lambda<Body>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'l', 'a', 'm', 'b', 'd', 'a', ' '>, Pretty_t<Body>, String<')'>>;
};

template<typename F, typename A>
struct Pretty<App<F, A>> {
    using type = Pretty_t<Canonicalize_t<App<F, A>>>;
};

template<typename Head, typename... Args>
struct Pretty<Call<Head, Args...>> {
    using type = pretty_detail::StringCatMany_t<String<'('>, typename JoinPretty<String<' '>, Head, Args...>::type, String<')'>>;
};

template<typename Name>
struct Pretty<Free<Name>> {
    using type = String<'f', 'r', 'e', 'e'>;
};

template<typename Name>
struct Pretty<Ref<Name>> {
    using type = Pretty_t<Name>;
};

template<typename Value>
struct Pretty<Quote<Value>> {
    using type = pretty_detail::StringCatMany_t<String<'\''>, Pretty_t<Value>>;
};

template<typename Expr>
struct Pretty<SList<SSymbol<String<'q', 'u', 'o', 't', 'e'>>, Expr>> {
    using type = pretty_detail::StringCatMany_t<String<'\''>, Pretty_t<Expr>>;
};

template<typename Name, typename Type>
struct Pretty<Param<Name, Type>> {
    using type = pretty_detail::StringCatMany_t<String<'('>, Pretty_t<Name>, String<' '>, Pretty_t<Type>, String<')'>>;
};

template<typename... ParamsT>
struct Pretty<Params<ParamsT...>> {
    using type = typename JoinPretty<String<' '>, ParamsT...>::type;
};

template<typename ParamsT, typename Body>
struct Pretty<LambdaExpr<ParamsT, Body>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'l', 'a', 'm', 'b', 'd', 'a', ' ', '('>,
        Pretty_t<ParamsT>,
        String<')', ' '>,
        Pretty_t<Body>,
        String<')'>
    >;
};

template<typename FnExpr, typename... ArgExprs>
struct Pretty<CallExpr<FnExpr, ArgExprs...>> {
    using type = pretty_detail::StringCatMany_t<String<'('>, typename JoinPretty<String<' '>, FnExpr, ArgExprs...>::type, String<')'>>;
};

template<typename Cond, typename ThenExpr, typename ElseExpr>
struct Pretty<IfExpr<Cond, ThenExpr, ElseExpr>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'i', 'f', ' '>,
        Pretty_t<Cond>,
        String<' '>,
        Pretty_t<ThenExpr>,
        String<' '>,
        Pretty_t<ElseExpr>,
        String<')'>
    >;
};

template<typename Name, typename Expr>
struct Pretty<Binding<Name, Expr>> {
    using type = pretty_detail::StringCatMany_t<String<'('>, Pretty_t<Name>, String<' '>, Pretty_t<Expr>, String<')'>>;
};

template<typename... BindingsT>
struct Pretty<Bindings<BindingsT...>> {
    using type = typename JoinPretty<String<' '>, BindingsT...>::type;
};

template<typename BindingsT, typename Body>
struct Pretty<LetExpr<BindingsT, Body>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'l', 'e', 't', ' ', '('>,
        Pretty_t<BindingsT>,
        String<')', ' '>,
        Pretty_t<Body>,
        String<')'>
    >;
};

template<typename Expr>
struct Pretty<BeginExpr<Expr>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'b', 'e', 'g', 'i', 'n', ' '>, Pretty_t<Expr>, String<')'>>;
};

template<typename Expr, typename NextExpr, typename... RestExprs>
struct Pretty<BeginExpr<Expr, NextExpr, RestExprs...>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'b', 'e', 'g', 'i', 'n', ' '>, typename JoinPretty<String<' '>, Expr, NextExpr, RestExprs...>::type, String<')'>>;
};

template<typename Name, typename Expr>
struct Pretty<DefineForm<Name, Expr>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'd', 'e', 'f', 'i', 'n', 'e', ' '>,
        Pretty_t<Name>,
        String<' '>,
        Pretty_t<Expr>,
        String<')'>
    >;
};

template<typename Expr>
struct Pretty<ExprForm<Expr>> {
    using type = Pretty_t<Expr>;
};

template<typename Form>
struct Pretty<ProgramExpr<Form>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'p', 'r', 'o', 'g', 'r', 'a', 'm', ' '>, Pretty_t<Form>, String<')'>>;
};

template<typename FirstForm, typename SecondForm, typename... RestForms>
struct Pretty<ProgramExpr<FirstForm, SecondForm, RestForms...>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'p', 'r', 'o', 'g', 'r', 'a', 'm', ' '>,
        typename JoinPretty<String<' '>, FirstForm, SecondForm, RestForms...>::type,
        String<')'>
    >;
};

template<typename ParamsT, typename Body, typename Env>
struct Pretty<Closure<ParamsT, Body, Env>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 'c', 'l', 'o', 's', 'u', 'r', 'e', ' '>, Pretty_t<ParamsT>, String<'>'> >;
};

template<typename Name, typename ParamsT, typename Body, typename Env>
struct Pretty<RecursiveClosure<Name, ParamsT, Body, Env>> {
    using type = pretty_detail::StringCatMany_t<
        String<'#', '<', 'r', 'e', 'c', '-', 'c', 'l', 'o', 's', 'u', 'r', 'e', ' '>,
        Pretty_t<Name>,
        String<' '>,
        Pretty_t<ParamsT>,
        String<'>'>
    >;
};

template<>
struct Pretty<NatType> {
    using type = String<'N', 'a', 't'>;
};

template<>
struct Pretty<IntType> {
    using type = String<'I', 'n', 't'>;
};

template<>
struct Pretty<BigIntType> {
    using type = String<'B', 'i', 'g', 'I', 'n', 't'>;
};

template<>
struct Pretty<RationalType> {
    using type = String<'R', 'a', 't', 'i', 'o', 'n', 'a', 'l'>;
};

template<>
struct Pretty<BoolType> {
    using type = String<'B', 'o', 'o', 'l'>;
};

template<>
struct Pretty<StringType> {
    using type = String<'S', 't', 'r', 'i', 'n', 'g'>;
};

template<>
struct Pretty<NoneType> {
    using type = String<'N', 'o', 'n', 'e'>;
};

template<>
struct Pretty<AnyType> {
    using type = String<'A', 'n', 'y'>;
};

template<>
struct Pretty<UnknownType> {
    using type = String<'U', 'n', 'k', 'n', 'o', 'w', 'n'>;
};

template<>
struct Pretty<InferType> {
    using type = String<'I', 'n', 'f', 'e', 'r'>;
};

template<typename Elem>
struct Pretty<ListType<Elem>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'L', 'i', 's', 't', ' '>, Pretty_t<Elem>, String<')'>>;
};

template<typename Elem>
struct Pretty<SetType<Elem>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'S', 'e', 't', ' '>, Pretty_t<Elem>, String<')'>>;
};

template<typename Key, typename Value>
struct Pretty<MapType<Key, Value>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'M', 'a', 'p', ' '>, Pretty_t<Key>, String<' '>, Pretty_t<Value>, String<')'>>;
};

template<typename ParamsT, typename ReturnType>
struct Pretty<FunctionType<ParamsT, ReturnType>> {
    using type = pretty_detail::StringCatMany_t<String<'(', '-', '>', ' ', '('>, Pretty_t<ParamsT>, String<')', ' '>, Pretty_t<ReturnType>, String<')'>>;
};

template<typename Primitive>
struct Pretty<PrimitiveType<Primitive>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'P', 'r', 'i', 'm', ' '>, Pretty_t<Primitive>, String<')'>>;
};

template<typename Message>
struct Pretty<EvalError<Message>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 'e', 'v', 'a', 'l', '-', 'e', 'r', 'r', 'o', 'r', ' '>, typename pretty_detail::PrettyMessage<Message>::type, String<'>'> >;
};

template<typename Message>
struct Pretty<TypeError<Message>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 't', 'y', 'p', 'e', '-', 'e', 'r', 'r', 'o', 'r', ' '>, typename pretty_detail::PrettyMessage<Message>::type, String<'>'> >;
};

template<typename Message>
struct Pretty<ReaderError<Message>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 'r', 'e', 'a', 'd', 'e', 'r', '-', 'e', 'r', 'r', 'o', 'r', ' '>, typename pretty_detail::PrettyMessage<Message>::type, String<'>'> >;
};

template<typename Label>
struct Pretty<ErrorFrame<Label, void>> {
    using type = Pretty_t<Label>;
};

template<typename Label, typename Detail>
struct Pretty<ErrorFrame<Label, Detail>> {
    using type = pretty_detail::StringCatMany_t<Pretty_t<Label>, String<':', ' '>, Pretty_t<Detail>>;
};

template<typename Frame, typename Inner>
struct Pretty<ErrorContext<Frame, Inner>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 'c', 't', 'x', ' '>, Pretty_t<Frame>, String<' '>, Pretty_t<Inner>, String<'>'> >;
};

template<typename Term>
struct Pretty<OutOfFuel<Term>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 'o', 'u', 't', '-', 'o', 'f', '-', 'f', 'u', 'e', 'l', ' '>, Pretty_t<Term>, String<'>'> >;
};

template<typename Term>
struct Pretty<CycleDetected<Term>> {
    using type = pretty_detail::StringCatMany_t<String<'#', '<', 'c', 'y', 'c', 'l', 'e', ' '>, Pretty_t<Term>, String<'>'> >;
};

template<typename Name>
struct Pretty<SSymbol<Name>> {
    using type = Pretty_t<Symbol<Name>>;
};

template<typename Token>
struct Pretty<SNumberLit<Token>> {
    using type = Pretty_t<Symbol<Token>>;
};

template<char... Chars>
struct Pretty<SStringLit<Chars...>> {
    using type = Pretty_t<String<Chars...>>;
};

template<bool B>
struct Pretty<SBool<B>> {
    using type = Pretty_t<Bool<B>>;
};

template<typename... Items>
struct Pretty<SList<Items...>> {
    using type = pretty_detail::StringCatMany_t<String<'('>, typename JoinPretty<String<' '>, Items...>::type, String<')'>>;
};

template<typename Form>
struct Pretty<SProgram<Form>> {
    using type = Pretty_t<Form>;
};

template<typename FirstForm, typename SecondForm, typename... RestForms>
struct Pretty<SProgram<FirstForm, SecondForm, RestForms...>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 't', 'o', 'p', ' '>,
        typename JoinPretty<String<' '>, FirstForm, SecondForm, RestForms...>::type,
        String<')'>
    >;
};

template<typename T>
inline constexpr std::string_view pretty_string_view_v = to_string_view_v<Pretty_t<T>>;

} // namespace lc
