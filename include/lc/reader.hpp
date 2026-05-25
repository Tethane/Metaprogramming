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

template<typename Frame, typename Inner>
struct IsReaderError<ErrorContext<Frame, Inner>> : IsReaderError<Inner> {};

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
using msg_reader_invalid_define = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'd', 'e', 'f', 'i', 'n', 'e'>;
using msg_reader_invalid_cond = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'c', 'o', 'n', 'd'>;
using msg_reader_missing_else = String<'m', 'i', 's', 's', 'i', 'n', 'g', '-', 'e', 'l', 's', 'e'>;
using msg_reader_unknown_type = String<'u', 'n', 'k', 'n', 'o', 'w', 'n', '-', 't', 'y', 'p', 'e'>;
using msg_reader_invalid_number = String<'i', 'n', 'v', 'a', 'l', 'i', 'd', '-', 'n', 'u', 'm', 'b', 'e', 'r'>;

template<typename Expr, typename Rest>
struct Parsed {
    using expr = Expr;
    using rest = Rest;
};

template<typename Name>
struct SSymbol {};

template<typename Token>
struct SNumberLit {};

template<char... Chars>
struct SStringLit {};

template<bool B>
struct SBool {};

template<typename... Items>
struct SList {};

template<typename... Forms>
struct SProgram {};

namespace reader_detail {

using define_label = String<'d', 'e', 'f', 'i', 'n', 'e'>;
using cond_label = String<'c', 'o', 'n', 'd'>;
using list_label = String<'l', 'i', 's', 't'>;
using if_label = String<'i', 'f'>;
using lambda_label = String<'l', 'a', 'm', 'b', 'd', 'a'>;
using let_label = String<'l', 'e', 't'>;
using begin_label = String<'b', 'e', 'g', 'i', 'n'>;
using quote_label = String<'q', 'u', 'o', 't', 'e'>;
using else_label = String<'e', 'l', 's', 'e'>;
using cons_label = String<'c', 'o', 'n', 's'>;
using nil_label = String<'n', 'i', 'l'>;
using none_label = String<'n', 'o', 'n', 'e'>;
using infer_label = String<'I', 'n', 'f', 'e', 'r'>;

template<typename Label>
using ReaderFrame = ErrorFrame<Label>;

template<typename Label, typename Inner>
using ReaderContext = ErrorContext<ReaderFrame<Label>, Inner>;

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

template<typename Acc, typename Str>
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
    using type = Parsed<SList<SSymbol<quote_label>, Expr>, Rest>;
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
    using type = typename ParseNumberBody<String<'-', Next>, String<Rest...>>::type;
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
    using type = typename ParseNumberBody<String<C>, String<Rest...>>::type;
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

template<typename Acc>
struct ParseNumberBody<Acc, String<>> {
    using type = Parsed<SNumberLit<Acc>, String<>>;
};

template<bool Delimited, typename Acc, typename Str>
struct ParseNumberDispatch;

template<typename Acc, char C, char... Rest>
struct ParseNumberDispatch<true, Acc, String<C, Rest...>> {
    using type = Parsed<SNumberLit<Acc>, String<C, Rest...>>;
};

template<typename Acc, char C, char... Rest>
struct ParseNumberDispatch<false, Acc, String<C, Rest...>> {
    using type = typename ParseNumberBody<StringPushBack_t<Acc, C>, String<Rest...>>::type;
};

template<typename Acc, char C, char... Rest>
struct ParseNumberBody<Acc, String<C, Rest...>> {
    using type = typename ParseNumberDispatch<IsDelimiter<C>::value, Acc, String<C, Rest...>>::type;
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

template<typename Acc, typename Str>
struct ParseTopLevelForms;

template<typename... Forms>
struct ParseTopLevelForms<TypeList<Forms...>, String<>> {
    using type = SProgram<Forms...>;
};

template<typename Acc, typename Str>
struct ParseTopLevelDispatch;

template<typename Acc, typename Trimmed>
struct ParseTopLevelAfterTrim;

template<typename... Forms>
struct ParseTopLevelAfterTrim<TypeList<Forms...>, String<>> {
    using type = SProgram<Forms...>;
};

template<typename Acc, char C, char... Rest>
struct ParseTopLevelAfterTrim<Acc, String<C, Rest...>> {
    using type = typename ParseTopLevelDispatch<Acc, String<C, Rest...>>::type;
};

template<typename... Forms, char C, char... Rest>
struct ParseTopLevelDispatch<TypeList<Forms...>, String<C, Rest...>> {
private:
    using parsed = typename ParseExpr<String<C, Rest...>>::type;

    template<typename ParsedT>
    struct Next {
        using type = ParsedT;
    };

