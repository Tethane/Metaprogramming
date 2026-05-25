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

template<auto Storage, std::size_t Begin, typename Seq>
struct DecimalDigitsSlice;

template<auto Storage, std::size_t Begin, std::size_t... Indices>
struct DecimalDigitsSlice<Storage, Begin, std::index_sequence<Indices...>> {
    using type = String<Storage.digits[Begin + Indices]...>;
};

template<std::size_t Count>
struct ZeroString {
    using type = StringCatMany_t<String<'0'>, typename ZeroString<Count - 1>::type>;
};

template<>
struct ZeroString<0> {
    using type = String<>;
};

template<auto Storage>
struct BigIntString {
private:
    inline static constexpr auto normalized = detail::normalize_bigint_storage(Storage);
    using digits = typename BigIntDigitsString<normalized, std::make_index_sequence<normalized.size>>::type;

public:
    using type = IfType_t<normalized.negative, StringCatMany_t<String<'-'>, digits>, digits>;
};

template<auto Storage>
struct DecimalString {
private:
    inline static constexpr auto normalized = detail::normalize_decimal_storage(Storage);
    inline static constexpr std::size_t integer_digits =
        normalized.scale > 0 && static_cast<std::size_t>(normalized.scale) < normalized.size
            ? normalized.size - static_cast<std::size_t>(normalized.scale)
            : 0;

    using all_digits = typename BigIntDigitsString<normalized, std::make_index_sequence<normalized.size>>::type;
    using integer_part = IfType_t<
        (normalized.scale > 0 && static_cast<std::size_t>(normalized.scale) < normalized.size),
        typename DecimalDigitsSlice<normalized, 0, std::make_index_sequence<integer_digits>>::type,
        String<'0'>
    >;
    using fractional_existing = IfType_t<
        (normalized.scale > 0 && static_cast<std::size_t>(normalized.scale) < normalized.size),
        typename DecimalDigitsSlice<normalized, integer_digits, std::make_index_sequence<normalized.size - integer_digits>>::type,
        all_digits
    >;

    using fractional_part = IfType_t<
        (normalized.scale > 0 && static_cast<std::size_t>(normalized.scale) >= normalized.size),
        StringCatMany_t<
            typename ZeroString<static_cast<std::size_t>(normalized.scale) - normalized.size>::type,
            all_digits
        >,
        fractional_existing
    >;

    using unsigned_type = IfType_t<
        (normalized.scale <= 0),
        all_digits,
        StringCatMany_t<integer_part, String<'.'>, fractional_part>
    >;

public:
    using type = IfType_t<normalized.negative, StringCatMany_t<String<'-'>, unsigned_type>, unsigned_type>;
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

template<auto Storage>
struct Pretty<Decimal<Storage>> {
    using type = typename pretty_detail::DecimalString<Storage>::type;
};

template<typename Num, typename Den>
struct Pretty<Rational<Num, Den>> {
    using type = pretty_detail::StringCatMany_t<Pretty_t<Num>, String<'/'>, Pretty_t<Den>>;
};

template<>
struct Pretty<Irrational<pi_tag>> {
    using type = String<'p', 'i'>;
};

template<>
struct Pretty<Irrational<e_tag>> {
    using type = String<'e'>;
};

template<>
struct Pretty<Irrational<tau_tag>> {
    using type = String<'t', 'a', 'u'>;
};

template<typename Op, typename... Args>
struct Pretty<RealExpr<Op, Args...>> {
    using type = pretty_detail::StringCatMany_t<String<'('>, typename JoinPretty<String<' '>, Op, Args...>::type, String<')'>>;
};

template<typename Real, typename Imag>
struct Pretty<Complex<Real, Imag>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'c', 'o', 'm', 'p', 'l', 'e', 'x', ' '>,
        Pretty_t<Real>,
        String<' '>,
        Pretty_t<Imag>,
        String<')'>
    >;
};

