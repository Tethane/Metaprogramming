#pragma once

#include "generated/lc/resources.hpp"
#include "lisp.hpp"

namespace lc {

template<typename... Decls>
struct HProgram {};

template<typename Name, typename Type>
struct HSignatureDecl {};

template<typename... Names>
struct HParamList {};

template<typename Name, typename ParamsT, typename Expr>
struct HBindingDecl {};

template<typename Name, typename... Constructors>
struct HDataDecl {};

template<typename Name, typename... FieldTypes>
struct HConstructorDecl {};

template<typename Name>
struct HVarExpr {};

template<typename Value>
struct HLiteralExpr {};

template<typename ParamNames, typename Body>
struct HLambdaExpr {};

template<typename FnExpr, typename ArgExpr>
struct HAppExpr {};

template<typename Primitive, typename... Args>
struct HPrimExpr {};

template<typename CondExpr, typename ThenExpr, typename ElseExpr>
struct HIfExpr {};

template<typename... Bindings>
struct HLetBindings {};

template<typename BindingsT, typename Body>
struct HLetExpr {};

template<typename Pattern, typename Expr>
struct HCaseAlt {};

template<typename Scrutinee, typename... Alts>
struct HCaseExpr {};

template<typename... Items>
struct HListExpr {};

template<typename... Items>
struct HTupleExpr {};

template<typename... Statements>
struct HDoExpr {};

template<typename Pattern, typename Expr>
struct HBindStmt {};

template<typename Expr>
struct HExprStmt {};

template<typename BindingsT>
struct HLetStmt {};

template<typename Expr>
struct HReturnExpr {};

template<typename Expr>
struct HPutStrExpr {};

template<typename Expr>
struct HPutStrLnExpr {};

struct HRandomIntExpr {};
struct HRandomNatExpr {};

template<typename Name>
struct HReadScriptExpr {};

struct HWildcardPattern {};

template<typename Name>
struct HVarPattern {};

template<typename Value>
struct HLiteralPattern {};

struct HEmptyListPattern {};

template<typename Head, typename Tail>
struct HConsPattern {};

template<typename... Items>
struct HTuplePattern {};

template<typename Name, typename... Items>
struct HConstructorPattern {};

struct UnitType {};

template<typename From, typename To>
struct HFunctionType {};

template<typename... Items>
struct HTupleType {};

template<typename Name>
struct HADTType {};

template<typename Inner>
struct IOType {};

struct HUnit {};

template<typename... Items>
struct HTuple {};

template<typename TypeName, typename CtorName, typename... Fields>
struct HConstructorValue {};

template<typename Expr, typename Env>
struct HThunk {};

template<typename Name, typename Expr, typename Env>
struct HRecursiveThunk {};

template<typename ParamsT, typename Body, typename Env>
struct HClosure {};

template<typename Name, typename ParamsT, typename Body, typename Env>
struct HRecursiveClosure {};

template<typename TypeName, typename CtorName, int Remaining, typename... Captured>
struct HConstructorFn {};

template<typename Value, typename OutputLog, typename NextSeed>
struct EffectResult {
    using value = Value;
    using output = OutputLog;
    using next_seed = NextSeed;
};

using msg_haskell_invalid_program = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'h', 'a', 's', 'k', 'e', 'l', 'l', '-', 'p', 'r', 'o', 'g', 'r', 'a', 'm'>;
using msg_haskell_invalid_pattern = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'p', 'a', 't', 't', 'e', 'r', 'n'>;
using msg_haskell_non_exhaustive = String<'n', 'o', 'n', '-', 'e', 'x', 'h', 'a', 'u', 's', 't', 'i', 'v', 'e', '-', 'p', 'a', 't', 't', 'e', 'r', 'n'>;
using msg_haskell_signature_required = String<'s', 'i', 'g', 'n', 'a', 't', 'u', 'r', 'e', '-', 'r', 'e', 'q', 'u', 'i', 'r', 'e', 'd'>;
using msg_haskell_effect_requires_run = String<'e', 'f', 'f', 'e', 'c', 't', '-', 'r', 'e', 'q', 'u', 'i', 'r', 'e', 's', '-', 'r', 'u', 'n'>;
using msg_haskell_unknown_constructor = String<'u', 'n', 'k', 'n', 'o', 'w', 'n', '-', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c', 't', 'o', 'r'>;
using msg_haskell_script_not_found = String<'s', 'c', 'r', 'i', 'p', 't', '-', 'n', 'o', 't', '-', 'f', 'o', 'u', 'n', 'd'>;
using msg_haskell_expected_io = String<'e', 'x', 'p', 'e', 'c', 't', 'e', 'd', '-', 'i', 'o'>;
using msg_haskell_invalid_do = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'd', 'o'>;

template<>
struct ValueStaticType<HUnit> {
    using type = UnitType;
};

template<typename... Items>
struct ValueStaticType<HTuple<Items...>> {
    using type = HTupleType<typename ValueStaticType<Items>::type...>;
};

template<typename TypeName, typename CtorName, typename... Fields>
struct ValueStaticType<HConstructorValue<TypeName, CtorName, Fields...>> {
    using type = HADTType<TypeName>;
};

template<typename Value, typename OutputLog, typename NextSeed>
struct ValueStaticType<EffectResult<Value, OutputLog, NextSeed>> {
    using type = IOType<typename ValueStaticType<Value>::type>;
};

template<typename T>
struct ContainsInferType<HFunctionType<T, T>> : ContainsInferType<T> {};

template<typename From, typename To>
struct ContainsInferType<HFunctionType<From, To>>
    : std::bool_constant<ContainsInferType<From>::value || ContainsInferType<To>::value> {};

template<typename... Items>
struct ContainsInferType<HTupleType<Items...>>
    : std::bool_constant<(ContainsInferType<Items>::value || ...)> {};

template<typename Inner>
struct ContainsInferType<IOType<Inner>> : ContainsInferType<Inner> {};

template<typename Left, typename Right>
struct TypeMatches<HFunctionType<Left, Right>, HFunctionType<Left, Right>> : std::true_type {};

template<typename... Left, typename... Right>
struct TypeMatches<HTupleType<Left...>, HTupleType<Right...>>
    : std::bool_constant<sizeof...(Left) == sizeof...(Right) && (TypeMatches<Left, Right>::value && ...)> {};

template<typename Name>
struct TypeMatches<HADTType<Name>, HADTType<Name>> : std::true_type {};

template<typename InnerLeft, typename InnerRight>
struct TypeMatches<IOType<InnerLeft>, IOType<InnerRight>> : TypeMatches<InnerLeft, InnerRight> {};

namespace haskell_detail {

template<typename... Types>
struct BuildCurriedType;

template<typename Final>
struct BuildCurriedType<Final> {
    using type = Final;
};

template<typename Head, typename Next, typename... Rest>
struct BuildCurriedType<Head, Next, Rest...> {
    using type = HFunctionType<Head, typename BuildCurriedType<Next, Rest...>::type>;
};

template<typename... Types>
using BuildCurriedType_t = typename BuildCurriedType<Types...>::type;

template<typename Name>
struct HEnvLookup {
    template<typename Env>
    struct In {
    private:
        using found = detail::AssocFind_t<Name, Env>;