    template<typename Expr, typename Tail>
    struct Next<Parsed<Expr, Tail>> {
        using type = typename ParseTopLevelForms<TypeList<Forms..., Expr>, Tail>::type;
    };

public:
    using type = typename Next<parsed>::type;
};

template<typename... Forms, typename Str>
struct ParseTopLevelForms<TypeList<Forms...>, Str> {
private:
    using trimmed = SkipWhitespace_t<Str>;

public:
    using type = typename ParseTopLevelAfterTrim<TypeList<Forms...>, trimmed>::type;
};

template<typename Str>
using ParseProgram_t = typename ParseTopLevelForms<TypeList<>, Str>::type;

template<typename SExpr>
struct ExpandExpr {
    using type = SExpr;
};

template<typename SExpr>
struct ExpandTopLevelForm {
    using type = typename ExpandExpr<SExpr>::type;
};

template<typename... Forms>
struct ExpandProgram;

template<>
struct ExpandProgram<> {
    using type = SProgram<>;
};

template<typename Head, typename... Tail>
struct ExpandProgram<Head, Tail...> {
private:
    using expanded_head = typename ExpandTopLevelForm<Head>::type;
    using expanded_tail = typename ExpandProgram<Tail...>::type;

    template<typename HeadT, typename TailProgram>
    struct Combine;

    template<typename HeadT, typename... TailForms>
    struct Combine<HeadT, SProgram<TailForms...>> {
        using type = SProgram<HeadT, TailForms...>;
    };

public:
    using type = IfType_t<
        IsReaderError<expanded_head>::value,
        expanded_head,
        typename Combine<expanded_head, expanded_tail>::type
    >;
};

template<typename SExprs>
struct ExpandProgramDispatch {
    using type = ReaderContext<String<'p', 'r', 'o', 'g', 'r', 'a', 'm'>, ReaderError<msg_invalid_program>>;
};

template<typename Message>
struct ExpandProgramDispatch<ReaderError<Message>> {
    using type = ReaderError<Message>;
};

template<typename Frame, typename Inner>
struct ExpandProgramDispatch<ErrorContext<Frame, Inner>> {
    using type = ErrorContext<Frame, Inner>;
};

template<typename... Forms>
struct ExpandProgramDispatch<SProgram<Forms...>> {
    using type = typename ExpandProgram<Forms...>::type;
};

template<typename... Exprs>
struct ExpandExprPack;

template<typename Done, typename... Exprs>
struct ExpandExprPackImpl;

template<typename... Done>
struct ExpandExprPackImpl<TypeList<Done...>> {
    using type = TypeList<Done...>;
};

template<bool Error, typename Current, typename Done, typename... Rest>
struct ExpandExprPackNext;

template<typename Current, typename Done, typename... Rest>
struct ExpandExprPackNext<true, Current, Done, Rest...> {
    using type = Current;
};

template<typename Current, typename... Done, typename... Rest>
struct ExpandExprPackNext<false, Current, TypeList<Done...>, Rest...> {
    using type = typename ExpandExprPackImpl<TypeList<Done..., Current>, Rest...>::type;
};

template<typename... Done, typename Expr, typename... Rest>
struct ExpandExprPackImpl<TypeList<Done...>, Expr, Rest...> {
private:
    using current = typename ExpandExpr<Expr>::type;

public:
    using type = typename ExpandExprPackNext<
        IsReaderError<current>::value,
        current,
        TypeList<Done...>,
        Rest...
    >::type;
};

template<typename... Exprs>
struct ExpandExprPack {
    using type = typename ExpandExprPackImpl<TypeList<>, Exprs...>::type;
};

template<typename ExprPack>
struct RebuildSList;

template<typename... Exprs>
struct RebuildSList<TypeList<Exprs...>> {
    using type = SList<Exprs...>;
};

template<typename Expr>
struct MakeBeginSExpr {
    using type = Expr;
};

template<typename Expr, typename NextExpr, typename... RestExprs>
struct MakeBeginSExpr<SList<Expr, NextExpr, RestExprs...>> {
    using type = SList<SSymbol<begin_label>, Expr, NextExpr, RestExprs...>;
};

template<typename ExprPack>
struct MakeExpandedBody;

template<typename Expr>
struct MakeExpandedBody<TypeList<Expr>> {
    using type = Expr;
};

template<typename Expr, typename NextExpr, typename... RestExprs>
struct MakeExpandedBody<TypeList<Expr, NextExpr, RestExprs...>> {
    using type = SList<SSymbol<begin_label>, Expr, NextExpr, RestExprs...>;
};

template<typename... Items>
struct ExpandExpr<SList<Items...>> {
private:
    using pack = typename ExpandExprPack<Items...>::type;

public:
    using type = IfType_t<
        IsReaderError<pack>::value,
        pack,
        typename RebuildSList<pack>::type
    >;
};

template<typename Expr>
struct ExpandExpr<SList<SSymbol<quote_label>, Expr>> {
    using type = SList<SSymbol<quote_label>, Expr>;
};

template<typename... Parts>
struct ExpandExpr<SList<SSymbol<define_label>, Parts...>> {
    using type = ReaderContext<define_label, ReaderError<msg_reader_invalid_define>>;
};

template<typename Cond, typename ThenExpr, typename ElseExpr>
struct ExpandExpr<SList<SSymbol<if_label>, Cond, ThenExpr, ElseExpr>> {
private:
    using expanded_cond = typename ExpandExpr<Cond>::type;
    using expanded_then = typename ExpandExpr<ThenExpr>::type;
    using expanded_else = typename ExpandExpr<ElseExpr>::type;

public:
    using type = IfType_t<
        IsReaderError<expanded_cond>::value,
        expanded_cond,
        IfType_t<
            IsReaderError<expanded_then>::value,
            expanded_then,
            IfType_t<
                IsReaderError<expanded_else>::value,
                expanded_else,
                SList<SSymbol<if_label>, expanded_cond, expanded_then, expanded_else>
            >
        >
    >;
};

template<typename... Parts>
struct ExpandExpr<SList<SSymbol<if_label>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_if>;
};

template<typename ParamsSExpr, typename BodyExpr, typename... RestBodyExprs>
struct ExpandExpr<SList<SSymbol<lambda_label>, ParamsSExpr, BodyExpr, RestBodyExprs...>> {
private:
    using body_pack = typename ExpandExprPack<BodyExpr, RestBodyExprs...>::type;

public:
    using type = IfType_t<
        IsReaderError<body_pack>::value,
        body_pack,
        SList<SSymbol<lambda_label>, ParamsSExpr, typename MakeExpandedBody<body_pack>::type>
    >;
};

template<typename... Parts>
struct ExpandExpr<SList<SSymbol<lambda_label>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_lambda>;
};