template<typename... Elems>
struct Pretty<Vector<Elems...>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'v', 'e', 'c', 't', 'o', 'r'>,
        IfType_t<
            sizeof...(Elems) == 0,
            String<')'>,
            pretty_detail::StringCatMany_t<String<' '>, typename JoinPretty<String<' '>, Elems...>::type, String<')'>>
        >
    >;
};

template<typename... Rows>
struct Pretty<Matrix<Rows...>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'm', 'a', 't', 'r', 'i', 'x'>,
        IfType_t<
            sizeof...(Rows) == 0,
            String<')'>,
            pretty_detail::StringCatMany_t<String<' '>, typename JoinPretty<String<' '>, Rows...>::type, String<')'>>
        >
    >;
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
struct Pretty<Sqrt> {
    using type = String<'s', 'q', 'r', 't'>;
};

template<>
struct Pretty<Exp> {
    using type = String<'e', 'x', 'p'>;
};

template<>
struct Pretty<Log> {
    using type = String<'l', 'o', 'g'>;
};

template<>
struct Pretty<Sin> {
    using type = String<'s', 'i', 'n'>;
};

template<>
struct Pretty<Cos> {
    using type = String<'c', 'o', 's'>;
};

template<>
struct Pretty<Tan> {
    using type = String<'t', 'a', 'n'>;
};

template<>
struct Pretty<Asin> {
    using type = String<'a', 's', 'i', 'n'>;
};

template<>
struct Pretty<Acos> {
    using type = String<'a', 'c', 'o', 's'>;
};