    public:
        using type = IfType_t<IsSame<found, None>::value, EvalError<msg_unbound_symbol>, found>;
    };
};

template<typename Name, typename Env>
using HEnvLookup_t = typename HEnvLookup<Name>::template In<Env>::type;

template<typename Name>
struct HTypeEnvLookup {
    template<typename Env>
    struct In {
    private:
        using found = detail::AssocFind_t<Name, Env>;

    public:
        using type = IfType_t<IsSame<found, None>::value, TypeError<msg_unbound_symbol>, found>;
    };
};

template<typename Name, typename Env>
using HTypeEnvLookup_t = typename HTypeEnvLookup<Name>::template In<Env>::type;

template<typename T>
struct ForceValue {
    using type = T;
};

template<typename Expr, typename Env>
struct HForce;

template<typename Expr, typename Env>
using HForce_t = typename HForce<Expr, Env>::type;

template<typename T>
struct IsHError : std::bool_constant<IsEvalError<T>::value || IsTypeError<T>::value> {};

template<typename Name, typename Value, typename Env>
using InsertEnv_t = detail::AssocInsert_t<Name, Value, Env>;

template<typename... Names>
struct ArityOfParams : std::integral_constant<int, sizeof...(Names)> {};

template<typename ParamsT>
struct ParamsArity;

template<typename... Names>
struct ParamsArity<HParamList<Names...>> : std::integral_constant<int, sizeof...(Names)> {};

template<typename Expr, typename Env>
struct EvalExpr;

template<typename Expr, typename Env>
using EvalExpr_t = typename EvalExpr<Expr, Env>::type;

template<typename Expr, typename Env, int Seed = 0>
struct RunExpr;

template<typename Expr, typename Env, int Seed = 0>
using RunExpr_t = typename RunExpr<Expr, Env, Seed>::type;

template<typename Value, typename ArgExpr, typename ArgEnv>
struct ApplyValue {
    using type = EvalError<msg_invalid_application>;
};

template<typename Param, typename... Rest, typename Body, typename ClosureEnv, typename ArgExpr, typename ArgEnv>
struct ApplyValue<HClosure<HParamList<Param, Rest...>, Body, ClosureEnv>, ArgExpr, ArgEnv> {
private:
    using arg_value = EvalExpr_t<ArgExpr, ArgEnv>;
    using bound_env = InsertEnv_t<Param, arg_value, ClosureEnv>;

public:
    using type = IfType_t<
        sizeof...(Rest) == 0,
        EvalExpr_t<Body, bound_env>,
        HClosure<HParamList<Rest...>, Body, bound_env>
    >;
};

template<typename Name, typename Param, typename... Rest, typename Body, typename ClosureEnv, typename ArgExpr, typename ArgEnv>
struct ApplyValue<HRecursiveClosure<Name, HParamList<Param, Rest...>, Body, ClosureEnv>, ArgExpr, ArgEnv> {
private:
    using self_env = InsertEnv_t<Name, HRecursiveClosure<Name, HParamList<Param, Rest...>, Body, ClosureEnv>, ClosureEnv>;
    using arg_value = EvalExpr_t<ArgExpr, ArgEnv>;
    using bound_env = InsertEnv_t<Param, arg_value, self_env>;

public:
    using type = IfType_t<
        sizeof...(Rest) == 0,
        EvalExpr_t<Body, bound_env>,
        HRecursiveClosure<Name, HParamList<Rest...>, Body, bound_env>
    >;
};

template<typename TypeName, typename CtorName, int Remaining, typename... Captured, typename ArgExpr, typename ArgEnv>
struct ApplyValue<HConstructorFn<TypeName, CtorName, Remaining, Captured...>, ArgExpr, ArgEnv> {
    using captured = HThunk<ArgExpr, ArgEnv>;
    using type = IfType_t<
        Remaining == 1,
        HConstructorValue<TypeName, CtorName, Captured..., captured>,
        HConstructorFn<TypeName, CtorName, Remaining - 1, Captured..., captured>
    >;
};

template<typename Expr, typename Env>
struct HForce {
    using type = EvalExpr_t<Expr, Env>;
};

template<typename T, typename ThunkEnv, typename CurrentEnv>
struct HForce<HThunk<T, ThunkEnv>, CurrentEnv> {
    using type = EvalExpr_t<T, ThunkEnv>;
};

template<typename Name, typename Expr, typename ThunkEnv, typename CurrentEnv>
struct HForce<HRecursiveThunk<Name, Expr, ThunkEnv>, CurrentEnv> {
private:
    using self_env = InsertEnv_t<Name, HRecursiveThunk<Name, Expr, ThunkEnv>, ThunkEnv>;

public:
    using type = EvalExpr_t<Expr, self_env>;
};

template<int N, typename Env>
struct HForce<Nat<N>, Env> {
    using type = Nat<N>;
};

template<int N, typename Env>
struct HForce<Int<N>, Env> {
    using type = Int<N>;
};

template<bool B, typename Env>
struct HForce<Bool<B>, Env> {
    using type = Bool<B>;
};

template<char... Chars, typename Env>
struct HForce<String<Chars...>, Env> {
    using type = String<Chars...>;
};

template<typename Expr, typename Env>
struct EvalExpr {
    using type = Expr;
};

template<typename Name, typename Env>
struct EvalExpr<HVarExpr<Name>, Env> {
private:
    using found = HEnvLookup_t<Name, Env>;

public:
    using type = IfType_t<IsEvalError<found>::value, found, HForce_t<found, Env>>;
};

template<typename Value, typename Env>
struct EvalExpr<HLiteralExpr<Value>, Env> {
    using type = Value;
};

template<typename ParamsT, typename Body, typename Env>
struct EvalExpr<HLambdaExpr<ParamsT, Body>, Env> {
    using type = HClosure<ParamsT, Body, Env>;
};

template<typename Primitive, typename... Args, typename Env>
struct EvalExpr<HPrimExpr<Primitive, Args...>, Env> {
    using type = Normalize_t<Apply_t<Primitive, EvalExpr_t<Args, Env>...>, 4096>;
};

template<typename FnExpr, typename ArgExpr, typename Env>
struct EvalExpr<HAppExpr<FnExpr, ArgExpr>, Env> {
private:
    using fn_value = EvalExpr_t<FnExpr, Env>;

public:
    using type = IfType_t<
        IsEvalError<fn_value>::value,
        fn_value,
        typename ApplyValue<fn_value, ArgExpr, Env>::type
    >;
};

template<typename CondExpr, typename ThenExpr, typename ElseExpr, typename Env>
struct EvalExpr<HIfExpr<CondExpr, ThenExpr, ElseExpr>, Env> {
private:
    using cond_value = EvalExpr_t<CondExpr, Env>;

public:
    using type = IfType_t<
        IsSame<cond_value, Bool<true>>::value,
        EvalExpr_t<ThenExpr, Env>,
        IfType_t<
            IsSame<cond_value, Bool<false>>::value,
            EvalExpr_t<ElseExpr, Env>,
            EvalError<msg_expected_bool>
        >
    >;
};

template<typename... Items, typename Env>
struct EvalExpr<HListExpr<Items...>, Env> {
    using type = List<EvalExpr_t<Items, Env>...>;
};

template<typename... Items, typename Env>
struct EvalExpr<HTupleExpr<Items...>, Env> {
    using type = HTuple<EvalExpr_t<Items, Env>...>;
};

template<typename BindingsT, typename Env>
struct BuildLetEnv;

template<typename BaseEnv, typename... Bindings>
struct BuildRecursiveBindingEnv;

template<typename BaseEnv>
struct BuildRecursiveBindingEnv<BaseEnv> {
    using type = BaseEnv;
};

template<typename BaseEnv, typename Name, typename ParamsT, typename Expr, typename... Rest>
struct BuildRecursiveBindingEnv<BaseEnv, HBindingDecl<Name, ParamsT, Expr>, Rest...> {
private:
    using value = IfType_t<
        ParamsArity<ParamsT>::value == 0,
        HRecursiveThunk<Name, Expr, BaseEnv>,
        HRecursiveClosure<Name, ParamsT, Expr, BaseEnv>
    >;
    using next_env = InsertEnv_t<Name, value, BaseEnv>;

public:
    using type = typename BuildRecursiveBindingEnv<next_env, Rest...>::type;
};

template<typename Env>
struct BuildLetEnv<HLetBindings<>, Env> {
    using type = Env;
};

template<typename... Bindings, typename Env>
struct BuildLetEnv<HLetBindings<Bindings...>, Env> {
    using type = typename BuildRecursiveBindingEnv<Env, Bindings...>::type;
};

template<typename BindingsT, typename Body, typename Env>
struct EvalExpr<HLetExpr<BindingsT, Body>, Env> {
private:
    using local_env = typename BuildLetEnv<BindingsT, Env>::type;

public:
    using type = EvalExpr_t<Body, local_env>;
};

template<typename Pattern, typename Value, typename Env>
struct MatchPattern {
    using type = EvalError<msg_haskell_invalid_pattern>;
};

template<typename Value, typename Env>
struct MatchPattern<HWildcardPattern, Value, Env> {
    using type = Env;
};

template<typename Name, typename Value, typename Env>
struct MatchPattern<HVarPattern<Name>, Value, Env> {
    using type = InsertEnv_t<Name, Value, Env>;
};

template<typename Literal, typename Env>
struct MatchPattern<HLiteralPattern<Literal>, Literal, Env> {
    using type = Env;
};

template<typename Env>
struct MatchPattern<HEmptyListPattern, List<>, Env> {
    using type = Env;
};

template<typename HeadPattern, typename TailPattern, typename Head, typename... Tail, typename Env>
struct MatchPattern<HConsPattern<HeadPattern, TailPattern>, List<Head, Tail...>, Env> {
private:
    using with_head = typename MatchPattern<HeadPattern, Head, Env>::type;

public:
    using type = IfType_t<
        IsEvalError<with_head>::value,
        with_head,
        typename MatchPattern<TailPattern, List<Tail...>, with_head>::type
    >;
};

template<typename... Patterns, typename... Values, typename Env>
struct MatchPattern<HTuplePattern<Patterns...>, HTuple<Values...>, Env> {
private:
    template<typename PatternPack, typename ValuePack, typename CurrentEnv>
    struct MatchTuple;