template<typename BindingSExpr>
struct ExpandBinding {
    using type = ReaderError<msg_reader_invalid_binding>;
};

template<typename Name, typename Expr>
struct ExpandBinding<SList<SSymbol<Name>, Expr>> {
private:
    using expanded_expr = typename ExpandExpr<Expr>::type;

public:
    using type = IfType_t<
        IsReaderError<expanded_expr>::value,
        expanded_expr,
        SList<SSymbol<Name>, expanded_expr>
    >;
};

template<typename BindingsSExpr>
struct ExpandBindings {
    using type = ReaderError<msg_reader_invalid_let>;
};

template<>
struct ExpandBindings<SList<>> {
    using type = SList<>;
};

template<typename First, typename... Rest>
struct ExpandBindings<SList<First, Rest...>> {
private:
    using first = typename ExpandBinding<First>::type;
    using rest = typename ExpandBindings<SList<Rest...>>::type;

    template<typename FirstBinding, typename RestBindings>
    struct Combine;

    template<typename FirstBinding, typename... RestBindings>
    struct Combine<FirstBinding, SList<RestBindings...>> {
        using type = SList<FirstBinding, RestBindings...>;
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

template<typename BindingsSExpr, typename BodyExpr, typename... RestBodyExprs>
struct ExpandExpr<SList<SSymbol<let_label>, BindingsSExpr, BodyExpr, RestBodyExprs...>> {
private:
    using expanded_bindings = typename ExpandBindings<BindingsSExpr>::type;
    using body_pack = typename ExpandExprPack<BodyExpr, RestBodyExprs...>::type;

public:
    using type = IfType_t<
        IsReaderError<expanded_bindings>::value,
        expanded_bindings,
        IfType_t<
            IsReaderError<body_pack>::value,
            body_pack,
            SList<SSymbol<let_label>, expanded_bindings, typename MakeExpandedBody<body_pack>::type>
        >
    >;
};

template<typename... Parts>
struct ExpandExpr<SList<SSymbol<let_label>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_let>;
};

template<typename Expr, typename... RestExprs>
struct ExpandExpr<SList<SSymbol<begin_label>, Expr, RestExprs...>> {
private:
    using pack = typename ExpandExprPack<Expr, RestExprs...>::type;

public:
    using type = IfType_t<
        IsReaderError<pack>::value,
        pack,
        typename MakeExpandedBody<pack>::type
    >;
};

template<>
struct ExpandExpr<SList<SSymbol<begin_label>>> {
    using type = ReaderError<msg_reader_invalid_begin>;
};

template<typename TailExpr>
struct ExpandListExpr {
    using type = SList<SSymbol<quote_label>, SList<>>;
};

template<typename HeadExpr, typename... TailExprs>
struct ExpandListExpr<SList<HeadExpr, TailExprs...>> {
private:
    using expanded_head = typename ExpandExpr<HeadExpr>::type;
    using expanded_tail = typename ExpandListExpr<SList<TailExprs...>>::type;

public:
    using type = IfType_t<
        IsReaderError<expanded_head>::value,
        expanded_head,
        IfType_t<
            IsReaderError<expanded_tail>::value,
            expanded_tail,
            SList<SSymbol<cons_label>, expanded_head, expanded_tail>
        >
    >;
};

template<typename... Items>
struct ExpandExpr<SList<SSymbol<list_label>, Items...>> {
    using type = typename ExpandListExpr<SList<Items...>>::type;
};

template<typename Clause>
struct ExpandCondClause;

template<typename Test, typename Expr, typename... RestExprs>
struct ExpandCondClause<SList<Test, Expr, RestExprs...>> {
private:
    using expanded_test = typename ExpandExpr<Test>::type;
    using body_pack = typename ExpandExprPack<Expr, RestExprs...>::type;

public:
    using test = expanded_test;
    using body = IfType_t<
        IsReaderError<body_pack>::value,
        body_pack,
        typename MakeExpandedBody<body_pack>::type
    >;
};

template<typename... Clauses>
struct ExpandCondClauses;

template<>
struct ExpandCondClauses<> {
    using type = ReaderContext<cond_label, ReaderError<msg_reader_missing_else>>;
};

template<typename Expr, typename... RestExprs>
struct ExpandCondClauses<SList<SSymbol<else_label>, Expr, RestExprs...>> {
private:
    using body_pack = typename ExpandExprPack<Expr, RestExprs...>::type;

public:
    using type = IfType_t<
        IsReaderError<body_pack>::value,
        body_pack,
        typename MakeExpandedBody<body_pack>::type
    >;
};

template<typename... RestClauses>
struct ExpandCondClauses<SList<SSymbol<else_label>>, RestClauses...> {
    using type = ReaderContext<cond_label, ReaderError<msg_reader_invalid_cond>>;
};

template<typename Test, typename Expr, typename... RestExprs, typename... RestClauses>
struct ExpandCondClauses<SList<Test, Expr, RestExprs...>, RestClauses...> {
private:
    using clause = ExpandCondClause<SList<Test, Expr, RestExprs...>>;
    using next = typename ExpandCondClauses<RestClauses...>::type;

public:
    using type = IfType_t<
        IsReaderError<typename clause::test>::value,
        typename clause::test,
        IfType_t<
            IsReaderError<typename clause::body>::value,
            typename clause::body,
            IfType_t<
                IsReaderError<next>::value,
                next,
                SList<SSymbol<if_label>, typename clause::test, typename clause::body, next>
            >
        >
    >;
};

template<typename... Clauses>
struct ExpandExpr<SList<SSymbol<cond_label>, Clauses...>> {
    using type = typename ExpandCondClauses<Clauses...>::type;
};

template<>
struct ExpandExpr<SList<SSymbol<cond_label>>> {
    using type = ReaderContext<cond_label, ReaderError<msg_reader_invalid_cond>>;
};

template<typename Name, typename Expr>
struct ExpandTopLevelForm<SList<SSymbol<define_label>, SSymbol<Name>, Expr>> {
private:
    using expanded_expr = typename ExpandExpr<Expr>::type;

public:
    using type = IfType_t<
        IsReaderError<expanded_expr>::value,
        expanded_expr,
        SList<SSymbol<define_label>, SSymbol<Name>, expanded_expr>
    >;
};

template<typename Name, typename... ParamsT, typename BodyExpr, typename... RestBodyExprs>
struct ExpandTopLevelForm<SList<SSymbol<define_label>, SList<SSymbol<Name>, ParamsT...>, BodyExpr, RestBodyExprs...>> {
private:
    using body_pack = typename ExpandExprPack<BodyExpr, RestBodyExprs...>::type;
    using lambda_expr = IfType_t<
        IsReaderError<body_pack>::value,
        body_pack,
        SList<SSymbol<lambda_label>, SList<ParamsT...>, typename MakeExpandedBody<body_pack>::type>
    >;

public:
    using type = IfType_t<
        IsReaderError<lambda_expr>::value,
        lambda_expr,
        SList<SSymbol<define_label>, SSymbol<Name>, lambda_expr>
    >;
};

template<typename... Parts>
struct ExpandTopLevelForm<SList<SSymbol<define_label>, Parts...>> {
    using type = ReaderContext<define_label, ReaderError<msg_reader_invalid_define>>;
};

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
template<> struct PrimitiveOf<String<'a', 'b', 's'> > { using type = Abs; };
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
template<> struct PrimitiveOf<String<'c', 'o', 'n', 's'> > { using type = Cons; };
template<> struct PrimitiveOf<String<'h', 'e', 'a', 'd'> > { using type = Head; };
template<> struct PrimitiveOf<String<'t', 'a', 'i', 'l'> > { using type = Tail; };
template<> struct PrimitiveOf<String<'e', 'm', 'p', 't', 'y', '?'> > { using type = IsEmpty; };
template<> struct PrimitiveOf<String<'c', 'o', 'n', 'c', 'a', 't'> > { using type = Concat; };
template<> struct PrimitiveOf<String<'r', 'e', 'v', 'e', 'r', 's', 'e'> > { using type = Reverse; };
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
template<> struct PrimitiveOf<String<'s', 'q', 'r', 't'> > { using type = Sqrt; };
template<> struct PrimitiveOf<String<'e', 'x', 'p'> > { using type = Exp; };
template<> struct PrimitiveOf<String<'l', 'o', 'g'> > { using type = Log; };
template<> struct PrimitiveOf<String<'s', 'i', 'n'> > { using type = Sin; };
template<> struct PrimitiveOf<String<'c', 'o', 's'> > { using type = Cos; };
template<> struct PrimitiveOf<String<'t', 'a', 'n'> > { using type = Tan; };
template<> struct PrimitiveOf<String<'a', 's', 'i', 'n'> > { using type = Asin; };
template<> struct PrimitiveOf<String<'a', 'c', 'o', 's'> > { using type = Acos; };
template<> struct PrimitiveOf<String<'a', 't', 'a', 'n'> > { using type = Atan; };
template<> struct PrimitiveOf<String<'v', 'e', 'c', 't', 'o', 'r'> > { using type = VectorCtor; };
template<> struct PrimitiveOf<String<'m', 'a', 't', 'r', 'i', 'x'> > { using type = MatrixCtor; };
template<> struct PrimitiveOf<String<'c', 'o', 'm', 'p', 'l', 'e', 'x'> > { using type = ComplexCtor; };
template<> struct PrimitiveOf<String<'v', 'e', 'c', '-', 'a', 'd', 'd'> > { using type = VecAdd; };
template<> struct PrimitiveOf<String<'v', 'e', 'c', '-', 's', 'u', 'b'> > { using type = VecSub; };
template<> struct PrimitiveOf<String<'v', 'e', 'c', '-', 's', 'c', 'a', 'l', 'e'> > { using type = VecScale; };
template<> struct PrimitiveOf<String<'d', 'o', 't'> > { using type = Dot; };
template<> struct PrimitiveOf<String<'n', 'o', 'r', 'm'> > { using type = Norm; };
template<> struct PrimitiveOf<String<'n', 'o', 'r', 'm', 'a', 'l', 'i', 'z', 'e'> > { using type = NormalizeVector; };
template<> struct PrimitiveOf<String<'m', 'a', 't', '-', 'a', 'd', 'd'> > { using type = MatAdd; };
template<> struct PrimitiveOf<String<'m', 'a', 't', '-', 's', 'u', 'b'> > { using type = MatSub; };
template<> struct PrimitiveOf<String<'m', 'a', 't', '-', 's', 'c', 'a', 'l', 'e'> > { using type = MatScale; };
template<> struct PrimitiveOf<String<'m', 'a', 't', '-', 'v', 'e', 'c', '-', 'm', 'u', 'l'> > { using type = MatVecMul; };
template<> struct PrimitiveOf<String<'m', 'a', 't', '-', 'm', 'u', 'l'> > { using type = MatMul; };
template<> struct PrimitiveOf<String<'t', 'r', 'a', 'n', 's', 'p', 'o', 's', 'e'> > { using type = Transpose; };
template<> struct PrimitiveOf<String<'d', 'e', 't'> > { using type = Determinant; };
template<> struct PrimitiveOf<String<'i', 'n', 'v', 'e', 'r', 's', 'e'> > { using type = Inverse; };
template<> struct PrimitiveOf<String<'c', 'o', 'n', 'j', 'u', 'g', 'a', 't', 'e'> > { using type = Conjugate; };
template<> struct PrimitiveOf<String<'n', 'o', 'r', 'm', '-', 's', 'q', 'u', 'a', 'r', 'e', 'd'> > { using type = NormSquared; };
template<> struct PrimitiveOf<String<'m', 'a', 'g', 'n', 'i', 't', 'u', 'd', 'e'> > { using type = Magnitude; };
template<> struct PrimitiveOf<String<'a', 'r', 'g', 'u', 'm', 'e', 'n', 't'> > { using type = Argument; };
template<> struct PrimitiveOf<String<'m', 'e', 'a', 'n'> > { using type = Mean; };
template<> struct PrimitiveOf<String<'m', 'e', 'd', 'i', 'a', 'n'> > { using type = Median; };
template<> struct PrimitiveOf<String<'m', 'o', 'd', 'e'> > { using type = Mode; };
template<> struct PrimitiveOf<String<'v', 'a', 'r', 'i', 'a', 'n', 'c', 'e'> > { using type = Variance; };
template<> struct PrimitiveOf<String<'s', 't', 'd', 'd', 'e', 'v'> > { using type = StdDev; };
template<> struct PrimitiveOf<String<'m', 'i', 'n'> > { using type = Minimum; };
template<> struct PrimitiveOf<String<'m', 'a', 'x'> > { using type = Maximum; };
template<> struct PrimitiveOf<String<'r', 'a', 'n', 'g', 'e'> > { using type = StatRange; };
template<> struct PrimitiveOf<String<'c', 'o', 'v', 'a', 'r', 'i', 'a', 'n', 'c', 'e'> > { using type = Covariance; };
template<> struct PrimitiveOf<String<'c', 'o', 'r', 'r', 'e', 'l', 'a', 't', 'i', 'o', 'n'> > { using type = Correlation; };

template<typename Name>
using PrimitiveOf_t = typename PrimitiveOf<Name>::type;

template<typename Token>
struct LowerNumberLiteral {
    using type = ReaderError<msg_reader_invalid_number>;
};

template<bool Fits, typename Token>
struct LowerIntegerLiteralImpl;

template<char... Chars>
struct LowerNumberLiteral<String<Chars...>> {
private:
    inline static constexpr char token[sizeof...(Chars)] = {Chars...};
    inline static constexpr std::size_t size = sizeof...(Chars);

