#pragma once

#include "eval.hpp"

namespace lc {

template<typename Name>
struct Symbol {};

template<char... Chars>
using Sym = Symbol<String<Chars...>>;

template<typename Name>
struct Ref {};

template<typename Value>
struct Quote {};

template<typename Cond, typename ThenExpr, typename ElseExpr>
struct IfExpr {};

template<typename... ParamsT>
struct Params {};

template<typename Name, typename Type>
struct Param {
    using name = Name;
    using type = Type;
};

template<typename ParamsT, typename Body>
struct LambdaExpr {};

template<typename FnExpr, typename... ArgExprs>
struct CallExpr {};

template<typename Name, typename Expr>
struct Binding {
    using name = Name;
    using expr = Expr;
};

template<typename... BindingsT>
struct Bindings {};

template<typename BindingsT, typename Body>
struct LetExpr {};

template<typename... Exprs>
struct BeginExpr {};

template<typename... Forms>
struct ProgramExpr {};

template<typename Name, typename Expr>
struct DefineForm {
    using name = Name;
    using expr = Expr;
};

template<typename Expr>
struct ExprForm {
    using expr = Expr;
};

template<typename ParamsT, typename Body, typename Env>
struct Closure {};

template<typename Name, typename ParamsT, typename Body, typename Env>
struct RecursiveClosure {};

struct NatType {};
struct IntType {};
struct BigIntType {};
struct RationalType {};
struct DecimalType {};
struct RealType {};
struct BoolType {};
struct StringType {};
struct NoneType {};
struct AnyType {};
struct UnknownType {};
struct InferType {};

template<typename Elem>
struct ComplexType {};

template<typename Elem, std::size_t N>
struct VectorType {};

template<typename Elem, std::size_t Rows, std::size_t Cols>
struct MatrixType {};

template<typename Elem>
struct ListType {};

template<typename Elem>
struct SetType {};

template<typename Key, typename Value>
struct MapType {};

template<typename ParamsT, typename ReturnType>
struct FunctionType {};

template<typename Primitive>
struct PrimitiveType {};

template<typename Label, typename Detail = void>
struct ErrorFrame {};

template<typename Frame, typename Inner>
struct ErrorContext {
    using frame = Frame;
    using inner = Inner;
};

template<typename Message>
struct EvalError {
    using message = Message;
};

template<typename Message>
struct TypeError {
    using message = Message;
};

template<typename... Ts>
struct ValuePack {};

template<typename... Ts>
struct TypePack {};

template<typename T>
struct IsEvalError : std::false_type {};

template<typename Message>
struct IsEvalError<EvalError<Message>> : std::true_type {};

template<typename T>
struct IsTypeError : std::false_type {};

template<typename Message>
struct IsTypeError<TypeError<Message>> : std::true_type {};

using msg_unbound_symbol = String<'u', 'n', 'b', 'o', 'u', 'n', 'd', '-', 's', 'y', 'm', 'b', 'o', 'l'>;
using msg_invalid_condition = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'c', 'o', 'n', 'd', 'i', 't', 'i', 'o', 'n'>;
using msg_wrong_arity = String<'w', 'r', 'o', 'n', 'g', '-', 'a', 'r', 'i', 't', 'y'>;
using msg_invalid_application = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'a', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n'>;
using msg_expected_bool = String<'e', 'x', 'p', 'e', 'c', 't', 'e', 'd', '-', 'b', 'o', 'o', 'l'>;
using msg_branch_type_mismatch = String<'b', 'r', 'a', 'n', 'c', 'h', '-', 't', 'y', 'p', 'e', '-', 'm', 'i', 's', 'm', 'a', 't', 'c', 'h'>;
using msg_call_type_error = String<'c', 'a', 'l', 'l', '-', 't', 'y', 'p', 'e', '-', 'e', 'r', 'r', 'o', 'r'>;
using msg_heterogeneous_list = String<'h', 'e', 't', 'e', 'r', 'o', 'g', 'e', 'n', 'e', 'o', 'u', 's', '-', 'l', 'i', 's', 't'>;
using msg_heterogeneous_set = String<'h', 'e', 't', 'e', 'r', 'o', 'g', 'e', 'n', 'e', 'o', 'u', 's', '-', 's', 'e', 't'>;
using msg_heterogeneous_map = String<'h', 'e', 't', 'e', 'r', 'o', 'g', 'e', 'n', 'e', 'o', 'u', 's', '-', 'm', 'a', 'p'>;
using msg_recursive_value_define = String<'r', 'e', 'c', 'u', 'r', 's', 'i', 'v', 'e', '-', 'v', 'a', 'l', 'u', 'e', '-', 'd', 'e', 'f', 'i', 'n', 'e'>;
using msg_could_not_infer = String<'c', 'o', 'u', 'l', 'd', '-', 'n', 'o', 't', '-', 'i', 'n', 'f', 'e', 'r'>;
using msg_invalid_program = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'p', 'r', 'o', 'g', 'r', 'a', 'm'>;

template<typename T>
struct IsPrimitiveTag : std::false_type {};

template<> struct IsPrimitiveTag<Add> : std::true_type {};
template<> struct IsPrimitiveTag<Sub> : std::true_type {};
template<> struct IsPrimitiveTag<Mul> : std::true_type {};
template<> struct IsPrimitiveTag<Div> : std::true_type {};
template<> struct IsPrimitiveTag<Mod> : std::true_type {};
template<> struct IsPrimitiveTag<Pow> : std::true_type {};
template<> struct IsPrimitiveTag<Abs> : std::true_type {};
template<> struct IsPrimitiveTag<Eq> : std::true_type {};
template<> struct IsPrimitiveTag<Lt> : std::true_type {};
template<> struct IsPrimitiveTag<Lte> : std::true_type {};
template<> struct IsPrimitiveTag<Gt> : std::true_type {};
template<> struct IsPrimitiveTag<Gte> : std::true_type {};
template<> struct IsPrimitiveTag<IsZero> : std::true_type {};
template<> struct IsPrimitiveTag<StringConcat> : std::true_type {};
template<> struct IsPrimitiveTag<StringLength> : std::true_type {};
template<> struct IsPrimitiveTag<StringEq> : std::true_type {};
template<> struct IsPrimitiveTag<StringContains> : std::true_type {};
template<> struct IsPrimitiveTag<StringStartsWith> : std::true_type {};
template<> struct IsPrimitiveTag<StringTake> : std::true_type {};
template<> struct IsPrimitiveTag<StringDrop> : std::true_type {};
template<> struct IsPrimitiveTag<Cons> : std::true_type {};
template<> struct IsPrimitiveTag<Head> : std::true_type {};
template<> struct IsPrimitiveTag<Tail> : std::true_type {};
template<> struct IsPrimitiveTag<IsEmpty> : std::true_type {};
template<> struct IsPrimitiveTag<Concat> : std::true_type {};
template<> struct IsPrimitiveTag<Reverse> : std::true_type {};
template<> struct IsPrimitiveTag<Length> : std::true_type {};
template<> struct IsPrimitiveTag<SetInsert> : std::true_type {};
template<> struct IsPrimitiveTag<SetContains> : std::true_type {};
template<> struct IsPrimitiveTag<SetErase> : std::true_type {};
template<> struct IsPrimitiveTag<SetUnion> : std::true_type {};
template<> struct IsPrimitiveTag<SetIntersection> : std::true_type {};
template<> struct IsPrimitiveTag<SetSize> : std::true_type {};
template<> struct IsPrimitiveTag<MapInsert> : std::true_type {};
template<> struct IsPrimitiveTag<MapFind> : std::true_type {};
template<> struct IsPrimitiveTag<MapContainsKey> : std::true_type {};
template<> struct IsPrimitiveTag<MapErase> : std::true_type {};
template<> struct IsPrimitiveTag<MapSize> : std::true_type {};
template<> struct IsPrimitiveTag<Sqrt> : std::true_type {};
template<> struct IsPrimitiveTag<Exp> : std::true_type {};
template<> struct IsPrimitiveTag<Log> : std::true_type {};
template<> struct IsPrimitiveTag<Sin> : std::true_type {};
template<> struct IsPrimitiveTag<Cos> : std::true_type {};
template<> struct IsPrimitiveTag<Tan> : std::true_type {};
template<> struct IsPrimitiveTag<Asin> : std::true_type {};
template<> struct IsPrimitiveTag<Acos> : std::true_type {};
template<> struct IsPrimitiveTag<Atan> : std::true_type {};
template<> struct IsPrimitiveTag<VectorCtor> : std::true_type {};
template<> struct IsPrimitiveTag<MatrixCtor> : std::true_type {};
template<> struct IsPrimitiveTag<ComplexCtor> : std::true_type {};
template<> struct IsPrimitiveTag<VecAdd> : std::true_type {};
template<> struct IsPrimitiveTag<VecSub> : std::true_type {};
template<> struct IsPrimitiveTag<VecScale> : std::true_type {};
template<> struct IsPrimitiveTag<Dot> : std::true_type {};
template<> struct IsPrimitiveTag<Norm> : std::true_type {};
template<> struct IsPrimitiveTag<NormalizeVector> : std::true_type {};
template<> struct IsPrimitiveTag<MatAdd> : std::true_type {};
template<> struct IsPrimitiveTag<MatSub> : std::true_type {};
template<> struct IsPrimitiveTag<MatScale> : std::true_type {};
template<> struct IsPrimitiveTag<MatVecMul> : std::true_type {};
template<> struct IsPrimitiveTag<MatMul> : std::true_type {};
template<> struct IsPrimitiveTag<Transpose> : std::true_type {};
template<> struct IsPrimitiveTag<Determinant> : std::true_type {};
template<> struct IsPrimitiveTag<Inverse> : std::true_type {};
template<> struct IsPrimitiveTag<Conjugate> : std::true_type {};
template<> struct IsPrimitiveTag<NormSquared> : std::true_type {};
template<> struct IsPrimitiveTag<Magnitude> : std::true_type {};
template<> struct IsPrimitiveTag<Argument> : std::true_type {};
template<> struct IsPrimitiveTag<Mean> : std::true_type {};
template<> struct IsPrimitiveTag<Median> : std::true_type {};
template<> struct IsPrimitiveTag<Mode> : std::true_type {};
template<> struct IsPrimitiveTag<Variance> : std::true_type {};
template<> struct IsPrimitiveTag<StdDev> : std::true_type {};
template<> struct IsPrimitiveTag<Minimum> : std::true_type {};
template<> struct IsPrimitiveTag<Maximum> : std::true_type {};
template<> struct IsPrimitiveTag<StatRange> : std::true_type {};
template<> struct IsPrimitiveTag<Covariance> : std::true_type {};
template<> struct IsPrimitiveTag<Correlation> : std::true_type {};

template<typename T>
struct IsInferMarker : std::false_type {};

template<>
struct IsInferMarker<InferType> : std::true_type {};

template<typename T>
struct ValueStaticType {
    using type = UnknownType;
};

template<int N>
struct ValueStaticType<Nat<N>> {
    using type = NatType;
};

template<int N>
struct ValueStaticType<Int<N>> {
    using type = IntType;
};

template<auto Storage>
struct ValueStaticType<BigInt<Storage>> {
    using type = BigIntType;
};

template<bool B>
struct ValueStaticType<Bool<B>> {
    using type = BoolType;
};

template<typename Num, typename Den>
struct ValueStaticType<Rational<Num, Den>> {
    using type = RationalType;
};

template<auto Storage>
struct ValueStaticType<Decimal<Storage>> {
    using type = DecimalType;
};

template<typename Tag>
struct ValueStaticType<Irrational<Tag>> {
    using type = RealType;
};

template<typename Op, typename... Args>
struct ValueStaticType<RealExpr<Op, Args...>> {
    using type = RealType;
};

template<char... Chars>
struct ValueStaticType<String<Chars...>> {
    using type = StringType;
};

template<typename Real, typename Imag>
struct ValueStaticType<Complex<Real, Imag>> {
    using type = ComplexType<typename ValueStaticType<Real>::type>;
};

template<>
struct ValueStaticType<None> {
    using type = NoneType;
};

template<>
struct ValueStaticType<InferType> {
    using type = InferType;
};

template<typename Expected, typename... Items>
struct AllValueTypesMatch;

template<typename Expected>
struct AllValueTypesMatch<Expected> : std::true_type {};

template<typename Expected, typename Item, typename... Rest>
struct AllValueTypesMatch<Expected, Item, Rest...>
    : std::bool_constant<
          IsSame<Expected, typename ValueStaticType<Item>::type>::value &&
          AllValueTypesMatch<Expected, Rest...>::value> {};

template<typename... Items>
struct InferListType {
    using type = ListType<AnyType>;
};

template<typename First, typename... Rest>
struct InferListType<First, Rest...> {
private:
    using elem_type = typename ValueStaticType<First>::type;

public:
    using type = IfType_t<
        AllValueTypesMatch<elem_type, Rest...>::value,
        ListType<elem_type>,
        TypeError<msg_heterogeneous_list>
    >;
};

template<typename... Items>
struct ValueStaticType<List<Items...>> {
    using type = typename InferListType<Items...>::type;
};

template<typename... Items>
struct InferVectorType {
    using type = VectorType<AnyType, sizeof...(Items)>;
};

template<typename First, typename... Rest>
struct InferVectorType<First, Rest...> {
private:
    using elem_type = typename ValueStaticType<First>::type;

public:
    using type = IfType_t<
        AllValueTypesMatch<elem_type, Rest...>::value,
        VectorType<elem_type, 1 + sizeof...(Rest)>,
        TypeError<msg_heterogeneous_list>
    >;
};

template<typename... Items>
struct ValueStaticType<Vector<Items...>> {
    using type = typename InferVectorType<Items...>::type;
};

template<typename... Items>
struct InferSetType {
    using type = SetType<AnyType>;
};

template<typename First, typename... Rest>
struct InferSetType<First, Rest...> {
private:
    using elem_type = typename ValueStaticType<First>::type;

public:
    using type = IfType_t<
        AllValueTypesMatch<elem_type, Rest...>::value,
        SetType<elem_type>,
        TypeError<msg_heterogeneous_set>
    >;
};

template<typename... Items>
struct ValueStaticType<Set<Items...>> {
    using type = typename InferSetType<Items...>::type;
};

template<typename EntryT>
struct EntryStaticTypes;

template<typename Key, typename Value>
struct EntryStaticTypes<Entry<Key, Value>> {
    using key_type = typename ValueStaticType<Key>::type;
    using value_type = typename ValueStaticType<Value>::type;
};

template<typename KeyType, typename ValueType, typename... Entries>
struct AllEntryTypesMatch;

template<typename KeyType, typename ValueType>
struct AllEntryTypesMatch<KeyType, ValueType> : std::true_type {};

template<typename KeyType, typename ValueType, typename EntryT, typename... Rest>
struct AllEntryTypesMatch<KeyType, ValueType, EntryT, Rest...>
    : std::bool_constant<
          IsSame<KeyType, typename EntryStaticTypes<EntryT>::key_type>::value &&
          IsSame<ValueType, typename EntryStaticTypes<EntryT>::value_type>::value &&
          AllEntryTypesMatch<KeyType, ValueType, Rest...>::value> {};

template<typename... Entries>
struct InferMapType {
    using type = MapType<AnyType, AnyType>;
};

template<typename FirstEntry, typename... RestEntries>
struct InferMapType<FirstEntry, RestEntries...> {
private:
    using key_type = typename EntryStaticTypes<FirstEntry>::key_type;
    using value_type = typename EntryStaticTypes<FirstEntry>::value_type;

public:
    using type = IfType_t<
        AllEntryTypesMatch<key_type, value_type, RestEntries...>::value,
        MapType<key_type, value_type>,
        TypeError<msg_heterogeneous_map>
    >;
};

template<typename... Entries>
struct ValueStaticType<AssocMap<Entries...>> {
    using type = typename InferMapType<Entries...>::type;
};

template<typename Row>
struct MatrixRowType;

template<typename Elem, std::size_t Cols>
struct MatrixRowType<VectorType<Elem, Cols>> {
    using elem = Elem;
    static constexpr std::size_t cols = Cols;
};

template<typename... Rows>
struct InferMatrixType {
    using type = MatrixType<AnyType, sizeof...(Rows), 0>;
};

template<typename FirstRow, typename... RestRows>
struct InferMatrixType<FirstRow, RestRows...> {
private:
    using first_type = typename ValueStaticType<FirstRow>::type;
    using elem_type = typename MatrixRowType<first_type>::elem;
    static constexpr std::size_t cols = MatrixRowType<first_type>::cols;