    template<typename CurrentEnv>
    struct MatchTuple<TypeList<>, TypeList<>, CurrentEnv> {
        using type = CurrentEnv;
    };

    template<typename Pattern, typename... RestPatterns, typename Value, typename... RestValues, typename CurrentEnv>
    struct MatchTuple<TypeList<Pattern, RestPatterns...>, TypeList<Value, RestValues...>, CurrentEnv> {
    private:
        using next_env = typename MatchPattern<Pattern, Value, CurrentEnv>::type;

    public:
        using type = IfType_t<
            IsEvalError<next_env>::value,
            next_env,
            typename MatchTuple<TypeList<RestPatterns...>, TypeList<RestValues...>, next_env>::type
        >;
    };

public:
    using type = IfType_t<
        sizeof...(Patterns) == sizeof...(Values),
        typename MatchTuple<TypeList<Patterns...>, TypeList<Values...>, Env>::type,
        EvalError<msg_haskell_invalid_pattern>
    >;
};

template<typename CtorName, typename... Patterns, typename TypeName, typename... Values, typename Env>
struct MatchPattern<HConstructorPattern<CtorName, Patterns...>, HConstructorValue<TypeName, CtorName, Values...>, Env> {
private:
    template<typename PatternPack, typename ValuePack, typename CurrentEnv>
    struct MatchCtor;