    static consteval std::size_t find_slash() {
        std::size_t found = size;
        for (std::size_t i = 0; i < size; ++i) {
            if (token[i] == '/') {
                if (found != size) {
                    return size + 1;
                }
                found = i;
            }
        }
        return found;
    }

    static consteval bool all_digits(std::size_t begin, std::size_t end) {
        if (begin >= end) {
            return false;
        }
        for (std::size_t i = begin; i < end; ++i) {
            if (token[i] < '0' || token[i] > '9') {
                return false;
            }
        }
        return true;
    }

    static consteval bool valid_integer(std::size_t begin, std::size_t end, bool allow_sign) {
        if (begin >= end) {
            return false;
        }
        std::size_t cursor = begin;
        if (allow_sign && (token[cursor] == '-' || token[cursor] == '+')) {
            ++cursor;
        }
        return all_digits(cursor, end);
    }

    static consteval bool fits_int() {
        if (!valid_integer(0, size, true)) {
            return false;
        }

        long long value = 0;
        bool negative = false;
        std::size_t cursor = 0;
        if (token[cursor] == '-' || token[cursor] == '+') {
            negative = token[cursor] == '-';
            ++cursor;
        }
        for (; cursor < size; ++cursor) {
            value = value * 10 + (token[cursor] - '0');
            if (!negative && value > 2147483647LL) {
                return false;
            }
            if (negative && value > 2147483648LL) {
                return false;
            }
        }
        return true;
    }