template<>
struct Pretty<Atan> {
    using type = String<'a', 't', 'a', 'n'>;
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

template<>
struct Pretty<VectorCtor> {
    using type = String<'v', 'e', 'c', 't', 'o', 'r'>;
};

template<>
struct Pretty<MatrixCtor> {
    using type = String<'m', 'a', 't', 'r', 'i', 'x'>;
};

template<>
struct Pretty<ComplexCtor> {
    using type = String<'c', 'o', 'm', 'p', 'l', 'e', 'x'>;
};

template<>
struct Pretty<VecAdd> {
    using type = String<'v', 'e', 'c', '-', 'a', 'd', 'd'>;
};

template<>
struct Pretty<VecSub> {
    using type = String<'v', 'e', 'c', '-', 's', 'u', 'b'>;
};

template<>
struct Pretty<VecScale> {
    using type = String<'v', 'e', 'c', '-', 's', 'c', 'a', 'l', 'e'>;
};

template<>
struct Pretty<Dot> {
    using type = String<'d', 'o', 't'>;
};

template<>
struct Pretty<Norm> {
    using type = String<'n', 'o', 'r', 'm'>;
};

template<>
struct Pretty<NormalizeVector> {
    using type = String<'n', 'o', 'r', 'm', 'a', 'l', 'i', 'z', 'e'>;
};

template<>
struct Pretty<MatAdd> {
    using type = String<'m', 'a', 't', '-', 'a', 'd', 'd'>;
};

template<>
struct Pretty<MatSub> {
    using type = String<'m', 'a', 't', '-', 's', 'u', 'b'>;
};

template<>
struct Pretty<MatScale> {
    using type = String<'m', 'a', 't', '-', 's', 'c', 'a', 'l', 'e'>;
};

template<>
struct Pretty<MatVecMul> {
    using type = String<'m', 'a', 't', '-', 'v', 'e', 'c', '-', 'm', 'u', 'l'>;
};

template<>
struct Pretty<MatMul> {
    using type = String<'m', 'a', 't', '-', 'm', 'u', 'l'>;
};

template<>
struct Pretty<Transpose> {
    using type = String<'t', 'r', 'a', 'n', 's', 'p', 'o', 's', 'e'>;
};

template<>
struct Pretty<Determinant> {
    using type = String<'d', 'e', 't'>;
};

template<>
struct Pretty<Inverse> {
    using type = String<'i', 'n', 'v', 'e', 'r', 's', 'e'>;
};

template<>
struct Pretty<Conjugate> {
    using type = String<'c', 'o', 'n', 'j', 'u', 'g', 'a', 't', 'e'>;
};

template<>
struct Pretty<NormSquared> {
    using type = String<'n', 'o', 'r', 'm', '-', 's', 'q', 'u', 'a', 'r', 'e', 'd'>;
};

template<>
struct Pretty<Magnitude> {
    using type = String<'m', 'a', 'g', 'n', 'i', 't', 'u', 'd', 'e'>;
};

template<>
struct Pretty<Argument> {
    using type = String<'a', 'r', 'g', 'u', 'm', 'e', 'n', 't'>;
};

template<>
struct Pretty<Mean> {
    using type = String<'m', 'e', 'a', 'n'>;
};

template<>
struct Pretty<Median> {
    using type = String<'m', 'e', 'd', 'i', 'a', 'n'>;
};

template<>
struct Pretty<Mode> {
    using type = String<'m', 'o', 'd', 'e'>;
};

template<>
struct Pretty<Variance> {
    using type = String<'v', 'a', 'r', 'i', 'a', 'n', 'c', 'e'>;
};

template<>
struct Pretty<StdDev> {
    using type = String<'s', 't', 'd', 'd', 'e', 'v'>;
};

template<>
struct Pretty<Minimum> {
    using type = String<'m', 'i', 'n'>;
};

template<>
struct Pretty<Maximum> {
    using type = String<'m', 'a', 'x'>;
};

template<>
struct Pretty<StatRange> {
    using type = String<'r', 'a', 'n', 'g', 'e'>;
};

template<>
struct Pretty<Covariance> {
    using type = String<'c', 'o', 'v', 'a', 'r', 'i', 'a', 'n', 'c', 'e'>;
};

template<>
struct Pretty<Correlation> {
    using type = String<'c', 'o', 'r', 'r', 'e', 'l', 'a', 't', 'i', 'o', 'n'>;
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
struct Pretty<DecimalType> {
    using type = String<'D', 'e', 'c', 'i', 'm', 'a', 'l'>;
};

template<>
struct Pretty<RealType> {
    using type = String<'R', 'e', 'a', 'l'>;
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

template<typename Elem>
struct Pretty<ComplexType<Elem>> {
    using type = pretty_detail::StringCatMany_t<String<'(', 'C', 'o', 'm', 'p', 'l', 'e', 'x', ' '>, Pretty_t<Elem>, String<')'>>;
};

template<typename Elem, std::size_t N>
struct Pretty<VectorType<Elem, N>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'V', 'e', 'c', 't', 'o', 'r', ' '>,
        Pretty_t<Elem>,
        String<' '>,
        typename pretty_detail::IntString<static_cast<int>(N)>::type,
        String<')'>
    >;
};

template<typename Elem, std::size_t Rows, std::size_t Cols>
struct Pretty<MatrixType<Elem, Rows, Cols>> {
    using type = pretty_detail::StringCatMany_t<
        String<'(', 'M', 'a', 't', 'r', 'i', 'x', ' '>,
        Pretty_t<Elem>,
        String<' '>,
        typename pretty_detail::IntString<static_cast<int>(Rows)>::type,
        String<' '>,
        typename pretty_detail::IntString<static_cast<int>(Cols)>::type,
        String<')'>
    >;
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

template<typename Value, int Reductions, int Nodes, int Approximations>
struct Pretty<ComputationStats<Value, Reductions, Nodes, Approximations>> {
    using type = pretty_detail::StringCatMany_t<
        String<'#', '<', 's', 't', 'a', 't', 's', ' '>,
        Pretty_t<Value>,
        String<' ', 'r', '='>,
        typename pretty_detail::IntString<Reductions>::type,
        String<' ', 'n', '='>,
        typename pretty_detail::IntString<Nodes>::type,
        String<' ', 'a', '='>,
        typename pretty_detail::IntString<Approximations>::type,
        String<'>'>
    >;
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