    template<typename CurrentEnv>
    struct MatchCtor<TypeList<>, TypeList<>, CurrentEnv> {
        using type = CurrentEnv;
    };

    template<typename Pattern, typename... RestPatterns, typename Value, typename... RestValues, typename CurrentEnv>
    struct MatchCtor<TypeList<Pattern, RestPatterns...>, TypeList<Value, RestValues...>, CurrentEnv> {
    private:
        using next_env = typename MatchPattern<Pattern, Value, CurrentEnv>::type;

    public:
        using type = IfType_t<
            IsEvalError<next_env>::value,
            next_env,
            typename MatchCtor<TypeList<RestPatterns...>, TypeList<RestValues...>, next_env>::type
        >;
    };

public:
    using type = IfType_t<
        sizeof...(Patterns) == sizeof...(Values),
        typename MatchCtor<TypeList<Patterns...>, TypeList<Values...>, Env>::type,
        EvalError<msg_haskell_invalid_pattern>
    >;
};

template<typename Value, typename Env, typename... Alts>
struct EvalCaseAlts;

template<typename Value, typename Env>
struct EvalCaseAlts<Value, Env> {
    using type = EvalError<msg_haskell_non_exhaustive>;
};

template<typename Value, typename Env, typename Pattern, typename Expr, typename... Rest>
struct EvalCaseAlts<Value, Env, HCaseAlt<Pattern, Expr>, Rest...> {
private:
    using matched_env = typename MatchPattern<Pattern, Value, Env>::type;

public:
    using type = IfType_t<
        IsEvalError<matched_env>::value,
        typename EvalCaseAlts<Value, Env, Rest...>::type,
        EvalExpr_t<Expr, matched_env>
    >;
};

template<typename Scrutinee, typename... Alts, typename Env>
struct EvalExpr<HCaseExpr<Scrutinee, Alts...>, Env> {
private:
    using value = EvalExpr_t<Scrutinee, Env>;

public:
    using type = EvalCaseAlts<value, Env, Alts...>;
};

template<typename... Statements, typename Env>
struct EvalExpr<HDoExpr<Statements...>, Env> {
    using type = EvalError<msg_haskell_effect_requires_run>;
};

template<typename Expr, typename Env, int Seed>
struct RunExpr {
private:
    using value = EvalExpr_t<Expr, Env>;

public:
    using type = EffectResult<value, String<>, Int<Seed>>;
};

template<typename Expr, typename Env, int Seed>
struct RunExpr<HReturnExpr<Expr>, Env, Seed> {
    using type = EffectResult<EvalExpr_t<Expr, Env>, String<>, Int<Seed>>;
};

template<typename Expr, typename Env, int Seed>
struct RunExpr<HPutStrExpr<Expr>, Env, Seed> {
    using type = EffectResult<HUnit, EvalExpr_t<Expr, Env>, Int<Seed>>;
};

template<typename Expr, typename Env, int Seed>
struct RunExpr<HPutStrLnExpr<Expr>, Env, Seed> {
private:
    using text = EvalExpr_t<Expr, Env>;

public:
    using type = EffectResult<HUnit, detail::StringConcat_t<text, String<'\n'>>, Int<Seed>>;
};

template<typename Env, int Seed>
struct RunExpr<HRandomIntExpr, Env, Seed> {
private:
    static constexpr int next_seed = (Seed * 1103515245 + 12345) & 0x7fffffff;

public:
    using type = EffectResult<Int<next_seed>, String<>, Int<next_seed>>;
};

template<typename Env, int Seed>
struct RunExpr<HRandomNatExpr, Env, Seed> {
private:
    static constexpr int next_seed = (Seed * 1103515245 + 12345) & 0x7fffffff;

public:
    using type = EffectResult<Nat<next_seed>, String<>, Int<next_seed>>;
};

template<typename Name, typename Env, int Seed>
struct RunExpr<HReadScriptExpr<Name>, Env, Seed> {
private:
    using resource = generated_resources::ResourceByName<Name>;

public:
    using type = IfType_t<
        resource::found,
        EffectResult<fixed_string_to_string_t<resource::source>, String<>, Int<Seed>>,
        EvalError<msg_haskell_script_not_found>
    >;
};

template<typename Pattern, typename Expr, typename Env, int Seed, typename... Rest>
struct RunDo;

template<typename Env, int Seed>
struct RunDo<void, void, Env, Seed> {
    using type = EffectResult<HUnit, String<>, Int<Seed>>;
};

template<typename Expr, typename Env, int Seed>
struct RunDo<HExprStmt<Expr>, void, Env, Seed> {
    using type = RunExpr_t<Expr, Env, Seed>;
};

template<typename Pattern, typename Expr, typename Env, int Seed, typename Next, typename... Rest>
struct RunDo<HBindStmt<Pattern, Expr>, Next, Env, Seed, Rest...> {
private:
    using current = RunExpr_t<Expr, Env, Seed>;
    using matched_env = typename MatchPattern<Pattern, typename current::value, Env>::type;
    using next = typename RunDo<Next, void, matched_env, current::next_seed::value, Rest...>::type;

public:
    using type = EffectResult<
        typename next::value,
        detail::StringConcat_t<typename current::output, typename next::output>,
        typename next::next_seed
    >;
};

template<typename Expr, typename Next, typename Env, int Seed, typename... Rest>
struct RunDo<HExprStmt<Expr>, Next, Env, Seed, Rest...> {
private:
    using current = RunExpr_t<Expr, Env, Seed>;
    using next = typename RunDo<Next, void, Env, current::next_seed::value, Rest...>::type;

public:
    using type = EffectResult<
        typename next::value,
        detail::StringConcat_t<typename current::output, typename next::output>,
        typename next::next_seed
    >;
};

template<typename BindingsT, typename Next, typename Env, int Seed, typename... Rest>
struct RunDo<HLetStmt<BindingsT>, Next, Env, Seed, Rest...> {
private:
    using local_env = typename BuildLetEnv<BindingsT, Env>::type;
    using next = typename RunDo<Next, void, local_env, Seed, Rest...>::type;

public:
    using type = next;
};

template<typename Last, typename Env, int Seed>
struct RunDo<Last, void, Env, Seed> {
    using type = RunExpr_t<Last, Env, Seed>;
};

template<typename First, typename Second, typename... Rest, typename Env, int Seed>
struct RunExpr<HDoExpr<First, Second, Rest...>, Env, Seed> {
    using type = typename RunDo<First, Second, Env, Seed, Rest...>::type;
};

template<typename BindingT, typename TypeEnv, typename SigEnv>
struct BindingTypeCheck {
    using type = TypeError<msg_call_type_error>;
};

template<typename Expr, typename TypeEnv>
struct InferExpr;

template<typename Expr, typename TypeEnv>
using InferExpr_t = typename InferExpr<Expr, TypeEnv>::type;

template<typename Expr, typename Expected, typename TypeEnv>
struct CheckExpr {
private:
    using inferred = InferExpr_t<Expr, TypeEnv>;

public:
    using type = IfType_t<
        IsTypeError<inferred>::value,
        inferred,
        IfType_t<TypeMatches<Expected, inferred>::value, Expected, TypeError<msg_call_type_error>>
    >;
};

template<typename Value, typename TypeEnv>
struct InferExpr<HLiteralExpr<Value>, TypeEnv> {
    using type = ValueStaticType_t<Value>;
};

template<typename Name, typename TypeEnv>
struct InferExpr<HVarExpr<Name>, TypeEnv> {
    using type = HTypeEnvLookup_t<Name, TypeEnv>;
};

template<typename Primitive, typename... Args, typename TypeEnv>
struct InferExpr<HPrimExpr<Primitive, Args...>, TypeEnv> {
    using type = typename TypeApply<PrimitiveType<Primitive>, TypePack<InferExpr_t<Args, TypeEnv>...>>::type;
};

template<typename FnExpr, typename ArgExpr, typename TypeEnv>
struct InferExpr<HAppExpr<FnExpr, ArgExpr>, TypeEnv> {
private:
    using fn_type = InferExpr_t<FnExpr, TypeEnv>;
    using arg_type = InferExpr_t<ArgExpr, TypeEnv>;