    static consteval int parse_int() {
        long long value = 0;
        bool negative = false;
        std::size_t cursor = 0;
        if (token[cursor] == '-' || token[cursor] == '+') {
            negative = token[cursor] == '-';
            ++cursor;
        }
        for (; cursor < size; ++cursor) {
            value = value * 10 + (token[cursor] - '0');
        }
        return static_cast<int>(negative ? -value : value);
    }

    static consteval bigint_storage parse_bigint_slice(std::size_t begin, std::size_t end, bool allow_sign) {
        char buffer[bigint_capacity] = {};
        std::size_t written = 0;
        if (allow_sign && begin < end && (token[begin] == '-' || token[begin] == '+')) {
            buffer[written++] = token[begin++];
        }
        for (; begin < end; ++begin) {
            buffer[written++] = token[begin];
        }
        return detail::make_bigint_storage_from_chars(buffer, written);
    }

    inline static constexpr std::size_t slash = find_slash();
    inline static constexpr bool is_integer_token = slash == size;
    inline static constexpr bool integer_ok = is_integer_token && valid_integer(0, size, true);
    inline static constexpr bool rational_ok =
        slash < size &&
        slash + 1 < size &&
        valid_integer(0, slash, true) &&
        valid_integer(slash + 1, size, false);
    inline static constexpr auto numerator_storage =
        rational_ok ? parse_bigint_slice(0, slash, true) : detail::make_bigint_storage_from_int(0);
    inline static constexpr auto denominator_storage =
        rational_ok ? parse_bigint_slice(slash + 1, size, false) : detail::make_bigint_storage_from_int(1);
    using integer_type = typename LowerIntegerLiteralImpl<fits_int(), String<Chars...>>::type;
    using rational_type =
        detail::NormalizeRational_t<BigInt<numerator_storage>, BigInt<denominator_storage>>;

public:
    using type = IfType_t<
        integer_ok,
        integer_type,
        IfType_t<
            rational_ok && !detail::bigint_is_zero(denominator_storage),
            rational_type,
            ReaderError<msg_reader_invalid_number>
        >
    >;
};

template<char... Chars>
struct LowerIntegerLiteralImpl<true, String<Chars...>> {
private:
    inline static constexpr char token[sizeof...(Chars)] = {Chars...};
    inline static constexpr std::size_t size = sizeof...(Chars);