    template<typename... Remaining>
    struct RowsMatch : std::true_type {};

    template<typename Current, typename... Remaining>
    struct RowsMatch<Current, Remaining...>
        : std::bool_constant<
              IsSame<typename ValueStaticType<Current>::type, VectorType<elem_type, cols>>::value &&
              RowsMatch<Remaining...>::value> {};

public:
    using type = IfType_t<
        RowsMatch<RestRows...>::value,
        MatrixType<elem_type, 1 + sizeof...(RestRows), cols>,
        TypeError<msg_heterogeneous_list>
    >;
};

template<typename... Rows>
struct ValueStaticType<Matrix<Rows...>> {
    using type = typename InferMatrixType<Rows...>::type;
};

template<typename T>
using ValueStaticType_t = typename ValueStaticType<T>::type;

template<typename T, bool Primitive = IsPrimitiveTag<T>::value>
struct DefaultExprTypeImpl {
    using type = ValueStaticType_t<T>;
};

template<typename T>
struct DefaultExprTypeImpl<T, true> {
    using type = PrimitiveType<T>;
};

template<typename T>
struct DefaultExprType {
    using type = typename DefaultExprTypeImpl<T>::type;
};

template<typename ParamsT, typename Body, typename Env>
struct DefaultExprType<Closure<ParamsT, Body, Env>> {
    using type = FunctionType<ParamsT, UnknownType>;
};

template<typename Name, typename ParamsT, typename Body, typename Env>
struct DefaultExprType<RecursiveClosure<Name, ParamsT, Body, Env>> {
    using type = FunctionType<ParamsT, UnknownType>;
};

template<typename Name, typename Env>
struct EnvLookup {
private:
    using found = detail::AssocFind_t<Name, Env>;

public:
    using type = IfType_t<IsSame<found, None>::value, EvalError<msg_unbound_symbol>, found>;
};

template<typename Name, typename Env>
using EnvLookup_t = typename EnvLookup<Name, Env>::type;

template<typename Name, typename TypeEnv>
struct TypeEnvLookup {
private:
    using found = detail::AssocFind_t<Name, TypeEnv>;

public:
    using type = IfType_t<IsSame<found, None>::value, TypeError<msg_unbound_symbol>, found>;
};

template<typename Name, typename TypeEnv>
using TypeEnvLookup_t = typename TypeEnvLookup<Name, TypeEnv>::type;

template<typename T>
struct ContainsInferType : std::false_type {};

template<>
struct ContainsInferType<InferType> : std::true_type {};

template<typename Elem>
struct ContainsInferType<ListType<Elem>> : ContainsInferType<Elem> {};

template<typename Elem>
struct ContainsInferType<SetType<Elem>> : ContainsInferType<Elem> {};

template<typename Key, typename Value>
struct ContainsInferType<MapType<Key, Value>>
    : std::bool_constant<ContainsInferType<Key>::value || ContainsInferType<Value>::value> {};

template<typename Name, typename Type>
struct ContainsInferType<Param<Name, Type>> : ContainsInferType<Type> {};

template<typename... ParamsT>
struct ContainsInferType<Params<ParamsT...>>
    : std::bool_constant<(ContainsInferType<ParamsT>::value || ...)> {};

template<typename ParamsT, typename ReturnType>
struct ContainsInferType<FunctionType<ParamsT, ReturnType>>
    : std::bool_constant<ContainsInferType<ParamsT>::value || ContainsInferType<ReturnType>::value> {};

template<typename T>
struct FinalizeInferredType {
    using type = IfType_t<ContainsInferType<T>::value, TypeError<msg_could_not_infer>, T>;
};

template<typename T>
using FinalizeInferredType_t = typename FinalizeInferredType<T>::type;

template<typename Expected, typename Actual>
struct TypeMatches
    : std::bool_constant<
          IsSame<Expected, Actual>::value ||
          IsSame<Expected, AnyType>::value ||
          IsSame<Expected, InferType>::value ||
          IsSame<Actual, InferType>::value> {};

template<typename Name, typename ParamsT>
struct ParamsContainName : std::false_type {};

template<typename Name>
struct ParamsContainName<Name, Params<>> : std::false_type {};

template<typename Name, typename Type, typename... RestParams>
struct ParamsContainName<Name, Params<Param<Name, Type>, RestParams...>> : std::true_type {};

template<typename Name, typename OtherName, typename Type, typename... RestParams>
struct ParamsContainName<Name, Params<Param<OtherName, Type>, RestParams...>>
    : ParamsContainName<Name, Params<RestParams...>> {};

template<typename Name, typename Expr>
struct ContainsRef : std::false_type {};

template<typename Name>
struct ContainsRef<Name, Ref<Name>> : std::true_type {};

template<typename Name, typename OtherName>
struct ContainsRef<Name, Ref<OtherName>> : std::false_type {};

template<typename Name, typename Value>
struct ContainsRef<Name, Quote<Value>> : std::false_type {};

template<typename Name, typename Cond, typename ThenExpr, typename ElseExpr>
struct ContainsRef<Name, IfExpr<Cond, ThenExpr, ElseExpr>>
    : std::bool_constant<
          ContainsRef<Name, Cond>::value ||
          ContainsRef<Name, ThenExpr>::value ||
          ContainsRef<Name, ElseExpr>::value> {};

template<typename Name, typename ParamsT, typename Body>
struct ContainsRef<Name, LambdaExpr<ParamsT, Body>>
    : std::bool_constant<!ParamsContainName<Name, ParamsT>::value && ContainsRef<Name, Body>::value> {};

template<typename Name, typename FnExpr, typename... ArgExprs>
struct ContainsRef<Name, CallExpr<FnExpr, ArgExprs...>>
    : std::bool_constant<ContainsRef<Name, FnExpr>::value || (false || ... || ContainsRef<Name, ArgExprs>::value)> {};

template<typename Name, typename BindingT>
struct BindingContainsRef;

template<typename Name, typename BindingName, typename Expr>
struct BindingContainsRef<Name, Binding<BindingName, Expr>> : ContainsRef<Name, Expr> {};

template<typename Name, typename... BindingsT>
struct BindingsContainName : std::false_type {};

template<typename Name>
struct BindingsContainName<Name, Bindings<>> : std::false_type {};

template<typename Name, typename Expr, typename... RestBindings>
struct BindingsContainName<Name, Bindings<Binding<Name, Expr>, RestBindings...>> : std::true_type {};

template<typename Name, typename BindingName, typename Expr, typename... RestBindings>
struct BindingsContainName<Name, Bindings<Binding<BindingName, Expr>, RestBindings...>>
    : BindingsContainName<Name, Bindings<RestBindings...>> {};

template<typename Name, typename... BindingsT>
struct ContainsRef<Name, Bindings<BindingsT...>>
    : std::bool_constant<(false || ... || BindingContainsRef<Name, BindingsT>::value)> {};

template<typename Name, typename BindingsT, typename Body>
struct ContainsRef<Name, LetExpr<BindingsT, Body>>
    : std::bool_constant<
          ContainsRef<Name, BindingsT>::value ||
          (!BindingsContainName<Name, BindingsT>::value && ContainsRef<Name, Body>::value)> {};

template<typename Name, typename... Exprs>
struct ContainsRef<Name, BeginExpr<Exprs...>>
    : std::bool_constant<(false || ... || ContainsRef<Name, Exprs>::value)> {};

template<typename ParamsT, typename ArgTypes>
struct BindInferredParams {
    using type = ParamsT;
};

template<>
struct BindInferredParams<Params<>, TypePack<>> {
    using type = Params<>;
};

template<typename Name, typename ParamType, typename... RestParams, typename ArgType, typename... RestArgs>
struct BindInferredParams<Params<Param<Name, ParamType>, RestParams...>, TypePack<ArgType, RestArgs...>> {
private:
    using bound_type = IfType_t<IsSame<ParamType, InferType>::value, ArgType, ParamType>;
    using rest = typename BindInferredParams<Params<RestParams...>, TypePack<RestArgs...>>::type;