    template<typename FnType>
    struct ApplyFn {
        using type = TypeError<msg_call_type_error>;
    };

    template<typename From, typename To>
    struct ApplyFn<HFunctionType<From, To>> {
        using type = IfType_t<TypeMatches<From, arg_type>::value, To, TypeError<msg_call_type_error>>;
    };

public:
    using type = IfType_t<
        IsTypeError<fn_type>::value || IsTypeError<arg_type>::value,
        TypeError<msg_call_type_error>,
        typename ApplyFn<fn_type>::type
    >;
};

template<typename CondExpr, typename ThenExpr, typename ElseExpr, typename TypeEnv>
struct InferExpr<HIfExpr<CondExpr, ThenExpr, ElseExpr>, TypeEnv> {
private:
    using cond_type = InferExpr_t<CondExpr, TypeEnv>;
    using then_type = InferExpr_t<ThenExpr, TypeEnv>;
    using else_type = InferExpr_t<ElseExpr, TypeEnv>;

public:
    using type = IfType_t<
        !TypeMatches<BoolType, cond_type>::value,
        TypeError<msg_expected_bool>,
        IfType_t<TypeMatches<then_type, else_type>::value, then_type, TypeError<msg_branch_type_mismatch>>
    >;
};

template<typename TypeEnv>
struct InferExpr<HListExpr<>, TypeEnv> {
    using type = ListType<AnyType>;
};

template<typename First, typename... Rest, typename TypeEnv>
struct InferExpr<HListExpr<First, Rest...>, TypeEnv> {
private:
    using first_type = InferExpr_t<First, TypeEnv>;
    static constexpr bool matches = (TypeMatches<first_type, InferExpr_t<Rest, TypeEnv>>::value && ...);

public:
    using type = IfType_t<matches, ListType<first_type>, TypeError<msg_heterogeneous_list>>;
};

template<typename... Items, typename TypeEnv>
struct InferExpr<HTupleExpr<Items...>, TypeEnv> {
    using type = HTupleType<InferExpr_t<Items, TypeEnv>...>;
};

template<typename Expr, typename TypeEnv>
struct InferExpr<HReturnExpr<Expr>, TypeEnv> {
    using type = IOType<InferExpr_t<Expr, TypeEnv>>;
};

template<typename Expr, typename TypeEnv>
struct InferExpr<HPutStrExpr<Expr>, TypeEnv> {
private:
    using expr_type = InferExpr_t<Expr, TypeEnv>;

public:
    using type = IfType_t<TypeMatches<StringType, expr_type>::value, IOType<UnitType>, TypeError<msg_call_type_error>>;
};

template<typename Expr, typename TypeEnv>
struct InferExpr<HPutStrLnExpr<Expr>, TypeEnv> {
    using type = typename InferExpr<HPutStrExpr<Expr>, TypeEnv>::type;
};

template<typename TypeEnv>
struct InferExpr<HRandomIntExpr, TypeEnv> {
    using type = IOType<IntType>;
};

template<typename TypeEnv>
struct InferExpr<HRandomNatExpr, TypeEnv> {
    using type = IOType<NatType>;
};

template<typename Name, typename TypeEnv>
struct InferExpr<HReadScriptExpr<Name>, TypeEnv> {
    using type = IOType<StringType>;
};

template<typename TypeEnv>
struct InferExpr<HDoExpr<>, TypeEnv> {
    using type = IOType<UnitType>;
};

template<typename First, typename... Rest, typename TypeEnv>
struct InferExpr<HDoExpr<First, Rest...>, TypeEnv> {
    using type = IOType<AnyType>;
};

template<typename Pattern, typename ValueType, typename TypeEnv>
struct PatternTypeEnv {
    using type = TypeError<msg_haskell_invalid_pattern>;
};

template<typename ValueType, typename TypeEnv>
struct PatternTypeEnv<HWildcardPattern, ValueType, TypeEnv> {
    using type = TypeEnv;
};

template<typename Name, typename ValueType, typename TypeEnv>
struct PatternTypeEnv<HVarPattern<Name>, ValueType, TypeEnv> {
    using type = InsertEnv_t<Name, ValueType, TypeEnv>;
};

template<typename Literal, typename TypeEnv>
struct PatternTypeEnv<HLiteralPattern<Literal>, typename ValueStaticType<Literal>::type, TypeEnv> {
    using type = TypeEnv;
};

template<typename ElemType, typename TypeEnv>
struct PatternTypeEnv<HEmptyListPattern, ListType<ElemType>, TypeEnv> {
    using type = TypeEnv;
};

template<typename HeadPattern, typename TailPattern, typename ElemType, typename TypeEnv>
struct PatternTypeEnv<HConsPattern<HeadPattern, TailPattern>, ListType<ElemType>, TypeEnv> {
private:
    using head_env = typename PatternTypeEnv<HeadPattern, ElemType, TypeEnv>::type;

public:
    using type = IfType_t<
        IsTypeError<head_env>::value,
        head_env,
        typename PatternTypeEnv<TailPattern, ListType<ElemType>, head_env>::type
    >;
};

template<typename Name, typename... Items, typename TypeEnv, typename... FieldTypes>
struct PatternTypeEnv<HConstructorPattern<Name, Items...>, TypeList<FieldTypes...>, TypeEnv>;

template<typename TypeName, typename TypeEnv, typename... Patterns, typename... FieldTypes>
struct PatternTypeEnv<HConstructorPattern<TypeName, Patterns...>, TypeList<FieldTypes...>, TypeEnv> {
private:
    template<typename PatternPack, typename TypePackX, typename CurrentEnv>
    struct BindFields;