    static consteval int parse_int() {
        long long value = 0;
        bool negative = false;
        std::size_t cursor = 0;
        if (token[cursor] == '-' || token[cursor] == '+') {
            negative = token[cursor] == '-';
            ++cursor;
        }
        for (; cursor < size; ++cursor) {
            value = value * 10 + (token[cursor] - '0');
        }
        return static_cast<int>(negative ? -value : value);
    }

public:
    using type = Int<parse_int()>;
};

template<char... Chars>
struct LowerIntegerLiteralImpl<false, String<Chars...>> {
private:
    inline static constexpr char token[sizeof...(Chars)] = {Chars...};
    inline static constexpr std::size_t size = sizeof...(Chars);

public:
    using type = BigInt<detail::make_bigint_storage_from_chars(token, size)>;
};

template<typename SExpr>
struct QuoteValue {
    using type = ReaderError<msg_reader_invalid_quote>;
};

template<typename Name>
struct QuoteValue<SSymbol<Name>> {
    using type = Symbol<Name>;
};

template<typename Token>
struct QuoteValue<SNumberLit<Token>> {
    using type = typename LowerNumberLiteral<Token>::type;
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

template<typename Frame, typename Inner, template<typename> class Container>
struct WrapLoweredType<ErrorContext<Frame, Inner>, Container> {
    using type = ErrorContext<Frame, Inner>;
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
struct LowerType<SSymbol<String<'B', 'i', 'g', 'I', 'n', 't'>>> {
    using type = BigIntType;
};

template<>
struct LowerType<SSymbol<String<'R', 'a', 't', 'i', 'o', 'n', 'a', 'l'>>> {
    using type = RationalType;
};

template<>
struct LowerType<SSymbol<String<'D', 'e', 'c', 'i', 'm', 'a', 'l'>>> {
    using type = DecimalType;
};

template<>
struct LowerType<SSymbol<String<'R', 'e', 'a', 'l'>>> {
    using type = RealType;
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
struct LowerType<SSymbol<infer_label>> {
    using type = InferType;
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
    using type = Param<Symbol<Name>, InferType>;
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
        IsSame<Name, String<'p', 'i'>>::value,
        Irrational<pi_tag>,
        IfType_t<
            IsSame<Name, String<'e'>>::value,
            Irrational<e_tag>,
            IfType_t<
                IsSame<Name, String<'t', 'a', 'u'>>::value,
                Irrational<tau_tag>,
                IfType_t<
        IsSame<Name, nil_label>::value,
        List<>,
        IfType_t<
            IsSame<Name, none_label>::value,
            None,
            IfType_t<
                IsSame<primitive, void>::value,
                Ref<Symbol<Name>>,
                primitive
            >
        >
                >
            >
        >
    >;
};

template<typename Token>
struct LowerExpr<SNumberLit<Token>> {
    using type = typename LowerNumberLiteral<Token>::type;
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
struct LowerExpr<SList<SSymbol<if_label>, Cond, ThenExpr, ElseExpr>> {
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
struct LowerExpr<SList<SSymbol<if_label>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_if>;
};

template<typename ParamsSExpr, typename BodyExpr>
struct LowerExpr<SList<SSymbol<lambda_label>, ParamsSExpr, BodyExpr>> {
private:
    using params = typename LowerParams<ParamsSExpr>::type;
    using body = typename LowerExpr<BodyExpr>::type;

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
struct LowerExpr<SList<SSymbol<lambda_label>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_lambda>;
};

template<typename BindingsSExpr, typename BodyExpr>
struct LowerExpr<SList<SSymbol<let_label>, BindingsSExpr, BodyExpr>> {
private:
    using bindings = typename LowerBindings<BindingsSExpr>::type;
    using body = typename LowerExpr<BodyExpr>::type;

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
struct LowerExpr<SList<SSymbol<let_label>, Parts...>> {
    using type = ReaderError<msg_reader_invalid_let>;
};

template<typename Expr, typename... RestExprs>
struct LowerExpr<SList<SSymbol<begin_label>, Expr, RestExprs...>> {
    using type = typename LowerBody<Expr, RestExprs...>::type;
};

template<>
struct LowerExpr<SList<SSymbol<begin_label>>> {
    using type = ReaderError<msg_reader_invalid_begin>;
};

template<typename Expr>
struct LowerExpr<SList<SSymbol<quote_label>, Expr>> {
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
struct LowerExpr<SList<SSymbol<quote_label>, Parts...>> {
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

template<typename Form>
struct LowerTopLevelForm {
private:
    using lowered_expr = typename LowerExpr<Form>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered_expr>::value,
        lowered_expr,
        ExprForm<lowered_expr>
    >;
};

template<typename Name, typename Expr>
struct LowerTopLevelForm<SList<SSymbol<define_label>, SSymbol<Name>, Expr>> {
private:
    using lowered_expr = typename LowerExpr<Expr>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered_expr>::value,
        lowered_expr,
        DefineForm<Symbol<Name>, lowered_expr>
    >;
};

template<typename Done, typename... Forms>
struct LowerTopLevelFormsImpl;

template<typename... Done>
struct LowerTopLevelFormsImpl<TypeList<Done...>> {
    using type = TypeList<Done...>;
};

template<bool Error, typename Current, typename Done, typename... Rest>
struct LowerTopLevelFormsNext;

template<typename Current, typename Done, typename... Rest>
struct LowerTopLevelFormsNext<true, Current, Done, Rest...> {
    using type = Current;
};

template<typename Current, typename... Done, typename... Rest>
struct LowerTopLevelFormsNext<false, Current, TypeList<Done...>, Rest...> {
    using type = typename LowerTopLevelFormsImpl<TypeList<Done..., Current>, Rest...>::type;
};

template<typename... Done, typename Form, typename... Rest>
struct LowerTopLevelFormsImpl<TypeList<Done...>, Form, Rest...> {
private:
    using current = typename LowerTopLevelForm<Form>::type;

public:
    using type = typename LowerTopLevelFormsNext<
        IsReaderError<current>::value,
        current,
        TypeList<Done...>,
        Rest...
    >::type;
};

template<typename LoweredForms>
struct LowerProgramResult {
    using type = ReaderError<msg_invalid_program>;
};

template<typename Expr>
struct LowerProgramResult<TypeList<ExprForm<Expr>>> {
    using type = Expr;
};

template<typename Name, typename Expr>
struct LowerProgramResult<TypeList<DefineForm<Name, Expr>>> {
    using type = ProgramExpr<DefineForm<Name, Expr>>;
};

template<typename... Forms>
struct LowerProgramResult<TypeList<Forms...>> {
    using type = ProgramExpr<Forms...>;
};

template<typename ExpandedProgram>
struct LowerExpandedProgram {
    using type = ReaderError<msg_invalid_program>;
};

template<typename... Forms>
struct LowerExpandedProgram<SProgram<Forms...>> {
private:
    using lowered_forms = typename LowerTopLevelFormsImpl<TypeList<>, Forms...>::type;

public:
    using type = IfType_t<
        IsReaderError<lowered_forms>::value,
        lowered_forms,
        typename LowerProgramResult<lowered_forms>::type
    >;
};

template<typename ParsedProgram>
struct ReadProgramDispatch {
    using expanded = typename ExpandProgramDispatch<ParsedProgram>::type;
    using type = IfType_t<
        IsReaderError<expanded>::value,
        expanded,
        typename LowerExpandedProgram<expanded>::type
    >;
};

template<typename Program>
struct ReadProgram {
private:
    using parsed = ParseProgram_t<Program>;

public:
    using type = typename ReadProgramDispatch<parsed>::type;
};

template<typename Program>
struct ExpandSourceProgram {
private:
    using parsed = ParseProgram_t<Program>;

public:
    using type = typename ExpandProgramDispatch<parsed>::type;
};

template<typename Program>
struct ReadEval {
private:
    using parsed = typename ReadProgram<Program>::type;
    using optimized = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        OptimizeLisp_t<parsed>
    >;

public:
    using type = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        EvalLisp_t<optimized>
    >;
};

template<typename Program>
struct ReadTypeCheck {
private:
    using parsed = typename ReadProgram<Program>::type;
    using optimized = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        OptimizeLisp_t<parsed>
    >;
    using checked = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        TypeCheck_t<optimized>
    >;

public:
    using type = IfType_t<
        IsReaderError<parsed>::value,
        parsed,
        FinalizeInferredType_t<checked>
    >;
};

} // namespace reader_detail

template<typename Program>
using ReadProgram_t = typename reader_detail::ReadProgram<Program>::type;

template<typename Program>
using ReadEval_t = typename reader_detail::ReadEval<Program>::type;

template<typename Program>
using ReadTypeCheck_t = typename reader_detail::ReadTypeCheck<Program>::type;

template<typename Program>
using ExpandProgram_t = typename reader_detail::ExpandSourceProgram<Program>::type;

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

template<fixed_string Source>
using ExpandSource_t = ExpandProgram_t<literal_string_t<Source>>;

template<fixed_string Source>
using ReadScript_t = ReadSource_t<Source>;

template<fixed_string Source>
using EvalScript_t = ReadSourceEval_t<Source>;

template<fixed_string Source>
using TypeCheckScript_t = ReadSourceTypeCheck_t<Source>;

template<fixed_string Source>
using ExpandScript_t = ExpandSource_t<Source>;

template<fixed_string Source, int Digits = 18>
using ReadScriptApprox_t = Approx_t<EvalScript_t<Source>, Digits>;

template<fixed_string Source>
using EvalScriptWithStats_t = ComputationStats<
    EvalScript_t<Source>,
    0,
    TermNodeCount<OptimizeLisp_t<ReadScript_t<Source>>>::value,
    0
>;

} // namespace lc