    template<typename FirstParam, typename RestParamsT>
    struct Combine;

    template<typename FirstParam, typename... RestParamsT>
    struct Combine<FirstParam, Params<RestParamsT...>> {
        using type = Params<FirstParam, RestParamsT...>;
    };

public:
    using type = typename Combine<Param<Name, bound_type>, rest>::type;
};

template<typename ParamsT, typename ArgTypes>
using BindInferredParams_t = typename BindInferredParams<ParamsT, ArgTypes>::type;

template<typename Expr, typename Env>
struct EvalLisp {
    using type = Expr;
};

template<typename Expr, typename Env = AssocMap<>>
using EvalLisp_t = typename EvalLisp<Expr, Env>::type;

template<typename Expr>
struct OptimizeLisp {
    using type = Expr;
};

template<typename Expr>
using OptimizeLisp_t = typename OptimizeLisp<Expr>::type;

template<typename Expr>
struct TermNodeCount<ExprForm<Expr>> : std::integral_constant<int, 1 + TermNodeCount<Expr>::value> {};

template<typename Name, typename Expr>
struct TermNodeCount<DefineForm<Name, Expr>> : std::integral_constant<int, 1 + TermNodeCount<Name>::value + TermNodeCount<Expr>::value> {};

template<typename... Forms>
struct TermNodeCount<ProgramExpr<Forms...>> : std::integral_constant<int, 1 + (0 + ... + TermNodeCount<Forms>::value)> {};

template<typename Name, typename Type>
struct TermNodeCount<Param<Name, Type>> : std::integral_constant<int, 1 + TermNodeCount<Name>::value + TermNodeCount<Type>::value> {};

template<typename... ParamsT>
struct TermNodeCount<Params<ParamsT...>> : std::integral_constant<int, 1 + (0 + ... + TermNodeCount<ParamsT>::value)> {};

template<typename ParamsT, typename Body>
struct TermNodeCount<LambdaExpr<ParamsT, Body>> : std::integral_constant<int, 1 + TermNodeCount<ParamsT>::value + TermNodeCount<Body>::value> {};

template<typename FnExpr, typename... ArgExprs>
struct TermNodeCount<CallExpr<FnExpr, ArgExprs...>>
    : std::integral_constant<int, 1 + TermNodeCount<FnExpr>::value + (0 + ... + TermNodeCount<ArgExprs>::value)> {};

template<typename Cond, typename ThenExpr, typename ElseExpr>
struct TermNodeCount<IfExpr<Cond, ThenExpr, ElseExpr>>
    : std::integral_constant<int, 1 + TermNodeCount<Cond>::value + TermNodeCount<ThenExpr>::value + TermNodeCount<ElseExpr>::value> {};

template<typename Name, typename Expr>
struct TermNodeCount<Binding<Name, Expr>> : std::integral_constant<int, 1 + TermNodeCount<Name>::value + TermNodeCount<Expr>::value> {};

template<typename... BindingsT>
struct TermNodeCount<Bindings<BindingsT...>> : std::integral_constant<int, 1 + (0 + ... + TermNodeCount<BindingsT>::value)> {};

template<typename BindingsT, typename Body>
struct TermNodeCount<LetExpr<BindingsT, Body>> : std::integral_constant<int, 1 + TermNodeCount<BindingsT>::value + TermNodeCount<Body>::value> {};

template<typename... Exprs>
struct TermNodeCount<BeginExpr<Exprs...>> : std::integral_constant<int, 1 + (0 + ... + TermNodeCount<Exprs>::value)> {};

template<typename ParamsT, typename Body, typename Env>
struct TermNodeCount<Closure<ParamsT, Body, Env>> : std::integral_constant<int, 1 + TermNodeCount<ParamsT>::value + TermNodeCount<Body>::value> {};

template<typename Name, typename ParamsT, typename Body, typename Env>
struct TermNodeCount<RecursiveClosure<Name, ParamsT, Body, Env>>
    : std::integral_constant<int, 1 + TermNodeCount<Name>::value + TermNodeCount<ParamsT>::value + TermNodeCount<Body>::value> {};

template<typename Frame, typename Inner>
struct TermNodeCount<ErrorContext<Frame, Inner>> : std::integral_constant<int, 1 + TermNodeCount<Frame>::value + TermNodeCount<Inner>::value> {};

template<typename Label, typename Detail>
struct TermNodeCount<ErrorFrame<Label, Detail>> : std::integral_constant<int, 1 + TermNodeCount<Label>::value + TermNodeCount<Detail>::value> {};

template<typename T>
struct IsImmediateValue : std::false_type {};

template<int N>
struct IsImmediateValue<Nat<N>> : std::true_type {};

template<int N>
struct IsImmediateValue<Int<N>> : std::true_type {};

template<auto Storage>
struct IsImmediateValue<BigInt<Storage>> : std::true_type {};

template<typename Num, typename Den>
struct IsImmediateValue<Rational<Num, Den>> : std::true_type {};

template<auto Storage>
struct IsImmediateValue<Decimal<Storage>> : std::true_type {};

template<typename Tag>
struct IsImmediateValue<Irrational<Tag>> : std::true_type {};

template<typename Op, typename... Args>
struct IsImmediateValue<RealExpr<Op, Args...>> : std::true_type {};

template<typename Real, typename Imag>
struct IsImmediateValue<Complex<Real, Imag>> : std::true_type {};

template<bool B>
struct IsImmediateValue<Bool<B>> : std::true_type {};

template<char... Chars>
struct IsImmediateValue<String<Chars...>> : std::true_type {};

template<typename... Items>
struct IsImmediateValue<List<Items...>> : std::true_type {};

template<typename... Items>
struct IsImmediateValue<Set<Items...>> : std::true_type {};

template<typename... Entries>
struct IsImmediateValue<AssocMap<Entries...>> : std::true_type {};

template<typename... Elems>
struct IsImmediateValue<Vector<Elems...>> : std::true_type {};

template<typename... Rows>
struct IsImmediateValue<Matrix<Rows...>> : std::true_type {};

template<>
struct IsImmediateValue<None> : std::true_type {};

template<typename... Ts>
struct ExprPack {};

template<typename Pack, typename Expr>
struct ExprPackAppend;

template<typename... Done, typename Expr>
struct ExprPackAppend<ExprPack<Done...>, Expr> {
    using type = ExprPack<Done..., Expr>;
};

template<typename Pack, typename Expr>
using ExprPackAppend_t = typename ExprPackAppend<Pack, Expr>::type;

template<typename Pack>
struct BeginToExpr;

template<>
struct BeginToExpr<ExprPack<>> {
    using type = BeginExpr<>;
};

template<typename Expr>
struct BeginToExpr<ExprPack<Expr>> {
    using type = Expr;
};

template<typename Expr, typename Next, typename... Rest>
struct BeginToExpr<ExprPack<Expr, Next, Rest...>> {
    using type = BeginExpr<Expr, Next, Rest...>;
};

template<typename Pack, typename Expr>
struct FlattenBeginExpr {
    using type = ExprPackAppend_t<Pack, Expr>;
};

template<typename... Done, typename... BeginExprs>
struct FlattenBeginExpr<ExprPack<Done...>, BeginExpr<BeginExprs...>> {
    using type = ExprPack<Done..., BeginExprs...>;
};

template<typename Pack, typename... Exprs>
struct OptimizeBeginPack;

template<typename Pack>
struct OptimizeBeginPack<Pack> {
    using type = Pack;
};

template<typename Pack, typename Expr, typename... Rest>
struct OptimizeBeginPack<Pack, Expr, Rest...> {
private:
    using optimized = OptimizeLisp_t<Expr>;
    using flattened = typename FlattenBeginExpr<Pack, optimized>::type;

public:
    using type = typename OptimizeBeginPack<flattened, Rest...>::type;
};

template<typename... Exprs>
struct OptimizeLisp<BeginExpr<Exprs...>> {
private:
    using flat = typename OptimizeBeginPack<ExprPack<>, Exprs...>::type;

public:
    using type = typename BeginToExpr<flat>::type;
};

template<typename Cond, typename ThenExpr, typename ElseExpr>
struct OptimizeLisp<IfExpr<Cond, ThenExpr, ElseExpr>> {
private:
    using cond = OptimizeLisp_t<Cond>;
    using then_expr = OptimizeLisp_t<ThenExpr>;
    using else_expr = OptimizeLisp_t<ElseExpr>;

public:
    using type = IfType_t<
        IsSame<cond, Bool<true>>::value,
        then_expr,
        IfType_t<
            IsSame<cond, Bool<false>>::value,
            else_expr,
            IfExpr<cond, then_expr, else_expr>
        >
    >;
};

template<typename ParamsT, typename Body>
struct OptimizeLisp<LambdaExpr<ParamsT, Body>> {
    using type = LambdaExpr<ParamsT, OptimizeLisp_t<Body>>;
};

template<typename Name, typename Expr>
struct OptimizeLisp<Binding<Name, Expr>> {
    using type = Binding<Name, OptimizeLisp_t<Expr>>;
};

template<typename... BindingsT>
struct OptimizeLisp<Bindings<BindingsT...>> {
    using type = Bindings<OptimizeLisp_t<BindingsT>...>;
};

template<typename BindingsT, typename Body>
struct OptimizeLisp<LetExpr<BindingsT, Body>> {
    using type = LetExpr<OptimizeLisp_t<BindingsT>, OptimizeLisp_t<Body>>;
};

template<typename Name, typename Expr>
struct OptimizeLisp<DefineForm<Name, Expr>> {
    using type = DefineForm<Name, OptimizeLisp_t<Expr>>;
};

template<typename Expr>
struct OptimizeLisp<ExprForm<Expr>> {
    using type = ExprForm<OptimizeLisp_t<Expr>>;
};

template<typename... Forms>
struct OptimizeLisp<ProgramExpr<Forms...>> {
    using type = ProgramExpr<OptimizeLisp_t<Forms>...>;
};

template<typename FnExpr, typename... ArgExprs>
struct OptimizeLisp<CallExpr<FnExpr, ArgExprs...>> {
private:
    using fn = OptimizeLisp_t<FnExpr>;
    using rebuilt = CallExpr<fn, OptimizeLisp_t<ArgExprs>...>;

public:
    using type = IfType_t<
        IsPrimitiveTag<fn>::value && (IsImmediateValue<OptimizeLisp_t<ArgExprs>>::value && ...),
        Normalize_t<Apply_t<fn, OptimizeLisp_t<ArgExprs>...>, 4096>,
        rebuilt
    >;
};

template<typename CondValue, bool Error, typename Env, typename ThenExpr, typename ElseExpr>
struct EvalIfDispatch;

template<typename CondValue, typename Env, typename ThenExpr, typename ElseExpr>
struct EvalIfDispatch<CondValue, true, Env, ThenExpr, ElseExpr> {
    using type = CondValue;
};

template<typename Env, typename ThenExpr, typename ElseExpr>
struct EvalIfDispatch<Bool<true>, false, Env, ThenExpr, ElseExpr> {
    using type = typename EvalLisp<ThenExpr, Env>::type;
};

template<typename Env, typename ThenExpr, typename ElseExpr>
struct EvalIfDispatch<Bool<false>, false, Env, ThenExpr, ElseExpr> {
    using type = typename EvalLisp<ElseExpr, Env>::type;
};

template<typename CondValue, typename Env, typename ThenExpr, typename ElseExpr>
struct EvalIfDispatch<CondValue, false, Env, ThenExpr, ElseExpr> {
    using type = EvalError<msg_invalid_condition>;
};

template<typename Env, typename DonePack, typename... Exprs>
struct EvalArgsImpl;

template<typename Env, typename... Done>
struct EvalArgsImpl<Env, ValuePack<Done...>> {
    using type = ValuePack<Done...>;
};

template<bool Error, typename Env, typename DonePack, typename CurrentValue, typename... RestExprs>
struct EvalArgsNext;

template<typename Env, typename... Done, typename CurrentValue, typename... RestExprs>
struct EvalArgsNext<true, Env, ValuePack<Done...>, CurrentValue, RestExprs...> {
    using type = CurrentValue;
};

template<typename Env, typename... Done, typename CurrentValue, typename... RestExprs>
struct EvalArgsNext<false, Env, ValuePack<Done...>, CurrentValue, RestExprs...> {
    using type = typename EvalArgsImpl<Env, ValuePack<Done..., CurrentValue>, RestExprs...>::type;
};

template<typename Env, typename... Done, typename Expr, typename... RestExprs>
struct EvalArgsImpl<Env, ValuePack<Done...>, Expr, RestExprs...> {
private:
    using current = typename EvalLisp<Expr, Env>::type;

public:
    using type = typename EvalArgsNext<
        IsEvalError<current>::value,
        Env,
        ValuePack<Done...>,
        current,
        RestExprs...
    >::type;
};

template<typename ParamsT, typename ArgsPack, typename Env>
struct BindValues {
    using type = EvalError<msg_wrong_arity>;
};

template<typename Env>
struct BindValues<Params<>, ValuePack<>, Env> {
    using type = Env;
};

template<typename Name, typename Type, typename... RestParams, typename Arg, typename... RestArgs, typename Env>
struct BindValues<Params<Param<Name, Type>, RestParams...>, ValuePack<Arg, RestArgs...>, Env> {
    using type = typename BindValues<
        Params<RestParams...>,
        ValuePack<RestArgs...>,
        detail::AssocInsert_t<Name, Arg, Env>
    >::type;
};

template<typename FnValue, typename ArgsPack>
struct ApplyLisp {
private:
    using attempted = Normalize_t<Apply_t<FnValue>, 4096>;

public:
    using type = attempted;
};

template<typename FnValue, typename... Args>
struct ApplyLisp<FnValue, ValuePack<Args...>> {
private:
    using attempted_term = Apply_t<FnValue, Args...>;
    using attempted = Normalize_t<attempted_term, 4096>;

public:
    using type = IfType_t<
        IsSame<attempted, attempted_term>::value,
        EvalError<msg_invalid_application>,
        attempted
    >;
};

template<typename ParamsT, typename Body, typename CapturedEnv, typename... Args>
struct ApplyLisp<Closure<ParamsT, Body, CapturedEnv>, ValuePack<Args...>> {
private:
    using bound_env = typename BindValues<ParamsT, ValuePack<Args...>, CapturedEnv>::type;

public:
    using type = IfType_t<
        IsEvalError<bound_env>::value,
        bound_env,
        typename EvalLisp<Body, bound_env>::type
    >;
};

template<typename Name, typename ParamsT, typename Body, typename CapturedEnv, typename... Args>
struct ApplyLisp<RecursiveClosure<Name, ParamsT, Body, CapturedEnv>, ValuePack<Args...>> {
private:
    using self_env = detail::AssocInsert_t<Name, RecursiveClosure<Name, ParamsT, Body, CapturedEnv>, CapturedEnv>;
    using bound_env = typename BindValues<ParamsT, ValuePack<Args...>, self_env>::type;

public:
    using type = IfType_t<
        IsEvalError<bound_env>::value,
        bound_env,
        typename EvalLisp<Body, bound_env>::type
    >;
};

template<typename Form, typename Env>
struct EvalProgramForm;

template<typename Expr, typename Env>
struct EvalProgramForm<ExprForm<Expr>, Env> {
    using value = typename EvalLisp<Expr, Env>::type;
    using next_env = Env;
};

template<typename Name, typename Expr, typename Env>
struct EvalProgramForm<DefineForm<Name, Expr>, Env> {
private:
    using recursive = std::bool_constant<ContainsRef<Name, Expr>::value>;
    using value_or_error = typename EvalLisp<Expr, Env>::type;

public:
    using value = IfType_t<
        recursive::value,
        EvalError<msg_recursive_value_define>,
        value_or_error
    >;
    using next_env = IfType_t<
        IsEvalError<value>::value,
        Env,
        detail::AssocInsert_t<Name, value, Env>
    >;
};

template<typename Name, typename ParamsT, typename Body, typename Env>
struct EvalProgramForm<DefineForm<Name, LambdaExpr<ParamsT, Body>>, Env> {
    using value = RecursiveClosure<Name, ParamsT, Body, Env>;
    using next_env = detail::AssocInsert_t<Name, value, Env>;
};

template<typename Env, typename LastValue, typename... Forms>
struct EvalProgramImpl;

template<typename Env, typename LastValue>
struct EvalProgramImpl<Env, LastValue> {
    using type = LastValue;
};

template<typename Env, typename LastValue, typename Form, typename... RestForms>
struct EvalProgramImpl<Env, LastValue, Form, RestForms...> {
private:
    using step = EvalProgramForm<Form, Env>;

public:
    using type = IfType_t<
        IsEvalError<typename step::value>::value,
        typename step::value,
        typename EvalProgramImpl<typename step::next_env, typename step::value, RestForms...>::type
    >;
};

template<typename Name, typename Env>
struct EvalLisp<Ref<Name>, Env> {
    using type = EnvLookup_t<Name, Env>;
};

template<typename Value, typename Env>
struct EvalLisp<Quote<Value>, Env> {
    using type = Value;
};

template<typename Cond, typename ThenExpr, typename ElseExpr, typename Env>
struct EvalLisp<IfExpr<Cond, ThenExpr, ElseExpr>, Env> {
private:
    using cond_value = typename EvalLisp<Cond, Env>::type;

public:
    using type = typename EvalIfDispatch<
        cond_value,
        IsEvalError<cond_value>::value,
        Env,
        ThenExpr,
        ElseExpr
    >::type;
};

template<typename ParamsT, typename Body, typename Env>
struct EvalLisp<LambdaExpr<ParamsT, Body>, Env> {
    using type = Closure<ParamsT, Body, Env>;
};

template<typename BindingsT, typename Env>
struct EvalBindings;

template<typename Env, typename DonePack, typename... BindingsT>
struct EvalBindingsImpl;

template<typename Env, typename... Done>
struct EvalBindingsImpl<Env, TypeList<Done...>> {
    using type = Env;
};

template<bool Error, typename Env, typename DonePack, typename Value, typename BindingT, typename... RestBindings>
struct EvalBindingsNext;

template<typename Env, typename... Done, typename Value, typename BindingT, typename... RestBindings>
struct EvalBindingsNext<true, Env, TypeList<Done...>, Value, BindingT, RestBindings...> {
    using type = Value;
};

template<typename Env, typename... Done, typename Value, typename Name, typename Expr, typename... RestBindings>
struct EvalBindingsNext<false, Env, TypeList<Done...>, Value, Binding<Name, Expr>, RestBindings...> {
    using type = typename EvalBindingsImpl<
        detail::AssocInsert_t<Name, Value, Env>,
        TypeList<Done..., Binding<Name, Expr>>,
        RestBindings...
    >::type;
};

template<typename Env, typename... Done, typename Name, typename Expr, typename... RestBindings>
struct EvalBindingsImpl<Env, TypeList<Done...>, Binding<Name, Expr>, RestBindings...> {
private:
    using value = typename EvalLisp<Expr, Env>::type;

public:
    using type = typename EvalBindingsNext<
        IsEvalError<value>::value,
        Env,
        TypeList<Done...>,
        value,
        Binding<Name, Expr>,
        RestBindings...
    >::type;
};

template<typename Env, typename... BindingsT>
struct EvalBindings<Bindings<BindingsT...>, Env> {
    using type = typename EvalBindingsImpl<Env, TypeList<>, BindingsT...>::type;
};

template<typename BindingsT, typename Body, typename Env>
struct EvalLisp<LetExpr<BindingsT, Body>, Env> {
private:
    using next_env = typename EvalBindings<BindingsT, Env>::type;

public:
    using type = IfType_t<
        IsEvalError<next_env>::value,
        next_env,
        typename EvalLisp<Body, next_env>::type
    >;
};

template<typename Env, typename Expr>
struct EvalBegin {
    using type = typename EvalLisp<Expr, Env>::type;
};

template<typename Env, typename Expr, typename NextExpr, typename... RestExprs>
struct EvalBegin<Env, BeginExpr<Expr, NextExpr, RestExprs...>> {
private:
    using current = typename EvalLisp<Expr, Env>::type;

public:
    using type = IfType_t<
        IsEvalError<current>::value,
        current,
        typename EvalBegin<Env, BeginExpr<NextExpr, RestExprs...>>::type
    >;
};

template<typename Expr, typename Env>
struct EvalLisp<BeginExpr<Expr>, Env> {
    using type = typename EvalLisp<Expr, Env>::type;
};

template<typename Expr, typename NextExpr, typename... RestExprs, typename Env>
struct EvalLisp<BeginExpr<Expr, NextExpr, RestExprs...>, Env> {
    using type = typename EvalBegin<Env, BeginExpr<Expr, NextExpr, RestExprs...>>::type;
};

template<typename... Forms, typename Env>
struct EvalLisp<ProgramExpr<Forms...>, Env> {
    using type = typename EvalProgramImpl<Env, None, Forms...>::type;
};

template<bool FnError, bool ArgsError, typename FnValue, typename ArgValues>
struct EvalCallDispatch;

template<bool ArgsError, typename FnValue, typename ArgValues>
struct EvalCallDispatch<true, ArgsError, FnValue, ArgValues> {
    using type = FnValue;
};

template<typename FnValue, typename ArgValues>
struct EvalCallDispatch<false, true, FnValue, ArgValues> {
    using type = ArgValues;
};

template<typename FnValue, typename ArgValues>
struct EvalCallDispatch<false, false, FnValue, ArgValues> {
    using type = typename ApplyLisp<FnValue, ArgValues>::type;
};

template<typename FnExpr, typename... ArgExprs, typename Env>
struct EvalLisp<CallExpr<FnExpr, ArgExprs...>, Env> {
private:
    using fn_value = typename EvalLisp<FnExpr, Env>::type;
    using arg_values = typename EvalArgsImpl<Env, ValuePack<>, ArgExprs...>::type;

public:
    using type = typename EvalCallDispatch<
        IsEvalError<fn_value>::value,
        IsEvalError<arg_values>::value,
        fn_value,
        arg_values
    >::type;
};

template<typename Expr, typename TypeEnv = AssocMap<>>
struct TypeCheck {
    using type = typename DefaultExprType<Expr>::type;
};

template<typename Expr, typename TypeEnv = AssocMap<>>
using TypeCheck_t = typename TypeCheck<Expr, TypeEnv>::type;

template<typename TypeEnv, typename DonePack, typename... Exprs>
struct TypeArgsImpl;

template<typename TypeEnv, typename... Done>
struct TypeArgsImpl<TypeEnv, TypePack<Done...>> {
    using type = TypePack<Done...>;
};

template<bool Error, typename TypeEnv, typename DonePack, typename CurrentType, typename... RestExprs>
struct TypeArgsNext;

template<typename TypeEnv, typename... Done, typename CurrentType, typename... RestExprs>
struct TypeArgsNext<true, TypeEnv, TypePack<Done...>, CurrentType, RestExprs...> {
    using type = CurrentType;
};

template<typename TypeEnv, typename... Done, typename CurrentType, typename... RestExprs>
struct TypeArgsNext<false, TypeEnv, TypePack<Done...>, CurrentType, RestExprs...> {
    using type = typename TypeArgsImpl<TypeEnv, TypePack<Done..., CurrentType>, RestExprs...>::type;
};

template<typename TypeEnv, typename... Done, typename Expr, typename... RestExprs>
struct TypeArgsImpl<TypeEnv, TypePack<Done...>, Expr, RestExprs...> {
private:
    using current = typename TypeCheck<Expr, TypeEnv>::type;

public:
    using type = typename TypeArgsNext<
        IsTypeError<current>::value,
        TypeEnv,
        TypePack<Done...>,
        current,
        RestExprs...
    >::type;
};

template<typename ParamsT, typename TypeEnv>
struct ExtendTypeEnvWithParams;

template<typename TypeEnv>
struct ExtendTypeEnvWithParams<Params<>, TypeEnv> {
    using type = TypeEnv;
};

template<typename Name, typename Type, typename... RestParams, typename TypeEnv>
struct ExtendTypeEnvWithParams<Params<Param<Name, Type>, RestParams...>, TypeEnv> {
    using type = typename ExtendTypeEnvWithParams<
        Params<RestParams...>,
        detail::AssocInsert_t<Name, Type, TypeEnv>
    >::type;
};

template<typename FnType, typename ArgTypes>
struct TypeApply {
    using type = TypeError<msg_call_type_error>;
};

template<typename T>
struct IsIntegerNumericType : std::false_type {};

template<>
struct IsIntegerNumericType<NatType> : std::true_type {};

template<>
struct IsIntegerNumericType<IntType> : std::true_type {};

template<>
struct IsIntegerNumericType<BigIntType> : std::true_type {};

template<typename T>
struct IsNumericType : IsIntegerNumericType<T> {};

template<>
struct IsNumericType<RationalType> : std::true_type {};

template<>
struct IsNumericType<DecimalType> : std::true_type {};

template<>
struct IsNumericType<RealType> : std::true_type {};

template<typename Left, typename Right>
struct AdditiveNumericType {
    using type = IfType_t<
        IsSame<Left, RealType>::value || IsSame<Right, RealType>::value,
        RealType,
        IfType_t<
            IsSame<Left, DecimalType>::value || IsSame<Right, DecimalType>::value,
            DecimalType,
            IfType_t<
                IsSame<Left, RationalType>::value || IsSame<Right, RationalType>::value,
                RationalType,
                IfType_t<
                    IsSame<Left, BigIntType>::value || IsSame<Right, BigIntType>::value,
                    BigIntType,
                    IfType_t<
                        IsSame<Left, IntType>::value || IsSame<Right, IntType>::value,
                        IntType,
                        NatType
                    >
                >
            >
        >
    >;
};

template<typename Left, typename Right>
using AdditiveNumericType_t = typename AdditiveNumericType<Left, Right>::type;

template<typename Left, typename Right>
struct ComparableNumericTypes
    : std::bool_constant<IsNumericType<Left>::value && IsNumericType<Right>::value> {};

template<typename Left, typename Right>
struct CombineValueTypes {
    using type = IfType_t<
        IsSame<Left, InferType>::value,
        Right,
        IfType_t<
            IsSame<Right, InferType>::value,
            Left,
            IfType_t<
                IsSame<Left, Right>::value,
                Left,
                IfType_t<
                    IsNumericType<Left>::value && IsNumericType<Right>::value,
                    AdditiveNumericType_t<Left, Right>,
                    TypeError<msg_call_type_error>
                >
            >
        >
    >;
};

template<typename Left, typename Right>
using CombineValueTypes_t = typename CombineValueTypes<Left, Right>::type;

template<typename... Ts>
struct CommonValueType;

template<>
struct CommonValueType<> {
    using type = AnyType;
};

template<typename T>
struct CommonValueType<T> {
    using type = T;
};

template<typename Left, typename Right, typename... Rest>
struct CommonValueType<Left, Right, Rest...> {
private:
    using combined = CombineValueTypes_t<Left, Right>;

public:
    using type = IfType_t<
        IsTypeError<combined>::value,
        combined,
        typename CommonValueType<combined, Rest...>::type
    >;
};

template<typename... Ts>
using CommonValueType_t = typename CommonValueType<Ts...>::type;

template<typename T>
struct MeanResultType {
    using type = IfType_t<
        IsSame<T, RealType>::value,
        RealType,
        IfType_t<
            IsSame<T, DecimalType>::value,
            DecimalType,
            RationalType
        >
    >;
};

template<typename T>
using MeanResultType_t = typename MeanResultType<T>::type;

template<typename... RowTypes>
struct CommonMatrixType {
    using type = TypeError<msg_call_type_error>;
};

template<typename Elem, std::size_t Cols>
struct CommonMatrixType<VectorType<Elem, Cols>> {
    using type = MatrixType<Elem, 1, Cols>;
};

template<typename Elem, std::size_t Cols, typename... RestRows>
struct CommonMatrixType<VectorType<Elem, Cols>, RestRows...> {
private:
    template<typename... Rows>
    struct RowsMatch : std::false_type {};