    template<typename CurrentEnv>
    struct BindFields<TypeList<>, TypeList<>, CurrentEnv> {
        using type = CurrentEnv;
    };

    template<typename Pattern, typename... RestPatterns, typename FieldType, typename... RestFieldTypes, typename CurrentEnv>
    struct BindFields<TypeList<Pattern, RestPatterns...>, TypeList<FieldType, RestFieldTypes...>, CurrentEnv> {
    private:
        using next = typename PatternTypeEnv<Pattern, FieldType, CurrentEnv>::type;

    public:
        using type = IfType_t<
            IsTypeError<next>::value,
            next,
            typename BindFields<TypeList<RestPatterns...>, TypeList<RestFieldTypes...>, next>::type
        >;
    };

public:
    using type = IfType_t<
        sizeof...(Patterns) == sizeof...(FieldTypes),
        typename BindFields<TypeList<Patterns...>, TypeList<FieldTypes...>, TypeEnv>::type,
        TypeError<msg_haskell_invalid_pattern>
    >;
};

template<typename... Patterns, typename TypeEnv, typename... ItemTypes>
struct PatternTypeEnv<HTuplePattern<Patterns...>, HTupleType<ItemTypes...>, TypeEnv> {
private:
    template<typename PatternPack, typename TypePackX, typename CurrentEnv>
    struct BindTuple;

