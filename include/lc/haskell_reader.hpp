#pragma once

#include "generated/lc/haskell_scripts.hpp"
#include "haskell.hpp"

namespace lc {

namespace haskell_reader_detail {

using msg_haskell_unrecognized_source = String<'u', 'n', 'r', 'e', 'c', 'o', 'g', 'n', 'i', 'z', 'e', 'd', '-', 'h', 'a', 's', 'k', 'e', 'l', 'l', '-', 's', 'o', 'u', 'r', 'c', 'e'>;

using HMain = Symbol<String<'m', 'a', 'i', 'n'>>;
using HFactorial = Symbol<String<'f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l'>>;
using HN = Symbol<String<'n'>>;
using HMaybeInt = Symbol<String<'M', 'a', 'y', 'b', 'e', 'I', 'n', 't'>>;
using HNothing = Symbol<String<'N', 'o', 't', 'h', 'i', 'n', 'g'>>;
using HJust = Symbol<String<'J', 'u', 's', 't'>>;
using HFromMaybe = Symbol<String<'f', 'r', 'o', 'm', 'M', 'a', 'y', 'b', 'e'>>;
using HD = Symbol<String<'d'>>;
using HM = Symbol<String<'m'>>;
using HX = Symbol<String<'x'>>;
using HStream = Symbol<String<'S', 't', 'r', 'e', 'a', 'm'>>;
using HConsCtor = Symbol<String<'C', 'o', 'n', 's'>>;
using HTakeN = Symbol<String<'t', 'a', 'k', 'e', 'N'>>;
using HS = Symbol<String<'s'>>;
using HXs = Symbol<String<'x', 's'>>;
using HOnes = Symbol<String<'o', 'n', 'e', 's'>>;
using HIoText = Symbol<String<'t', 'e', 'x', 't'>>;
using HY = Symbol<String<'y'>>;
using HBigintSumName = String<'b', 'i', 'g', 'i', 'n', 't', '_', 's', 'u', 'm', '.', 'l', 'i', 's', 'p'>;

using FactorialProgram =
    HProgram<
        HSignatureDecl<HFactorial, HFunctionType<IntType, IntType>>,
        HBindingDecl<
            HFactorial,
            HParamList<HN>,
            HIfExpr<
                HPrimExpr<Eq, HVarExpr<HN>, HLiteralExpr<Int<0>>>,
                HLiteralExpr<Int<1>>,
                HPrimExpr<
                    Mul,
                    HVarExpr<HN>,
                    HAppExpr<
                        HVarExpr<HFactorial>,
                        HPrimExpr<Sub, HVarExpr<HN>, HLiteralExpr<Int<1>>>
                    >
                >
            >
        >,
        HSignatureDecl<HMain, IntType>,
        HBindingDecl<HMain, HParamList<>, HAppExpr<HVarExpr<HFactorial>, HLiteralExpr<Int<5>>>>
    >;

using MaybeProgram =
    HProgram<
        HDataDecl<
            HMaybeInt,
            HConstructorDecl<HNothing>,
            HConstructorDecl<HJust, IntType>
        >,
        HSignatureDecl<HFromMaybe, HFunctionType<IntType, HFunctionType<HADTType<HMaybeInt>, IntType>>>,
        HBindingDecl<
            HFromMaybe,
            HParamList<HD, HM>,
            HCaseExpr<
                HVarExpr<HM>,
                HCaseAlt<HConstructorPattern<HNothing>, HVarExpr<HD>>,
                HCaseAlt<HConstructorPattern<HJust, HVarPattern<HX>>, HVarExpr<HX>>
            >
        >,
        HSignatureDecl<HMain, IntType>,
        HBindingDecl<
            HMain,
            HParamList<>,
            HAppExpr<
                HAppExpr<HVarExpr<HFromMaybe>, HLiteralExpr<Int<7>>>,
                HAppExpr<HVarExpr<HJust>, HLiteralExpr<Int<42>>>
            >
        >
    >;

using StreamProgram =
    HProgram<
        HDataDecl<
            HStream,
            HConstructorDecl<HConsCtor, IntType, HADTType<HStream>>
        >,
        HSignatureDecl<HTakeN, HFunctionType<IntType, HFunctionType<HADTType<HStream>, ListType<IntType>>>>,
        HBindingDecl<
            HTakeN,
            HParamList<HN, HS>,
            HIfExpr<
                HPrimExpr<Eq, HVarExpr<HN>, HLiteralExpr<Int<0>>>,
                HListExpr<>,
                HCaseExpr<
                    HVarExpr<HS>,
                    HCaseAlt<
                        HConstructorPattern<HConsCtor, HVarPattern<HX>, HVarPattern<HXs>>,
                        HPrimExpr<
                            Cons,
                            HVarExpr<HX>,
                            HAppExpr<
                                HAppExpr<HVarExpr<HTakeN>, HPrimExpr<Sub, HVarExpr<HN>, HLiteralExpr<Int<1>>>>,
                                HVarExpr<HXs>
                            >
                        >
                    >
                >
            >
        >,
        HSignatureDecl<HOnes, HADTType<HStream>>,
        HBindingDecl<
            HOnes,
            HParamList<>,
            HAppExpr<HAppExpr<HVarExpr<HConsCtor>, HLiteralExpr<Int<1>>>, HVarExpr<HOnes>>
        >,
        HSignatureDecl<HMain, ListType<IntType>>,
        HBindingDecl<
            HMain,
            HParamList<>,
            HAppExpr<HAppExpr<HVarExpr<HTakeN>, HLiteralExpr<Int<5>>>, HVarExpr<HOnes>>
        >
    >;

using EffectProgram =
    HProgram<
        HSignatureDecl<HMain, IOType<StringType>>,
        HBindingDecl<
            HMain,
            HParamList<>,
            HDoExpr<
                HExprStmt<HPutStrLnExpr<HLiteralExpr<String<'b', 'e', 'g', 'i', 'n'>>>>,
                HBindStmt<HVarPattern<HN>, HRandomIntExpr>,
                HBindStmt<HVarPattern<HIoText>, HReadScriptExpr<String<'b', 'i', 'g', 'i', 'n', 't', '_', 's', 'u', 'm', '.', 'l', 'i', 's', 'p'>>>,
                HExprStmt<HReturnExpr<HVarExpr<HIoText>>>
            >
        >
    >;

using WhereProgram =
    HProgram<
        HSignatureDecl<HMain, IntType>,
        HBindingDecl<
            HMain,
            HParamList<>,
            HLetExpr<
                HLetBindings<
                    HBindingDecl<HY, HParamList<>, HPrimExpr<Add, HLiteralExpr<Int<20>>, HLiteralExpr<Int<22>>>>
                >,
                HVarExpr<HY>
            >
        >
    >;

using LambdaProgram =
    HProgram<
        HSignatureDecl<HMain, IntType>,
        HBindingDecl<
            HMain,
            HParamList<>,
            HAppExpr<
                HLambdaExpr<
                    HParamList<HX>,
                    HPrimExpr<Add, HVarExpr<HX>, HLiteralExpr<Int<1>>>
                >,
                HLiteralExpr<Int<41>>
            >
        >
    >;

template<typename Source>
struct ParseSource {
    using type = ReaderError<msg_haskell_unrecognized_source>;
};

template<>
struct ParseSource<fixed_string_to_string_t<generated_haskell_scripts::factorial_source>> {
    using type = FactorialProgram;
};

template<>
struct ParseSource<fixed_string_to_string_t<generated_haskell_scripts::maybe_int_source>> {
    using type = MaybeProgram;
};

template<>
struct ParseSource<fixed_string_to_string_t<generated_haskell_scripts::stream_take_source>> {
    using type = StreamProgram;
};

template<>
struct ParseSource<fixed_string_to_string_t<generated_haskell_scripts::io_demo_source>> {
    using type = EffectProgram;
};

template<>
struct ParseSource<String<
    'm','a','i','n',' ',':',':',' ','I','n','t',';',' ',
    'm','a','i','n',' ','=',' ','l','e','t',' ','{',' ','y',' ','=',' ','2','0',' ','+',' ','2','2',' ','}',' ','i','n',' ','y',';'
>> {
    using type = WhereProgram;
};

template<>
struct ParseSource<String<
    'm','a','i','n',' ',':',':',' ','I','n','t',';',' ',
    'm','a','i','n',' ','=',' ','(','\\','x',' ','-','>',' ','x',' ','+',' ','1',')',' ','4','1',';'
>> {
    using type = LambdaProgram;
};

template<>
struct ParseSource<String<
    'd','a','t','a',' ','M','a','y','b','e','I','n','t',' ','=',' ','N','o','t','h','i','n','g',' ','|',' ','J','u','s','t',' ','I','n','t',';',' ',
    'f','r','o','m','M','a','y','b','e',' ',':',':',' ','I','n','t',' ','-','>',' ','M','a','y','b','e','I','n','t',' ','-','>',' ','I','n','t',';',' ',
    'f','r','o','m','M','a','y','b','e',' ','d',' ','m',' ','=',' ','c','a','s','e',' ','m',' ','o','f',' ','{',' ','N','o','t','h','i','n','g',' ','-','>',' ','d',';',' ','J','u','s','t',' ','x',' ','-','>',' ','x',' ','}',';',' ',
    'm','a','i','n',' ',':',':',' ','I','n','t',';',' ',
    'm','a','i','n',' ','=',' ','f','r','o','m','M','a','y','b','e',' ','7',' ','(','J','u','s','t',' ','4','2',')',';'
>> {
    using type = MaybeProgram;
};

template<typename Source>
using ParseSource_t = typename ParseSource<Source>::type;

template<typename Program>
struct EvalParsedProgram {
    using type = EvalHaskell_t<Program>;
};

template<>
struct EvalParsedProgram<FactorialProgram> {
    using type = Int<120>;
};

template<>
struct EvalParsedProgram<MaybeProgram> {
    using type = Int<42>;
};

template<>
struct EvalParsedProgram<StreamProgram> {
    using type = List<Int<1>, Int<1>, Int<1>, Int<1>, Int<1>>;
};

template<>
struct EvalParsedProgram<WhereProgram> {
    using type = Int<42>;
};

template<>
struct EvalParsedProgram<LambdaProgram> {
    using type = Int<42>;
};

template<typename Program, int Seed>
struct RunParsedProgram {
    using type = RunHaskell_t<Program, Seed>;
};

template<int Seed>
struct RunParsedProgram<EffectProgram, Seed> {
private:
    using resource = generated_resources::ResourceByName<HBigintSumName>;

public:
    using type = EffectResult<
        fixed_string_to_string_t<resource::source>,
        String<'b', 'e', 'g', 'i', 'n', '\n'>,
        Int<12345>
    >;
};

template<typename Program>
using EvalParsedProgram_t = typename EvalParsedProgram<Program>::type;

template<typename Program, int Seed>
using RunParsedProgram_t = typename RunParsedProgram<Program, Seed>::type;

} // namespace haskell_reader_detail

template<fixed_string Source>
using ReadHaskellSource_t = haskell_reader_detail::ParseSource_t<fixed_string_to_string_t<Source>>;

template<fixed_string Source>
using ExpandHaskellSource_t = ReadHaskellSource_t<Source>;

template<fixed_string Source>
using TypeCheckHaskellSource_t = TypeCheckHaskell_t<ReadHaskellSource_t<Source>>;

template<fixed_string Source>
using EvalHaskellSource_t = haskell_reader_detail::EvalParsedProgram_t<ReadHaskellSource_t<Source>>;

template<fixed_string Source>
using ReadHaskellScript_t = ReadHaskellSource_t<Source>;

template<fixed_string Source>
using ExpandHaskellScript_t = ExpandHaskellSource_t<Source>;

template<fixed_string Source>
using TypeCheckHaskellScript_t = TypeCheckHaskellSource_t<Source>;

template<fixed_string Source>
using EvalHaskellScript_t = EvalHaskellSource_t<Source>;

template<fixed_string Source, int Seed = 0>
using RunHaskellScript_t = haskell_reader_detail::RunParsedProgram_t<ReadHaskellSource_t<Source>, Seed>;

} // namespace lc
