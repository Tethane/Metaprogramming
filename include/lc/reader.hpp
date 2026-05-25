#pragma once

#include <utility>

#include "lisp.hpp"

namespace lc {

template<typename Message>
struct ReaderError {
    using message = Message;
};

template<typename T>
struct IsReaderError : std::false_type {};

template<typename Message>
struct IsReaderError<ReaderError<Message>> : std::true_type {};

using msg_reader_unexpected_eof = String<'u', 'n', 'e', 'x', 'p', 'e', 'c', 't', 'e', 'd', '-', 'e', 'o', 'f'>;
using msg_reader_unexpected_rparen = String<'u', 'n', 'e', 'x', 'p', 'e', 'c', 't', 'e', 'd', '-', 'r', 'p', 'a', 'r', 'e', 'n'>;
using msg_reader_unterminated_list = String<'u', 'n', 't', 'e', 'r', 'm', 'i', 'n', 'a', 't', 'e', 'd', '-', 'l', 'i', 's', 't'>;
using msg_reader_unterminated_string = String<'u', 'n', 't', 'e', 'r', 'm', 'i', 'n', 'a', 't', 'e', 'd', '-', 's', 't', 'r', 'i', 'n', 'g'>;
using msg_reader_trailing_input = String<'t', 'r', 'a', 'i', 'l', 'i', 'n', 'g', '-', 'i', 'n', 'p', 'u', 't'>;
using msg_reader_invalid_lambda = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'l', 'a', 'm', 'b', 'd', 'a'>;
using msg_reader_invalid_param = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'p', 'a', 'r', 'a', 'm'>;
using msg_reader_invalid_let = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'l', 'e', 't'>;
using msg_reader_invalid_binding = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'b', 'i', 'n', 'd', 'i', 'n', 'g'>;
using msg_reader_invalid_if = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'i', 'f'>;
using msg_reader_invalid_begin = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'b', 'e', 'g', 'i', 'n'>;
using msg_reader_invalid_quote = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'q', 'u', 'o', 't', 'e'>;
using msg_reader_invalid_application = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'a', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n'>;
using msg_reader_unknown_type = String<'u', 'n', 'k', 'n', 'o', 'w', 'n', '-', 't', 'y', 'p', 'e'>;

template<typename Expr, typename Rest>
struct Parsed {
    using expr = Expr;
    using rest = Rest;
};

template<typename Name>
struct SSymbol {};

template<int N>
struct SInt {};

template<char... Chars>
struct SStringLit {};

template<bool B>
struct SBool {};

template<typename... Items>
struct SList {};

namespace reader_detail {

template<typename Str, char C>
struct StringPushBack;

template<char... Chars, char C>
struct StringPushBack<String<Chars...>, C> {
    using type = String<Chars..., C>;
};

template<typename Str, char C>
using StringPushBack_t = typename StringPushBack<Str, C>::type;

template<char C>
struct IsWhitespace : std::bool_constant<
    C == ' ' || C == '\n' || C == '\r' || C == '\t'
> {};

template<char C>
struct IsDigit : std::bool_constant<(C >= '0' && C <= '9')> {};

template<char C>
struct IsDelimiter : std::bool_constant<
    IsWhitespace<C>::value || C == '(' || C == ')' || C == '"' || C == '\'' || C == ';'
> {};

template<typename Str>
struct SkipWhitespace;

template<typename Str>
struct SkipLineComment;

template<>
struct SkipWhitespace<String<>> {
    using type = String<>;
};

template<>
struct SkipLineComment<String<>> {
    using type = String<>;
};

template<char... Rest>
struct SkipLineComment<String<'\n', Rest...>> {
    using type = typename SkipWhitespace<String<Rest...>>::type;
};

template<char... Rest>
struct SkipLineComment<String<'\r', Rest...>> {
    using type = typename SkipWhitespace<String<Rest...>>::type;
};

template<char C, char... Rest>
struct SkipLineComment<String<C, Rest...>> {
    using type = typename SkipLineComment<String<Rest...>>::type;
};

template<bool SkipSpace, bool SkipComment, char C, typename Rest>
struct SkipWhitespaceDispatch;

template<char C, char... Rest>
struct SkipWhitespaceDispatch<false, false, C, String<Rest...>> {
    using type = String<C, Rest...>;
};

template<char C, char... Rest>
struct SkipWhitespaceDispatch<true, false, C, String<Rest...>> {
    using type = typename SkipWhitespace<String<Rest...>>::type;
};

template<char C, char... Rest>
struct SkipWhitespaceDispatch<false, true, C, String<Rest...>> {
    using type = typename SkipLineComment<String<Rest...>>::type;
};

template<char C, char... Rest>
struct SkipWhitespace<String<C, Rest...>> {
    using type = typename SkipWhitespaceDispatch<
        IsWhitespace<C>::value,
        C == ';',
        C,
        String<Rest...>
    >::type;
};

template<typename Str>
using SkipWhitespace_t = typename SkipWhitespace<Str>::type;

template<typename Token, typename Rest>
struct MakeAtom {
    using type = Parsed<SSymbol<Token>, Rest>;
};

template<typename Rest>
struct MakeAtom<String<'t', 'r', 'u', 'e'>, Rest> {
    using type = Parsed<SBool<true>, Rest>;
};

template<typename Rest>
struct MakeAtom<String<'f', 'a', 'l', 's', 'e'>, Rest> {
    using type = Parsed<SBool<false>, Rest>;
};

template<typename Rest>
struct MakeAtom<String<'#', 't'>, Rest> {
    using type = Parsed<SBool<true>, Rest>;
};

template<typename Rest>
struct MakeAtom<String<'#', 'f'>, Rest> {
    using type = Parsed<SBool<false>, Rest>;
};

template<typename Str>
struct ParseExpr;

template<typename Str>
struct ParseExprImpl;

template<typename Acc, typename Str>
struct ParseStringBody;

template<typename Acc, typename Str>
struct ParseStringEscape;

template<int Sign, int Acc, typename Str>
struct ParseNumberBody;

template<typename Acc, typename Str>
struct ParseSymbolBody;

template<typename Acc, typename Str>
struct ParseListItems;

template<typename ParsedT, typename Acc>
struct ParseListContinue {
    using type = ParsedT;
};

template<typename Expr, typename Rest, typename... Items>
struct ParseListContinue<Parsed<Expr, Rest>, SList<Items...>> {
    using type = typename ParseListItems<SList<Items..., Expr>, Rest>::type;
};

template<typename Str>
struct ParseExpr {
    using type = typename ParseExprImpl<SkipWhitespace_t<Str>>::type;
};

template<>
struct ParseExprImpl<String<>> {
    using type = ReaderError<msg_reader_unexpected_eof>;
};

template<char... Rest>
struct ParseExprImpl<String<'(', Rest...>> {
    using type = typename ParseListItems<SList<>, String<Rest...>>::type;
};

template<char... Rest>
struct ParseExprImpl<String<')', Rest...>> {
    using type = ReaderError<msg_reader_unexpected_rparen>;
};

template<typename ParsedT>
struct QuoteDispatch {
    using type = ParsedT;
};

template<typename Expr, typename Rest>
struct QuoteDispatch<Parsed<Expr, Rest>> {
    using type = Parsed<SList<SSymbol<String<'q', 'u', 'o', 't', 'e'>>, Expr>, Rest>;
};

template<char... Rest>
struct ParseExprImpl<String<'\'', Rest...>> {
private:
    using parsed = typename ParseExpr<String<Rest...>>::type;

public:
    using type = typename QuoteDispatch<parsed>::type;
};

template<char... Rest>
struct ParseExprImpl<String<'"', Rest...>> {
    using type = typename ParseStringBody<String<>, String<Rest...>>::type;
};

template<>
struct ParseExprImpl<String<'-'>> {
    using type = typename MakeAtom<String<'-'>, String<>>::type;
};

template<bool Digit, char Next, typename Rest>
struct ParseDashDispatch;

template<char Next, char... Rest>
struct ParseDashDispatch<true, Next, String<Rest...>> {
    using type = typename ParseNumberBody<-1, Next - '0', String<Rest...>>::type;
};

template<char Next, char... Rest>
struct ParseDashDispatch<false, Next, String<Rest...>> {
    using type = typename ParseSymbolBody<String<'-'>, String<Next, Rest...>>::type;
};

template<char Next, char... Rest>
struct ParseExprImpl<String<'-', Next, Rest...>> {
    using type = typename ParseDashDispatch<IsDigit<Next>::value, Next, String<Rest...>>::type;
};

template<bool Digit, char C, typename Rest>
struct ParseInitialDispatch;

template<char C, char... Rest>
struct ParseInitialDispatch<true, C, String<Rest...>> {
    using type = typename ParseNumberBody<1, C - '0', String<Rest...>>::type;
};

template<char C, char... Rest>
struct ParseInitialDispatch<false, C, String<Rest...>> {
    using type = typename ParseSymbolBody<String<C>, String<Rest...>>::type;
};

template<char C, char... Rest>
struct ParseExprImpl<String<C, Rest...>> {
    using type = typename ParseInitialDispatch<IsDigit<C>::value, C, String<Rest...>>::type;
};

template<char... AccChars>
struct ParseStringBody<String<AccChars...>, String<>> {
    using type = ReaderError<msg_reader_unterminated_string>;
};

template<char... AccChars, char... Rest>
struct ParseStringBody<String<AccChars...>, String<'"', Rest...>> {
    using type = Parsed<SStringLit<AccChars...>, String<Rest...>>;
};

template<char... AccChars, char... Rest>
struct ParseStringBody<String<AccChars...>, String<'\\', Rest...>> {
    using type = typename ParseStringEscape<String<AccChars...>, String<Rest...>>::type;
};

template<char... AccChars, char C, char... Rest>
struct ParseStringBody<String<AccChars...>, String<C, Rest...>> {
    using type = typename ParseStringBody<String<AccChars..., C>, String<Rest...>>::type;
};

template<typename Acc>
struct ParseStringEscape<Acc, String<>> {
    using type = ReaderError<msg_reader_unterminated_string>;
};

template<typename Acc, char... Rest>
struct ParseStringEscape<Acc, String<'n', Rest...>> {
    using type = typename ParseStringBody<StringPushBack_t<Acc, '\n'>, String<Rest...>>::type;
};

template<typename Acc, char... Rest>
struct ParseStringEscape<Acc, String<'t', Rest...>> {
    using type = typename ParseStringBody<StringPushBack_t<Acc, '\t'>, String<Rest...>>::type;
};

template<typename Acc, char... Rest>
struct ParseStringEscape<Acc, String<'r', Rest...>> {
    using type = typename ParseStringBody<StringPushBack_t<Acc, '\r'>, String<Rest...>>::type;
};

template<typename Acc, char... Rest>
struct ParseStringEscape<Acc, String<'\\', Rest...>> {
    using type = typename ParseStringBody<StringPushBack_t<Acc, '\\'>, String<Rest...>>::type;
};

template<typename Acc, char... Rest>
struct ParseStringEscape<Acc, String<'"', Rest...>> {
    using type = typename ParseStringBody<StringPushBack_t<Acc, '"'>, String<Rest...>>::type;
};

template<typename Acc, char C, char... Rest>
struct ParseStringEscape<Acc, String<C, Rest...>> {
    using type = typename ParseStringBody<StringPushBack_t<Acc, C>, String<Rest...>>::type;
};

template<int Sign, int Acc>
struct ParseNumberBody<Sign, Acc, String<>> {
    using type = Parsed<SInt<Sign * Acc>, String<>>;
};

template<bool Digit, int Sign, int Acc, typename Str>
struct ParseNumberDispatch;

template<int Sign, int Acc, char C, char... Rest>
struct ParseNumberDispatch<true, Sign, Acc, String<C, Rest...>> {
    using type = typename ParseNumberBody<Sign, Acc * 10 + (C - '0'), String<Rest...>>::type;
};

template<int Sign, int Acc, char C, char... Rest>
struct ParseNumberDispatch<false, Sign, Acc, String<C, Rest...>> {
    using type = Parsed<SInt<Sign * Acc>, String<C, Rest...>>;
};

template<int Sign, int Acc, char C, char... Rest>
struct ParseNumberBody<Sign, Acc, String<C, Rest...>> {
    using type = typename ParseNumberDispatch<IsDigit<C>::value, Sign, Acc, String<C, Rest...>>::type;
};

template<typename Acc>
struct ParseSymbolBody<Acc, String<>> {
    using type = typename MakeAtom<Acc, String<>>::type;
};

template<bool Delimited, typename Acc, typename Str>
struct ParseSymbolDispatch;

template<typename Acc, char C, char... Rest>
struct ParseSymbolDispatch<true, Acc, String<C, Rest...>> {
    using type = typename MakeAtom<Acc, String<C, Rest...>>::type;
};

template<typename Acc, char C, char... Rest>
struct ParseSymbolDispatch<false, Acc, String<C, Rest...>> {
    using type = typename ParseSymbolBody<StringPushBack_t<Acc, C>, String<Rest...>>::type;
};

template<typename Acc, char C, char... Rest>
struct ParseSymbolBody<Acc, String<C, Rest...>> {
    using type = typename ParseSymbolDispatch<IsDelimiter<C>::value, Acc, String<C, Rest...>>::type;
};

template<typename... Items>
struct ParseListItems<SList<Items...>, String<>> {
    using type = ReaderError<msg_reader_unterminated_list>;
};

template<typename Acc, typename Str>
struct ParseListItemsDispatch;

template<typename... Items, char... Rest>
struct ParseListItemsDispatch<SList<Items...>, String<')', Rest...>> {
    using type = Parsed<SList<Items...>, String<Rest...>>;
};

template<typename... Items, char C, char... Rest>
struct ParseListItemsDispatch<SList<Items...>, String<C, Rest...>> {
private:
    using parsed = typename ParseExpr<String<C, Rest...>>::type;

public:
    using type = typename ParseListContinue<parsed, SList<Items...>>::type;
};

template<typename... Items, typename Str>
struct ParseListItems<SList<Items...>, Str> {
    using type = typename ParseListItemsDispatch<SList<Items...>, SkipWhitespace_t<Str>>::type;
};

template<typename ParsedT>
struct ParseProgramResult {
    using type = ParsedT;
};

template<typename Expr, typename Rest>
struct ParseProgramResult<Parsed<Expr, Rest>> {
private:
    using tail = SkipWhitespace_t<Rest>;

public:
    using type = IfType_t<
        IsSame<tail, String<>>::value,
        Expr,
        ReaderError<msg_reader_trailing_input>
    >;
};

template<typename Str>
using ParseProgram_t = typename ParseProgramResult<typename ParseExpr<Str>::type>::type;

template<typename SExpr>
struct LowerExpr;

template<typename SExpr>
struct LowerType {
    using type = ReaderError<msg_reader_unknown_type>;
};

template<typename Name>
struct PrimitiveOf {
    using type = void;
};

template<> struct PrimitiveOf<String<'+'> > { using type = Add; };
template<> struct PrimitiveOf<String<'-'> > { using type = Sub; };
template<> struct PrimitiveOf<String<'*'> > { using type = Mul; };
template<> struct PrimitiveOf<String<'/'> > { using type = Div; };
template<> struct PrimitiveOf<String<'m', 'o', 'd'> > { using type = Mod; };
template<> struct PrimitiveOf<String<'p', 'o', 'w'> > { using type = Pow; };
template<> struct PrimitiveOf<String<'='> > { using type = Eq; };
template<> struct PrimitiveOf<String<'<'> > { using type = Lt; };
template<> struct PrimitiveOf<String<'<', '='> > { using type = Lte; };
template<> struct PrimitiveOf<String<'>'> > { using type = Gt; };
template<> struct PrimitiveOf<String<'>', '='> > { using type = Gte; };
template<> struct PrimitiveOf<String<'z', 'e', 'r', 'o', '?'> > { using type = IsZero; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 'a', 'p', 'p', 'e', 'n', 'd'> > { using type = StringConcat; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 'l', 'e', 'n', 'g', 't', 'h'> > { using type = StringLength; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 'e', 'q'> > { using type = StringEq; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 'c', 'o', 'n', 't', 'a', 'i', 'n', 's'> > { using type = StringContains; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 'p', 'r', 'e', 'f', 'i', 'x', '?'> > { using type = StringStartsWith; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 't', 'a', 'k', 'e'> > { using type = StringTake; };
template<> struct PrimitiveOf<String<'s', 't', 'r', 'i', 'n', 'g', '-', 'd', 'r', 'o', 'p'> > { using type = StringDrop; };
template<> struct PrimitiveOf<String<'l', 'e', 'n', 'g', 't', 'h'> > { using type = Length; };
template<> struct PrimitiveOf<String<'s', 'e', 't', '-', 'i', 'n', 's', 'e', 'r', 't'> > { using type = SetInsert; };
template<> struct PrimitiveOf<String<'s', 'e', 't', '-', 'c', 'o', 'n', 't', 'a', 'i', 'n', 's'> > { using type = SetContains; };
template<> struct PrimitiveOf<String<'s', 'e', 't', '-', 'e', 'r', 'a', 's', 'e'> > { using type = SetErase; };
template<> struct PrimitiveOf<String<'s', 'e', 't', '-', 'u', 'n', 'i', 'o', 'n'> > { using type = SetUnion; };
template<> struct PrimitiveOf<String<'s', 'e', 't', '-', 'i', 'n', 't', 'e', 'r', 's', 'e', 'c', 't', 'i', 'o', 'n'> > { using type = SetIntersection; };
template<> struct PrimitiveOf<String<'s', 'e', 't', '-', 's', 'i', 'z', 'e'> > { using type = SetSize; };
template<> struct PrimitiveOf<String<'m', 'a', 'p', '-', 'i', 'n', 's', 'e', 'r', 't'> > { using type = MapInsert; };
template<> struct PrimitiveOf<String<'m', 'a', 'p', '-', 'f', 'i', 'n', 'd'> > { using type = MapFind; };
template<> struct PrimitiveOf<String<'m', 'a', 'p', '-', 'c', 'o', 'n', 't', 'a', 'i', 'n', 's', '?'> > { using type = MapContainsKey; };
template<> struct PrimitiveOf<String<'m', 'a', 'p', '-', 'e', 'r', 'a', 's', 'e'> > { using type = MapErase; };
template<> struct PrimitiveOf<String<'m', 'a', 'p', '-', 's', 'i', 'z', 'e'> > { using type = MapSize; };

template<typename Name>
using PrimitiveOf_t = typename PrimitiveOf<Name>::type;

template<typename SExpr>
struct QuoteValue {
    using type = ReaderError<msg_reader_invalid_quote>;
};

template<typename Name>
struct QuoteValue<SSymbol<Name>> {
    using type = Symbol<Name>;
};

template<int N>
struct QuoteValue<SInt<N>> {
    using type = Int<N>;
};

template<bool B>
struct QuoteValue<SBool<B>> {
    using type = Bool<B>;
};

template<char... Chars>
struct QuoteValue<SStringLit<Chars...>> {
    using type = String<Chars...>;
};

template<typename... Items>
struct QuoteValue<SList<Items...>> {
    using type = List<typename QuoteValue<Items>::type...>;
};

template<typename LoweredType, template<typename> class Container>
struct WrapLoweredType {
    using type = Container<LoweredType>;
};

template<typename Message, template<typename> class Container>
struct WrapLoweredType<ReaderError<Message>, Container> {
    using type = ReaderError<Message>;
};

template<>
struct LowerType<SSymbol<String<'N', 'a', 't'>>> {
    using type = NatType;
};

template<>
struct LowerType<SSymbol<String<'I', 'n', 't'>>> {
    using type = IntType;
};

template<>
struct LowerType<SSymbol<String<'B', 'o', 'o', 'l'>>> {
    using type = BoolType;
};

template<>
struct LowerType<SSymbol<String<'S', 't', 'r', 'i', 'n', 'g'>>> {
    using type = StringType;
};

template<>
struct LowerType<SSymbol<String<'N', 'o', 'n', 'e'>>> {
    using type = NoneType;
};

template<>
struct LowerType<SSymbol<String<'A', 'n', 'y'>>> {
    using type = AnyType;
};

template<>
struct LowerType<SSymbol<String<'U', 'n', 'k', 'n', 'o', 'w', 'n'>>> {
    using type = UnknownType;
};

template<typename Elem>
struct LowerType<SList<SSymbol<String<'L', 'i', 's', 't'>>, Elem>> {
private:
    using elem_type = typename LowerType<Elem>::type;

public:
    using type = typename WrapLoweredType<elem_type, ListType>::type;
};

template<typename Elem>
struct LowerType<SList<SSymbol<String<'S', 'e', 't'>>, Elem>> {
private:
    using elem_type = typename LowerType<Elem>::type;

public:
    using type = typename WrapLoweredType<elem_type, SetType>::type;
};

template<typename Key, typename Value>
struct LowerType<SList<SSymbol<String<'M', 'a', 'p'>>, Key, Value>> {
private:
    using key_type = typename LowerType<Key>::type;
    using value_type = typename LowerType<Value>::type;

public:
    using type = IfType_t<
        IsReaderError<key_type>::value,
        key_type,
        IfType_t<
            IsReaderError<value_type>::value,
            value_type,
            MapType<key_type, value_type>
        >
    >;
};

template<typename... Exprs>
struct LowerExprPack;

template<typename Done, typename... Exprs>
struct LowerExprPackImpl;

template<typename... Done>
struct LowerExprPackImpl<TypeList<Done...>> {
    using type = TypeList<Done...>;
};

template<bool Error, typename Current, typename Done, typename... Rest>
struct LowerExprPackNext;

template<typename Current, typename Done, typename... Rest>
struct LowerExprPackNext<true, Current, Done, Rest...> {
    using type = Current;
};

template<typename Current, typename... Done, typename... Rest>
struct LowerExprPackNext<false, Current, TypeList<Done...>, Rest...> {
    using type = typename LowerExprPackImpl<TypeList<Done..., Current>, Rest...>::type;
};

template<typename... Done, typename Expr, typename... Rest>
struct LowerExprPackImpl<TypeList<Done...>, Expr, Rest...> {
private:
    using current = typename LowerExpr<Expr>::type;

public:
    using type = typename LowerExprPackNext<
        IsReaderError<current>::value,
        current,
        TypeList<Done...>,
        Rest...
    >::type;
};

template<typename... Exprs>
struct LowerExprPack {
    using type = typename LowerExprPackImpl<TypeList<>, Exprs...>::type;
};

template<typename LoweredPack>
struct MakeBeginFromPack {
    using type = ReaderError<msg_reader_invalid_begin>;
};

template<typename Expr>
struct MakeBeginFromPack<TypeList<Expr>> {
    using type = Expr;
};

template<typename Expr, typename NextExpr, typename... RestExprs>
struct MakeBeginFromPack<TypeList<Expr, NextExpr, RestExprs...>> {
    using type = BeginExpr<Expr, NextExpr, RestExprs...>;
};

template<typename LoweredPack>
struct MakeCallFromPack {
    using type = ReaderError<msg_reader_invalid_application>;
};

template<typename FnExpr, typename... ArgExprs>
struct MakeCallFromPack<TypeList<FnExpr, ArgExprs...>> {
    using type = CallExpr<FnExpr, ArgExprs...>;
};

template<typename... Exprs>
struct LowerBody {
    using type = typename MakeBeginFromPack<typename LowerExprPack<Exprs...>::type>::type;
};

template<typename ParamsSExpr>
struct LowerParams {
    using type = ReaderError<msg_reader_invalid_lambda>;
};

template<typename ParamSExpr>
struct LowerParam {
    using type = ReaderError<msg_reader_invalid_param>;
};

template<typename Name>
struct LowerParam<SSymbol<Name>> {
    using type = Param<Symbol<Name>, AnyType>;
};

template<typename Name, typename TypeSExpr>
struct LowerParam<SList<SSymbol<Name>, TypeSExpr>> {
private:
    using lowered_type = typename LowerType<TypeSExpr>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered_type>::value,
        lowered_type,
        Param<Symbol<Name>, lowered_type>
    >;
};

template<>
struct LowerParams<SList<>> {
    using type = Params<>;
};

template<typename First, typename... Rest>
struct LowerParams<SList<First, Rest...>> {
private:
    using first = typename LowerParam<First>::type;
    using rest = typename LowerParams<SList<Rest...>>::type;

    template<typename FirstParam, typename RestParams>
    struct Combine;

    template<typename FirstParam, typename... RestParams>
    struct Combine<FirstParam, Params<RestParams...>> {
        using type = Params<FirstParam, RestParams...>;
    };

public:
    using type = IfType_t<
        IsReaderError<first>::value,
        first,
        IfType_t<
            IsReaderError<rest>::value,
            rest,
            typename Combine<first, rest>::type
        >
    >;
};

template<typename BindingsSExpr>
struct LowerBindings {
    using type = ReaderError<msg_reader_invalid_let>;
};

template<typename BindingSExpr>
struct LowerBinding {
    using type = ReaderError<msg_reader_invalid_binding>;
};

template<typename Name, typename ExprSExpr>
struct LowerBinding<SList<SSymbol<Name>, ExprSExpr>> {
private:
    using lowered_expr = typename LowerExpr<ExprSExpr>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered_expr>::value,
        lowered_expr,
        Binding<Symbol<Name>, lowered_expr>
    >;
};

template<>
struct LowerBindings<SList<>> {
    using type = Bindings<>;
};

template<typename First, typename... Rest>
struct LowerBindings<SList<First, Rest...>> {
private:
    using first = typename LowerBinding<First>::type;
    using rest = typename LowerBindings<SList<Rest...>>::type;

    template<typename FirstBinding, typename RestBindings>
    struct Combine;

    template<typename FirstBinding, typename... RestBindings>
    struct Combine<FirstBinding, Bindings<RestBindings...>> {
        using type = Bindings<FirstBinding, RestBindings...>;
    };

public:
    using type = IfType_t<
        IsReaderError<first>::value,
        first,
        IfType_t<
            IsReaderError<rest>::value,
            rest,
            typename Combine<first, rest>::type
        >
    >;
};

template<typename SExpr>
struct LowerExpr {
    using type = ReaderError<msg_reader_invalid_application>;
};

template<typename Name>
struct LowerExpr<SSymbol<Name>> {
private:
    using primitive = PrimitiveOf_t<Name>;

public:
    using type = IfType_t<
        IsSame<primitive, void>::value,
        Ref<Symbol<Name>>,
        primitive
    >;
};

template<int N>
struct LowerExpr<SInt<N>> {
    using type = Int<N>;
};

template<bool B>
struct LowerExpr<SBool<B>> {
    using type = Bool<B>;
};

template<char... Chars>
struct LowerExpr<SStringLit<Chars...>> {
    using type = String<Chars...>;
};

template<>
struct LowerExpr<SList<>> {
    using type = ReaderError<msg_reader_invalid_application>;
};

template<typename Cond, typename ThenExpr, typename ElseExpr>
struct LowerExpr<SList<SSymbol<String<'i', 'f'>>, Cond, ThenExpr, ElseExpr>> {
private:
    using lowered_cond = typename LowerExpr<Cond>::type;
    using lowered_then = typename LowerExpr<ThenExpr>::type;
    using lowered_else = typename LowerExpr<ElseExpr>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered_cond>::value,
        lowered_cond,
        IfType_t<
            IsReaderError<lowered_then>::value,
            lowered_then,
            IfType_t<
                IsReaderError<lowered_else>::value,
                lowered_else,
                IfExpr<lowered_cond, lowered_then, lowered_else>
            >
        >
    >;
};

template<typename... Parts>
struct LowerExpr<SList<SSymbol<String<'i', 'f'>>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_if>;
};

template<typename ParamsSExpr, typename BodyExpr, typename... RestBodyExprs>
struct LowerExpr<SList<SSymbol<String<'l', 'a', 'm', 'b', 'd', 'a'>>, ParamsSExpr, BodyExpr, RestBodyExprs...>> {
private:
    using params = typename LowerParams<ParamsSExpr>::type;
    using body = typename LowerBody<BodyExpr, RestBodyExprs...>::type;

public:
    using type = IfType_t<
        IsReaderError<params>::value,
        params,
        IfType_t<
            IsReaderError<body>::value,
            body,
            LambdaExpr<params, body>
        >
    >;
};

template<typename... Parts>
struct LowerExpr<SList<SSymbol<String<'l', 'a', 'm', 'b', 'd', 'a'>>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_lambda>;
};

template<typename BindingsSExpr, typename BodyExpr, typename... RestBodyExprs>
struct LowerExpr<SList<SSymbol<String<'l', 'e', 't'>>, BindingsSExpr, BodyExpr, RestBodyExprs...>> {
private:
    using bindings = typename LowerBindings<BindingsSExpr>::type;
    using body = typename LowerBody<BodyExpr, RestBodyExprs...>::type;

public:
    using type = IfType_t<
        IsReaderError<bindings>::value,
        bindings,
        IfType_t<
            IsReaderError<body>::value,
            body,
            LetExpr<bindings, body>
        >
    >;
};

template<typename... Parts>
struct LowerExpr<SList<SSymbol<String<'l', 'e', 't'>>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_let>;
};

template<typename Expr, typename... RestExprs>
struct LowerExpr<SList<SSymbol<String<'b', 'e', 'g', 'i', 'n'>>, Expr, RestExprs...>> {
    using type = typename LowerBody<Expr, RestExprs...>::type;
};

template<>
struct LowerExpr<SList<SSymbol<String<'b', 'e', 'g', 'i', 'n'>>>> {
    using type = ReaderError<msg_reader_invalid_begin>;
};

template<typename Expr>
struct LowerExpr<SList<SSymbol<String<'q', 'u', 'o', 't', 'e'>>, Expr>> {
private:
    using quoted = typename QuoteValue<Expr>::type;

public:
    using type = IfType_t<
        IsReaderError<quoted>::value,
        quoted,
        Quote<quoted>
    >;
};

template<typename... Parts>
struct LowerExpr<SList<SSymbol<String<'q', 'u', 'o', 't', 'e'>>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_quote>;
};

template<typename Head, typename... Tail>
struct LowerExpr<SList<Head, Tail...>> {
private:
    using lowered = typename LowerExprPack<Head, Tail...>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered>::value,
        lowered,
        typename MakeCallFromPack<lowered>::type
    >;
};

template<typename ParsedProgram>
struct ReadProgramDispatch {
    using type = typename LowerExpr<ParsedProgram>::type;
};

template<typename Message>
struct ReadProgramDispatch<ReaderError<Message>> {
    using type = ReaderError<Message>;
};

template<typename Program>
struct ReadProgram {
private:
    using parsed = ParseProgram_t<Program>;

public:
    using type = typename ReadProgramDispatch<parsed>::type;
};

template<typename Program>
struct ReadEval {
private:
    using parsed = typename ReadProgram<Program>::type;

public:
    using type = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        EvalLisp_t<parsed>
    >;
};

template<typename Program>
struct ReadTypeCheck {
private:
    using parsed = typename ReadProgram<Program>::type;

public:
    using type = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        TypeCheck_t<parsed>
    >;
};

} // namespace reader_detail

template<typename Program>
using ReadProgram_t = typename reader_detail::ReadProgram<Program>::type;

template<typename Program>
using ReadEval_t = typename reader_detail::ReadEval<Program>::type;

template<typename Program>
using ReadTypeCheck_t = typename reader_detail::ReadTypeCheck<Program>::type;

template<std::size_t N>
struct fixed_string {
    char value[N];

    constexpr fixed_string(const char (&input)[N]) : value{} {
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = input[i];
        }
    }

    static constexpr std::size_t size = N - 1;
};

template<fixed_string Source, typename Indices>
struct literal_string_impl;

template<fixed_string Source, std::size_t... Indices>
struct literal_string_impl<Source, std::index_sequence<Indices...>> {
    using type = String<Source.value[Indices]...>;
};

template<fixed_string Source>
using literal_string_t = typename literal_string_impl<Source, std::make_index_sequence<Source.size>>::type;

template<fixed_string Source>
using ReadSource_t = ReadProgram_t<literal_string_t<Source>>;

template<fixed_string Source>
using ReadSourceEval_t = ReadEval_t<literal_string_t<Source>>;

template<fixed_string Source>
using ReadSourceTypeCheck_t = ReadTypeCheck_t<literal_string_t<Source>>;

} // namespace lc