    template<typename CurrentEnv>
    struct BindTuple<TypeList<>, TypeList<>, CurrentEnv> {
        using type = CurrentEnv;
    };

    template<typename Pattern, typename... RestPatterns, typename ItemType, typename... RestItemTypes, typename CurrentEnv>
    struct BindTuple<TypeList<Pattern, RestPatterns...>, TypeList<ItemType, RestItemTypes...>, CurrentEnv> {
    private:
        using next = typename PatternTypeEnv<Pattern, ItemType, CurrentEnv>::type;

    public:
        using type = IfType_t<
            IsTypeError<next>::value,
            next,
            typename BindTuple<TypeList<RestPatterns...>, TypeList<RestItemTypes...>, next>::type
        >;
    };

public:
    using type = IfType_t<
        sizeof...(Patterns) == sizeof...(ItemTypes),
        typename BindTuple<TypeList<Patterns...>, TypeList<ItemTypes...>, TypeEnv>::type,
        TypeError<msg_haskell_invalid_pattern>
    >;
};

template<typename Pattern, typename Expr, typename TypeEnv, typename ExpectedType>
struct CheckAlt {
    using type = TypeError<msg_haskell_invalid_pattern>;
};

template<typename Pattern, typename Expr, typename TypeEnv, typename ScrutineeType, typename ExpectedType>
struct CheckAlt<HCaseAlt<Pattern, Expr>, TypeList<ScrutineeType>, TypeEnv, ExpectedType> {
private:
    using pattern_env = typename PatternTypeEnv<Pattern, ScrutineeType, TypeEnv>::type;

public:
    using type = IfType_t<
        IsTypeError<pattern_env>::value,
        pattern_env,
        typename CheckExpr<Expr, ExpectedType, pattern_env>::type
    >;
};

template<typename Scrutinee, typename FirstAlt, typename... RestAlts, typename TypeEnv>
struct InferExpr<HCaseExpr<Scrutinee, FirstAlt, RestAlts...>, TypeEnv> {
private:
    using scrutinee_type = InferExpr_t<Scrutinee, TypeEnv>;
    using first_body_type = InferExpr_t<typename FirstAlt::expr, TypeEnv>;

public:
    using type = first_body_type;
};

template<typename ParamsT, typename Body, typename TypeEnv>
struct InferExpr<HLambdaExpr<ParamsT, Body>, TypeEnv> {
    using type = TypeError<msg_could_not_infer>;
};

template<typename Name, typename... Constructors>
struct ConstructorsToTypeEnv;

template<typename Name>
struct ConstructorsToTypeEnv<Name> {
    using type = AssocMap<>;
};

template<typename TypeName, typename CtorName, typename... FieldTypes, typename... Rest>
struct ConstructorsToTypeEnv<TypeName, HConstructorDecl<CtorName, FieldTypes...>, Rest...> {
private:
    using ctor_type = BuildCurriedType_t<FieldTypes..., HADTType<TypeName>>;
    using rest_env = typename ConstructorsToTypeEnv<TypeName, Rest...>::type;

public:
    using type = InsertEnv_t<CtorName, ctor_type, rest_env>;
};

template<typename... Decls>
struct CollectConstructorTypeEnv;

template<>
struct CollectConstructorTypeEnv<> {
    using type = AssocMap<>;
};

template<typename... Rest>
struct CollectConstructorTypeEnv<HDataDecl<Rest...>> {
    using type = AssocMap<>;
};

template<typename TypeName, typename... Constructors, typename... Rest>
struct CollectConstructorTypeEnv<HDataDecl<TypeName, Constructors...>, Rest...> {
private:
    using current = typename ConstructorsToTypeEnv<TypeName, Constructors...>::type;
    using rest = typename CollectConstructorTypeEnv<Rest...>::type;

    template<typename MapA, typename MapB>
    struct MergeMaps;