    template<typename FirstRow>
    struct RowsMatch<FirstRow> : std::true_type {};

    template<typename FirstRow, typename NextRow, typename... TailRows>
    struct RowsMatch<FirstRow, NextRow, TailRows...>
        : std::bool_constant<IsSame<FirstRow, NextRow>::value && RowsMatch<NextRow, TailRows...>::value> {};

public:
    using type = IfType_t<
        RowsMatch<VectorType<Elem, Cols>, RestRows...>::value,
        MatrixType<Elem, 1 + sizeof...(RestRows), Cols>,
        TypeError<msg_call_type_error>
    >;
};

template<typename ParamsT, typename ReturnType, typename... ArgTypes>
struct TypeApply<FunctionType<ParamsT, ReturnType>, TypePack<ArgTypes...>> {
private:
    template<typename ParamsX, typename ArgsX>
    struct Matches : std::false_type {};

    template<typename... ParamsP, typename... ArgsA>
    struct Matches<Params<ParamsP...>, TypePack<ArgsA...>>
        : std::bool_constant<
              sizeof...(ParamsP) == sizeof...(ArgsA) &&
              (TypeMatches<typename ParamsP::type, ArgsA>::value && ...)
          > {};

public:
    using type = IfType_t<Matches<ParamsT, TypePack<ArgTypes...>>::value, ReturnType, TypeError<msg_wrong_arity>>;
};

template<typename... ArgTypes>
struct TypeApply<PrimitiveType<Add>, TypePack<ArgTypes...>> {
    using type = TypeError<msg_call_type_error>;
};

template<>
struct TypeApply<PrimitiveType<Add>, TypePack<NatType, NatType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Add>, TypePack<IntType, IntType>> {
    using type = IntType;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Add>, TypePack<Left, Right>> {
    using type = AdditiveNumericType_t<Left, Right>;
};

template<>
struct TypeApply<PrimitiveType<Sub>, TypePack<NatType, NatType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Sub>, TypePack<IntType, IntType>> {
    using type = IntType;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Sub>, TypePack<Left, Right>> {
    using type = IfType_t<
        IsSame<Left, RationalType>::value || IsSame<Right, RationalType>::value,
        RationalType,
        IfType_t<
            IsSame<Left, NatType>::value && IsSame<Right, NatType>::value,
            NatType,
            AdditiveNumericType_t<Left, Right>
        >
    >;
};

template<>
struct TypeApply<PrimitiveType<Mul>, TypePack<NatType, NatType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Mul>, TypePack<IntType, IntType>> {
    using type = IntType;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Mul>, TypePack<Left, Right>> {
    using type = AdditiveNumericType_t<Left, Right>;
};

template<>
struct TypeApply<PrimitiveType<Div>, TypePack<NatType, NatType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Div>, TypePack<IntType, IntType>> {
    using type = IntType;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Div>, TypePack<Left, Right>> {
    using type = IfType_t<
        IsIntegerNumericType<Left>::value && IsIntegerNumericType<Right>::value,
        RationalType,
        RationalType
    >;
};

template<>
struct TypeApply<PrimitiveType<Mod>, TypePack<NatType, NatType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Mod>, TypePack<IntType, IntType>> {
    using type = IntType;
};

template<typename Left, typename Right>
    requires (IsIntegerNumericType<Left>::value && IsIntegerNumericType<Right>::value)
struct TypeApply<PrimitiveType<Mod>, TypePack<Left, Right>> {
    using type = AdditiveNumericType_t<Left, Right>;
};

template<>
struct TypeApply<PrimitiveType<Pow>, TypePack<NatType, NatType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Pow>, TypePack<IntType, NatType>> {
    using type = IntType;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsIntegerNumericType<Right>::value)
struct TypeApply<PrimitiveType<Pow>, TypePack<Left, Right>> {
    using type = AdditiveNumericType_t<Left, Right>;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value)
struct TypeApply<PrimitiveType<Abs>, TypePack<Arg>> {
    using type = Arg;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value)
struct TypeApply<PrimitiveType<Sqrt>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Exp>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Log>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Sin>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Cos>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Tan>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Asin>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Acos>, TypePack<Arg>> {
    using type = RealType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value || IsSame<Arg, RealType>::value)
struct TypeApply<PrimitiveType<Atan>, TypePack<Arg>> {
    using type = RealType;
};

template<>
struct TypeApply<PrimitiveType<Eq>, TypePack<NatType, NatType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Eq>, TypePack<IntType, IntType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Eq>, TypePack<StringType, StringType>> {
    using type = BoolType;
};

template<typename Left, typename Right>
    requires (ComparableNumericTypes<Left, Right>::value)
struct TypeApply<PrimitiveType<Eq>, TypePack<Left, Right>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lt>, TypePack<NatType, NatType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lt>, TypePack<IntType, IntType>> {
    using type = BoolType;
};

template<typename Left, typename Right>
    requires (ComparableNumericTypes<Left, Right>::value)
struct TypeApply<PrimitiveType<Lt>, TypePack<Left, Right>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lte>, TypePack<NatType, NatType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lte>, TypePack<IntType, IntType>> {
    using type = BoolType;
};

template<typename Left, typename Right>
    requires (ComparableNumericTypes<Left, Right>::value)
struct TypeApply<PrimitiveType<Lte>, TypePack<Left, Right>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gt>, TypePack<NatType, NatType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gt>, TypePack<IntType, IntType>> {
    using type = BoolType;
};

template<typename Left, typename Right>
    requires (ComparableNumericTypes<Left, Right>::value)
struct TypeApply<PrimitiveType<Gt>, TypePack<Left, Right>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gte>, TypePack<NatType, NatType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gte>, TypePack<IntType, IntType>> {
    using type = BoolType;
};

template<typename Left, typename Right>
    requires (ComparableNumericTypes<Left, Right>::value)
struct TypeApply<PrimitiveType<Gte>, TypePack<Left, Right>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<IsZero>, TypePack<NatType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<IsZero>, TypePack<IntType>> {
    using type = BoolType;
};

template<typename Arg>
    requires (IsNumericType<Arg>::value)
struct TypeApply<PrimitiveType<IsZero>, TypePack<Arg>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<StringConcat>, TypePack<StringType, StringType>> {
    using type = StringType;
};

template<>
struct TypeApply<PrimitiveType<StringLength>, TypePack<StringType>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<StringEq>, TypePack<StringType, StringType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<StringContains>, TypePack<StringType, StringType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<StringStartsWith>, TypePack<StringType, StringType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<StringTake>, TypePack<NatType, StringType>> {
    using type = StringType;
};

template<>
struct TypeApply<PrimitiveType<StringDrop>, TypePack<NatType, StringType>> {
    using type = StringType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Length>, TypePack<ListType<ElemType>>> {
    using type = NatType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Length>, TypePack<SetType<ElemType>>> {
    using type = NatType;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<Length>, TypePack<MapType<KeyType, ValueType>>> {
    using type = NatType;
};

template<>
struct TypeApply<PrimitiveType<Length>, TypePack<StringType>> {
    using type = NatType;
};

template<typename Elem, std::size_t N>
struct TypeApply<PrimitiveType<Length>, TypePack<VectorType<Elem, N>>> {
    using type = NatType;
};

template<typename Elem, std::size_t Rows, std::size_t Cols>
struct TypeApply<PrimitiveType<Length>, TypePack<MatrixType<Elem, Rows, Cols>>> {
    using type = NatType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<SetInsert>, TypePack<SetType<ElemType>, ElemType>> {
    using type = SetType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<SetContains>, TypePack<SetType<ElemType>, ElemType>> {
    using type = BoolType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<SetErase>, TypePack<SetType<ElemType>, ElemType>> {
    using type = SetType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<SetUnion>, TypePack<SetType<ElemType>, SetType<ElemType>>> {
    using type = SetType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<SetIntersection>, TypePack<SetType<ElemType>, SetType<ElemType>>> {
    using type = SetType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<SetSize>, TypePack<SetType<ElemType>>> {
    using type = NatType;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<MapInsert>, TypePack<MapType<KeyType, ValueType>, KeyType, ValueType>> {
    using type = MapType<KeyType, ValueType>;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<MapInsert>, TypePack<MapType<AnyType, AnyType>, KeyType, ValueType>> {
    using type = MapType<KeyType, ValueType>;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<MapFind>, TypePack<MapType<KeyType, ValueType>, KeyType>> {
    using type = ValueType;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<MapContainsKey>, TypePack<MapType<KeyType, ValueType>, KeyType>> {
    using type = BoolType;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<MapErase>, TypePack<MapType<KeyType, ValueType>, KeyType>> {
    using type = MapType<KeyType, ValueType>;
};

template<typename KeyType, typename ValueType>
struct TypeApply<PrimitiveType<MapSize>, TypePack<MapType<KeyType, ValueType>>> {
    using type = NatType;
};

template<typename... ArgTypes>
struct TypeApply<PrimitiveType<VectorCtor>, TypePack<ArgTypes...>> {
private:
    using elem_type = CommonValueType_t<ArgTypes...>;

public:
    using type = IfType_t<
        IsTypeError<elem_type>::value,
        elem_type,
        VectorType<elem_type, sizeof...(ArgTypes)>
    >;
};

template<typename... RowTypes>
struct TypeApply<PrimitiveType<MatrixCtor>, TypePack<RowTypes...>> {
    using type = typename CommonMatrixType<RowTypes...>::type;
};

template<typename Real, typename Imag>
    requires (IsNumericType<Real>::value && IsNumericType<Imag>::value)
struct TypeApply<PrimitiveType<ComplexCtor>, TypePack<Real, Imag>> {
    using type = ComplexType<AdditiveNumericType_t<Real, Imag>>;
};

template<typename Elem, std::size_t N>
struct TypeApply<PrimitiveType<VecAdd>, TypePack<VectorType<Elem, N>, VectorType<Elem, N>>> {
    using type = VectorType<Elem, N>;
};

template<typename Left, typename Right, std::size_t N>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<VecAdd>, TypePack<VectorType<Left, N>, VectorType<Right, N>>> {
    using type = VectorType<AdditiveNumericType_t<Left, Right>, N>;
};

template<typename Elem, std::size_t N>
struct TypeApply<PrimitiveType<VecSub>, TypePack<VectorType<Elem, N>, VectorType<Elem, N>>> {
    using type = VectorType<Elem, N>;
};

template<typename Left, typename Right, std::size_t N>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<VecSub>, TypePack<VectorType<Left, N>, VectorType<Right, N>>> {
    using type = VectorType<AdditiveNumericType_t<Left, Right>, N>;
};

template<typename Elem, typename Scalar, std::size_t N>
    requires (IsNumericType<Elem>::value && IsNumericType<Scalar>::value)
struct TypeApply<PrimitiveType<VecScale>, TypePack<VectorType<Elem, N>, Scalar>> {
    using type = VectorType<AdditiveNumericType_t<Elem, Scalar>, N>;
};

template<typename Left, typename Right, std::size_t N>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Dot>, TypePack<VectorType<Left, N>, VectorType<Right, N>>> {
    using type = AdditiveNumericType_t<Left, Right>;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Norm>, TypePack<VectorType<Elem, N>>> {
    using type = RealType;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<NormalizeVector>, TypePack<VectorType<Elem, N>>> {
    using type = VectorType<RealType, N>;
};

template<typename Elem, std::size_t Rows, std::size_t Cols>
struct TypeApply<PrimitiveType<MatAdd>, TypePack<MatrixType<Elem, Rows, Cols>, MatrixType<Elem, Rows, Cols>>> {
    using type = MatrixType<Elem, Rows, Cols>;
};

template<typename Left, typename Right, std::size_t Rows, std::size_t Cols>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<MatAdd>, TypePack<MatrixType<Left, Rows, Cols>, MatrixType<Right, Rows, Cols>>> {
    using type = MatrixType<AdditiveNumericType_t<Left, Right>, Rows, Cols>;
};

template<typename Elem, std::size_t Rows, std::size_t Cols>
struct TypeApply<PrimitiveType<MatSub>, TypePack<MatrixType<Elem, Rows, Cols>, MatrixType<Elem, Rows, Cols>>> {
    using type = MatrixType<Elem, Rows, Cols>;
};

template<typename Left, typename Right, std::size_t Rows, std::size_t Cols>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<MatSub>, TypePack<MatrixType<Left, Rows, Cols>, MatrixType<Right, Rows, Cols>>> {
    using type = MatrixType<AdditiveNumericType_t<Left, Right>, Rows, Cols>;
};

template<typename Elem, typename Scalar, std::size_t Rows, std::size_t Cols>
    requires (IsNumericType<Elem>::value && IsNumericType<Scalar>::value)
struct TypeApply<PrimitiveType<MatScale>, TypePack<MatrixType<Elem, Rows, Cols>, Scalar>> {
    using type = MatrixType<AdditiveNumericType_t<Elem, Scalar>, Rows, Cols>;
};

template<typename Left, typename Right, std::size_t Rows, std::size_t Cols>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<MatVecMul>, TypePack<MatrixType<Left, Rows, Cols>, VectorType<Right, Cols>>> {
    using type = VectorType<AdditiveNumericType_t<Left, Right>, Rows>;
};

template<typename Left, typename Right, std::size_t Rows, std::size_t Inner, std::size_t Cols>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<MatMul>, TypePack<MatrixType<Left, Rows, Inner>, MatrixType<Right, Inner, Cols>>> {
    using type = MatrixType<AdditiveNumericType_t<Left, Right>, Rows, Cols>;
};

template<typename Elem, std::size_t Rows, std::size_t Cols>
struct TypeApply<PrimitiveType<Transpose>, TypePack<MatrixType<Elem, Rows, Cols>>> {
    using type = MatrixType<Elem, Cols, Rows>;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Determinant>, TypePack<MatrixType<Elem, N, N>>> {
    using type = Elem;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Inverse>, TypePack<MatrixType<Elem, N, N>>> {
    using type = MatrixType<MeanResultType_t<Elem>, N, N>;
};

template<typename Elem>
struct TypeApply<PrimitiveType<Conjugate>, TypePack<ComplexType<Elem>>> {
    using type = ComplexType<Elem>;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Add>, TypePack<ComplexType<Left>, ComplexType<Right>>> {
    using type = ComplexType<AdditiveNumericType_t<Left, Right>>;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Sub>, TypePack<ComplexType<Left>, ComplexType<Right>>> {
    using type = ComplexType<AdditiveNumericType_t<Left, Right>>;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Mul>, TypePack<ComplexType<Left>, ComplexType<Right>>> {
    using type = ComplexType<AdditiveNumericType_t<Left, Right>>;
};

template<typename Left, typename Right>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Div>, TypePack<ComplexType<Left>, ComplexType<Right>>> {
    using type = ComplexType<AdditiveNumericType_t<Left, Right>>;
};

template<typename Elem>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<NormSquared>, TypePack<ComplexType<Elem>>> {
    using type = Elem;
};

template<typename Elem>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Magnitude>, TypePack<ComplexType<Elem>>> {
    using type = RealType;
};

template<typename Elem>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Argument>, TypePack<ComplexType<Elem>>> {
    using type = RealType;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Mean>, TypePack<VectorType<Elem, N>>> {
    using type = MeanResultType_t<Elem>;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Median>, TypePack<VectorType<Elem, N>>> {
    using type = MeanResultType_t<Elem>;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Mode>, TypePack<VectorType<Elem, N>>> {
    using type = Elem;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<Variance>, TypePack<VectorType<Elem, N>>> {
    using type = MeanResultType_t<Elem>;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<StdDev>, TypePack<VectorType<Elem, N>>> {
    using type = RealType;
};

template<typename Elem, std::size_t N>
struct TypeApply<PrimitiveType<Minimum>, TypePack<VectorType<Elem, N>>> {
    using type = Elem;
};

template<typename Elem, std::size_t N>
struct TypeApply<PrimitiveType<Maximum>, TypePack<VectorType<Elem, N>>> {
    using type = Elem;
};

template<typename Elem, std::size_t N>
    requires (IsNumericType<Elem>::value)
struct TypeApply<PrimitiveType<StatRange>, TypePack<VectorType<Elem, N>>> {
    using type = Elem;
};

template<typename Left, typename Right, std::size_t N>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Covariance>, TypePack<VectorType<Left, N>, VectorType<Right, N>>> {
    using type = MeanResultType_t<AdditiveNumericType_t<Left, Right>>;
};

template<typename Left, typename Right, std::size_t N>
    requires (IsNumericType<Left>::value && IsNumericType<Right>::value)
struct TypeApply<PrimitiveType<Correlation>, TypePack<VectorType<Left, N>, VectorType<Right, N>>> {
    using type = RealType;
};

template<>
struct TypeApply<PrimitiveType<Add>, TypePack<InferType, IntType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Add>, TypePack<IntType, InferType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Sub>, TypePack<InferType, IntType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Sub>, TypePack<IntType, InferType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Mul>, TypePack<InferType, IntType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Mul>, TypePack<IntType, InferType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Div>, TypePack<InferType, IntType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Div>, TypePack<IntType, InferType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Mod>, TypePack<InferType, IntType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Mod>, TypePack<IntType, InferType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Pow>, TypePack<InferType, NatType>> {
    using type = IntType;
};

template<>
struct TypeApply<PrimitiveType<Eq>, TypePack<InferType, IntType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Eq>, TypePack<IntType, InferType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lt>, TypePack<InferType, IntType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lt>, TypePack<IntType, InferType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lte>, TypePack<InferType, IntType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Lte>, TypePack<IntType, InferType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gt>, TypePack<InferType, IntType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gt>, TypePack<IntType, InferType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gte>, TypePack<InferType, IntType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Gte>, TypePack<IntType, InferType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<IsZero>, TypePack<InferType>> {
    using type = BoolType;
};

template<>
struct TypeApply<PrimitiveType<Length>, TypePack<InferType>> {
    using type = NatType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Cons>, TypePack<ElemType, ListType<AnyType>>> {
    using type = ListType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Cons>, TypePack<ElemType, ListType<ElemType>>> {
    using type = ListType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Head>, TypePack<ListType<ElemType>>> {
    using type = ElemType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Tail>, TypePack<ListType<ElemType>>> {
    using type = ListType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<IsEmpty>, TypePack<ListType<ElemType>>> {
    using type = BoolType;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Concat>, TypePack<ListType<ElemType>, ListType<ElemType>>> {
    using type = ListType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Concat>, TypePack<ListType<AnyType>, ListType<ElemType>>> {
    using type = ListType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Concat>, TypePack<ListType<ElemType>, ListType<AnyType>>> {
    using type = ListType<ElemType>;
};

template<typename ElemType>
struct TypeApply<PrimitiveType<Reverse>, TypePack<ListType<ElemType>>> {
    using type = ListType<ElemType>;
};

template<typename Name, typename TypeEnv>
struct TypeCheck<Ref<Name>, TypeEnv> {
    using type = TypeEnvLookup_t<Name, TypeEnv>;
};

template<typename Value, typename TypeEnv>
struct TypeCheck<Quote<Value>, TypeEnv> {
    using type = ValueStaticType_t<Value>;
};

template<typename ThenType, typename ElseType>
struct MergeBranchTypes {
    using type = IfType_t<
        IsSame<ThenType, ElseType>::value,
        ThenType,
        IfType_t<
            IsSame<ThenType, InferType>::value,
            ElseType,
            IfType_t<
                IsSame<ElseType, InferType>::value,
                ThenType,
                TypeError<msg_branch_type_mismatch>
            >
        >
    >;
};

template<typename Cond, typename ThenExpr, typename ElseExpr, typename TypeEnv>
struct TypeCheck<IfExpr<Cond, ThenExpr, ElseExpr>, TypeEnv> {
private:
    using cond_type = typename TypeCheck<Cond, TypeEnv>::type;
    using then_type = typename TypeCheck<ThenExpr, TypeEnv>::type;
    using else_type = typename TypeCheck<ElseExpr, TypeEnv>::type;

public:
    using type = IfType_t<
        IsTypeError<cond_type>::value,
        cond_type,
        IfType_t<
            !IsSame<cond_type, BoolType>::value,
            TypeError<msg_expected_bool>,
            IfType_t<
                IsTypeError<then_type>::value,
                then_type,
                IfType_t<
                    IsTypeError<else_type>::value,
                    else_type,
                    typename MergeBranchTypes<then_type, else_type>::type
                >
            >
        >
    >;
};

template<typename ParamsT, typename Body, typename TypeEnv>
struct TypeCheck<LambdaExpr<ParamsT, Body>, TypeEnv> {
private:
    using extended_env = typename ExtendTypeEnvWithParams<ParamsT, TypeEnv>::type;
    using body_type = typename TypeCheck<Body, extended_env>::type;

public:
    using type = IfType_t<IsTypeError<body_type>::value, body_type, FunctionType<ParamsT, body_type>>;
};

template<typename TypeEnv, typename DonePack, typename... BindingsT>
struct TypeBindingsImpl;

template<typename TypeEnv, typename... Done>
struct TypeBindingsImpl<TypeEnv, TypeList<Done...>> {
    using type = TypeEnv;
};

template<bool Error, typename TypeEnv, typename CurrentType, typename BindingT, typename... RestBindings>
struct TypeBindingsNext;

template<typename TypeEnv, typename CurrentType, typename BindingT, typename... RestBindings>
struct TypeBindingsNext<true, TypeEnv, CurrentType, BindingT, RestBindings...> {
    using type = CurrentType;
};

template<typename TypeEnv, typename CurrentType, typename Name, typename Expr, typename... RestBindings>
struct TypeBindingsNext<false, TypeEnv, CurrentType, Binding<Name, Expr>, RestBindings...> {
    using type = typename TypeBindingsImpl<
        detail::AssocInsert_t<Name, CurrentType, TypeEnv>,
        TypeList<Binding<Name, Expr>>,
        RestBindings...
    >::type;
};

template<typename TypeEnv, typename... Done, typename Name, typename Expr, typename... RestBindings>
struct TypeBindingsImpl<TypeEnv, TypeList<Done...>, Binding<Name, Expr>, RestBindings...> {
private:
    using current_type = typename TypeCheck<Expr, TypeEnv>::type;

public:
    using type = typename TypeBindingsNext<
        IsTypeError<current_type>::value,
        TypeEnv,
        current_type,
        Binding<Name, Expr>,
        RestBindings...
    >::type;
};

template<typename BindingsT, typename TypeEnv>
struct TypeBindings;

template<typename... BindingsT, typename TypeEnv>
struct TypeBindings<Bindings<BindingsT...>, TypeEnv> {
    using type = typename TypeBindingsImpl<TypeEnv, TypeList<>, BindingsT...>::type;
};

template<typename BindingsT, typename Body, typename TypeEnv>
struct TypeCheck<LetExpr<BindingsT, Body>, TypeEnv> {
private:
    using next_env = typename TypeBindings<BindingsT, TypeEnv>::type;

public:
    using type = IfType_t<
        IsTypeError<next_env>::value,
        next_env,
        typename TypeCheck<Body, next_env>::type
    >;
};

template<typename Expr, typename TypeEnv>
struct TypeCheck<BeginExpr<Expr>, TypeEnv> {
    using type = typename TypeCheck<Expr, TypeEnv>::type;
};

template<typename Expr, typename NextExpr, typename... RestExprs, typename TypeEnv>
struct TypeCheck<BeginExpr<Expr, NextExpr, RestExprs...>, TypeEnv> {
private:
    using current_type = typename TypeCheck<Expr, TypeEnv>::type;

public:
    using type = IfType_t<
        IsTypeError<current_type>::value,
        current_type,
        typename TypeCheck<BeginExpr<NextExpr, RestExprs...>, TypeEnv>::type
    >;
};

template<typename Form, typename TypeEnv>
struct TypeCheckProgramForm;

template<typename Expr, typename TypeEnv>
struct TypeCheckProgramForm<ExprForm<Expr>, TypeEnv> {
    using value_type = typename TypeCheck<Expr, TypeEnv>::type;
    using next_env = TypeEnv;
};

template<typename Name, typename Expr, typename TypeEnv>
struct TypeCheckProgramForm<DefineForm<Name, Expr>, TypeEnv> {
private:
    using expr_type = typename TypeCheck<Expr, TypeEnv>::type;

public:
    using value_type = IfType_t<
        ContainsRef<Name, Expr>::value,
        TypeError<msg_recursive_value_define>,
        expr_type
    >;
    using next_env = IfType_t<
        IsTypeError<value_type>::value,
        TypeEnv,
        detail::AssocInsert_t<Name, value_type, TypeEnv>
    >;
};

template<typename Name, typename ParamsT, typename Body, typename TypeEnv>
struct TypeCheckProgramForm<DefineForm<Name, LambdaExpr<ParamsT, Body>>, TypeEnv> {
private:
    using provisional_type = FunctionType<ParamsT, InferType>;
    using env_with_self = detail::AssocInsert_t<Name, provisional_type, TypeEnv>;
    using body_env = typename ExtendTypeEnvWithParams<ParamsT, env_with_self>::type;
    using body_type = typename TypeCheck<Body, body_env>::type;

public:
    using value_type = IfType_t<
        IsTypeError<body_type>::value,
        body_type,
        FunctionType<ParamsT, body_type>
    >;
    using next_env = IfType_t<
        IsTypeError<value_type>::value,
        TypeEnv,
        detail::AssocInsert_t<Name, value_type, TypeEnv>
    >;
};

template<typename TypeEnv, typename LastType, typename... Forms>
struct TypeCheckProgramImpl;

template<typename TypeEnv, typename LastType>
struct TypeCheckProgramImpl<TypeEnv, LastType> {
    using type = LastType;
};

template<typename TypeEnv, typename LastType, typename Form, typename... RestForms>
struct TypeCheckProgramImpl<TypeEnv, LastType, Form, RestForms...> {
private:
    using step = TypeCheckProgramForm<Form, TypeEnv>;

public:
    using type = IfType_t<
        IsTypeError<typename step::value_type>::value,
        typename step::value_type,
        typename TypeCheckProgramImpl<typename step::next_env, typename step::value_type, RestForms...>::type
    >;
};

template<typename... Forms, typename TypeEnv>
struct TypeCheck<ProgramExpr<Forms...>, TypeEnv> {
private:
    using raw_type = typename TypeCheckProgramImpl<TypeEnv, NoneType, Forms...>::type;

public:
    using type = FinalizeInferredType_t<raw_type>;
};

template<typename ParamsT, typename Body, typename... ArgExprs, typename TypeEnv>
struct TypeCheck<CallExpr<LambdaExpr<ParamsT, Body>, ArgExprs...>, TypeEnv> {
private:
    using arg_types = typename TypeArgsImpl<TypeEnv, TypePack<>, ArgExprs...>::type;

    template<typename T>
    struct Dispatch {
        using type = T;
    };

    template<typename... ArgTypes>
    struct Dispatch<TypePack<ArgTypes...>> {
    private:
        using inferred_params = BindInferredParams_t<ParamsT, TypePack<ArgTypes...>>;
        using expected_fn = FunctionType<inferred_params, InferType>;
        using arity_check = typename TypeApply<expected_fn, TypePack<ArgTypes...>>::type;
        using body_env = typename ExtendTypeEnvWithParams<inferred_params, TypeEnv>::type;
        using body_type = typename TypeCheck<Body, body_env>::type;

    public:
        using type = IfType_t<
            IsTypeError<arity_check>::value,
            arity_check,
            body_type
        >;
    };

public:
    using type = typename Dispatch<arg_types>::type;
};

template<typename FnExpr, typename... ArgExprs, typename TypeEnv>
struct TypeCheck<CallExpr<FnExpr, ArgExprs...>, TypeEnv> {
private:
    using fn_type = typename TypeCheck<FnExpr, TypeEnv>::type;
    using arg_types = typename TypeArgsImpl<TypeEnv, TypePack<>, ArgExprs...>::type;

public:
    using type = IfType_t<
        IsTypeError<fn_type>::value,
        fn_type,
        IfType_t<
            IsTypeError<arg_types>::value,
            arg_types,
            typename TypeApply<fn_type, arg_types>::type
        >
    >;
};

} // namespace lc