    template<typename... EntriesA, typename... EntriesB>
    struct MergeMaps<AssocMap<EntriesA...>, AssocMap<EntriesB...>> {
        using type = AssocMap<EntriesA..., EntriesB...>;
    };

public:
    using type = typename MergeMaps<current, rest>::type;
};

template<typename First, typename... Rest>
struct CollectConstructorTypeEnv<First, Rest...> {
    using type = typename CollectConstructorTypeEnv<Rest...>::type;
};

template<typename... Decls>
struct CollectSignatureEnv;

template<>
struct CollectSignatureEnv<> {
    using type = AssocMap<>;
};

template<typename Name, typename Type, typename... Rest>
struct CollectSignatureEnv<HSignatureDecl<Name, Type>, Rest...> {
    using type = InsertEnv_t<Name, Type, typename CollectSignatureEnv<Rest...>::type>;
};

template<typename First, typename... Rest>
struct CollectSignatureEnv<First, Rest...> {
    using type = typename CollectSignatureEnv<Rest...>::type;
};

template<typename TypeName, typename... Constructors>
struct BuildConstructorValueEnv;

template<typename TypeName>
struct BuildConstructorValueEnv<TypeName> {
    using type = AssocMap<>;
};

template<typename TypeName, typename CtorName, typename... FieldTypes, typename... Rest>
struct BuildConstructorValueEnv<TypeName, HConstructorDecl<CtorName, FieldTypes...>, Rest...> {
private:
    using rest = typename BuildConstructorValueEnv<TypeName, Rest...>::type;
    using value = HConstructorFn<TypeName, CtorName, sizeof...(FieldTypes)>;

public:
    using type = InsertEnv_t<CtorName, value, rest>;
};

template<typename... Decls>
struct CollectBindingDecls;

template<>
struct CollectBindingDecls<> {
    using type = TypeList<>;
};

template<typename Name, typename ParamsT, typename Expr, typename... Rest>
struct CollectBindingDecls<HBindingDecl<Name, ParamsT, Expr>, Rest...> {
    using type = PushFront_t<HBindingDecl<Name, ParamsT, Expr>, typename CollectBindingDecls<Rest...>::type>;
};

template<typename First, typename... Rest>
struct CollectBindingDecls<First, Rest...> {
    using type = typename CollectBindingDecls<Rest...>::type;
};

template<typename... Decls>
struct CollectDataValueEnv;

template<>
struct CollectDataValueEnv<> {
    using type = AssocMap<>;
};

template<typename TypeName, typename... Constructors, typename... Rest>
struct CollectDataValueEnv<HDataDecl<TypeName, Constructors...>, Rest...> {
private:
    using current = typename BuildConstructorValueEnv<TypeName, Constructors...>::type;
    using rest = typename CollectDataValueEnv<Rest...>::type;

    template<typename MapA, typename MapB>
    struct MergeMaps;

    template<typename... A, typename... B>
    struct MergeMaps<AssocMap<A...>, AssocMap<B...>> {
        using type = AssocMap<A..., B...>;
    };

public:
    using type = typename MergeMaps<current, rest>::type;
};

template<typename First, typename... Rest>
struct CollectDataValueEnv<First, Rest...> {
    using type = typename CollectDataValueEnv<Rest...>::type;
};

template<typename BaseEnv, typename BindingList>
struct BuildProgramBindingEnv;

template<typename BaseEnv, typename... Bindings>
struct BuildProgramBindingEnv<BaseEnv, TypeList<Bindings...>> {
    using type = typename BuildRecursiveBindingEnv<BaseEnv, Bindings...>::type;
};

template<typename Program>
struct ProgramMainName {
    using type = Symbol<String<'m', 'a', 'i', 'n'>>;
};

template<typename Program>
struct InferProgramType {
    using type = TypeError<msg_haskell_invalid_program>;
};

template<typename... Decls>
struct InferProgramType<HProgram<Decls...>> {
private:
    using constructor_env = typename CollectConstructorTypeEnv<Decls...>::type;
    using signature_env = typename CollectSignatureEnv<Decls...>::type;
    using type_env = AssocMap<>;
    using main_name = typename ProgramMainName<HProgram<Decls...>>::type;

public:
    using type = HTypeEnvLookup_t<main_name, signature_env>;
};

template<typename Program>
struct EvalProgram;

template<typename... Decls>
struct EvalProgram<HProgram<Decls...>> {
private:
    using base_env = typename CollectDataValueEnv<Decls...>::type;
    using bindings = typename CollectBindingDecls<Decls...>::type;
    using env = typename BuildProgramBindingEnv<base_env, bindings>::type;
    using main_name = typename ProgramMainName<HProgram<Decls...>>::type;
    using main_value = HEnvLookup_t<main_name, env>;

public:
    using type = IfType_t<IsEvalError<main_value>::value, main_value, HForce_t<main_value, env>>;
};

template<typename Program, int Seed = 0>
struct RunProgram;

template<typename... Decls, int Seed>
struct RunProgram<HProgram<Decls...>, Seed> {
private:
    using base_env = typename CollectDataValueEnv<Decls...>::type;
    using bindings = typename CollectBindingDecls<Decls...>::type;
    using env = typename BuildProgramBindingEnv<base_env, bindings>::type;
    using main_name = typename ProgramMainName<HProgram<Decls...>>::type;
    using main_value = HEnvLookup_t<main_name, env>;

    template<typename MainValue>
    struct RunMain {
        using type = EffectResult<HForce_t<MainValue, env>, String<>, Int<Seed>>;
    };

    template<typename Expr, typename ThunkEnv>
    struct RunMain<HThunk<Expr, ThunkEnv>> {
        using type = RunExpr_t<Expr, ThunkEnv, Seed>;
    };

public:
    using type = IfType_t<IsEvalError<main_value>::value, main_value, typename RunMain<main_value>::type>;
};

} // namespace haskell_detail

template<typename Program>
using EvalHaskell_t = typename haskell_detail::EvalProgram<Program>::type;

template<typename Program>
using TypeCheckHaskell_t = typename haskell_detail::InferProgramType<Program>::type;

template<typename Program, int Seed = 0>
using RunHaskell_t = typename haskell_detail::RunProgram<Program, Seed>::type;

} // namespace lc
