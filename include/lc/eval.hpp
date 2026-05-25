#pragma once

#include "intrinsics.hpp"

namespace lc {

template<int D, int Cutoff, typename Term>
struct Shift {
    using type = Term;
};

template<int D, int Cutoff, int N>
struct Shift<D, Cutoff, Var<N>> {
    using type = IfType_t<(N >= Cutoff), Var<N + D>, Var<N>>;
};

template<int D, int Cutoff, typename Body>
struct Shift<D, Cutoff, Lambda<Body>> {
    using type = Lambda<typename Shift<D, Cutoff + 1, Body>::type>;
};

template<int D, int Cutoff, typename F, typename A>
struct Shift<D, Cutoff, App<F, A>> {
    using type = Apply_t<typename Shift<D, Cutoff, F>::type, typename Shift<D, Cutoff, A>::type>;
};

template<int D, int Cutoff, typename Head, typename... Args>
struct Shift<D, Cutoff, Call<Head, Args...>> {
    using type = Apply_t<typename Shift<D, Cutoff, Head>::type, typename Shift<D, Cutoff, Args>::type...>;
};

template<int J, typename Replacement, typename Term>
struct Subst {
    using type = Term;
};

template<int J, typename Replacement, int N>
struct Subst<J, Replacement, Var<N>> {
    using type = IfType_t<(N == J), Replacement, Var<N>>;
};

template<int J, typename Replacement, typename Body>
struct Subst<J, Replacement, Lambda<Body>> {
    using type = Lambda<typename Subst<J + 1, Shift_t<1, Replacement>, Body>::type>;
};

template<int J, typename Replacement, typename F, typename A>
struct Subst<J, Replacement, App<F, A>> {
    using type = Apply_t<typename Subst<J, Replacement, F>::type, typename Subst<J, Replacement, A>::type>;
};

template<int J, typename Replacement, typename Head, typename... Args>
struct Subst<J, Replacement, Call<Head, Args...>> {
    using type = Apply_t<typename Subst<J, Replacement, Head>::type, typename Subst<J, Replacement, Args>::type...>;
};

template<typename Term>
struct Step {
    using type = ReductionResult<Term, false>;
};

template<int N>
struct Step<Var<N>> {
    using type = ReductionResult<Var<N>, false>;
};

template<typename Name>
struct Step<Free<Name>> {
    using type = ReductionResult<Free<Name>, false>;
};

template<typename Body>
struct Step<Lambda<Body>> {
private:
    using body_step = typename Step<Canonicalize_t<Body>>::type;

public:
    using type = ReductionResult<Lambda<typename body_step::term>, body_step::changed>;
};

template<typename F, typename A>
struct Step<App<F, A>> {
    using type = typename Step<Canonicalize_t<App<F, A>>>::type;
};

template<typename HeadT, typename DoneList, typename... Remaining>
struct StepArgs;

template<bool Changed, typename HeadT, typename DoneList, typename CurrentStep, typename Current, typename... Remaining>
struct StepArgsAfterCurrent;

template<typename HeadT, typename... Done>
struct StepArgs<HeadT, TypeList<Done...>> {
    using type = ReductionResult<Apply_t<HeadT, Done...>, false>;
};

template<typename HeadT, typename... Done, typename CurrentStep, typename Current, typename... Remaining>
struct StepArgsAfterCurrent<true, HeadT, TypeList<Done...>, CurrentStep, Current, Remaining...> {
    using type = ReductionResult<Apply_t<HeadT, Done..., typename CurrentStep::term, Remaining...>, true>;
};

template<typename HeadT, typename... Done, typename CurrentStep, typename Current, typename... Remaining>
struct StepArgsAfterCurrent<false, HeadT, TypeList<Done...>, CurrentStep, Current, Remaining...> {
    using type = typename StepArgs<HeadT, TypeList<Done..., Current>, Remaining...>::type;
};

template<typename HeadT, typename... Done, typename Current, typename... Remaining>
struct StepArgs<HeadT, TypeList<Done...>, Current, Remaining...> {
private:
    using current_step = typename Step<Canonicalize_t<Current>>::type;

public:
    using type = typename StepArgsAfterCurrent<
        current_step::changed,
        HeadT,
        TypeList<Done...>,
        current_step,
        Current,
        Remaining...
    >::type;
};

template<bool Changed, typename HeadStep, typename... Args>
struct StepCallFallbackAfterHead;

template<typename HeadStep, typename... Args>
struct StepCallFallbackAfterHead<true, HeadStep, Args...> {
    using type = ReductionResult<Apply_t<typename HeadStep::term, Args...>, true>;
};

template<typename HeadStep, typename... Args>
struct StepCallFallbackAfterHead<false, HeadStep, Args...> {
    using type = typename StepArgs<typename HeadStep::term, TypeList<>, Args...>::type;
};

template<typename HeadT, typename... Args>
struct StepCallFallback {
private:
    using head_step = typename Step<Canonicalize_t<HeadT>>::type;

public:
    using type = typename StepCallFallbackAfterHead<head_step::changed, head_step, Args...>::type;
};

template<bool Changed, typename IntrinsicStepT, typename HeadT, typename... Args>
struct StepCallAfterIntrinsic;

template<typename IntrinsicStepT, typename HeadT, typename... Args>
struct StepCallAfterIntrinsic<true, IntrinsicStepT, HeadT, Args...> {
    using type = IntrinsicStepT;
};

template<typename IntrinsicStepT, typename HeadT, typename... Args>
struct StepCallAfterIntrinsic<false, IntrinsicStepT, HeadT, Args...> {
    using type = typename StepCallFallback<HeadT, Args...>::type;
};

template<typename Body, typename Arg, typename... Rest>
struct Step<Call<Lambda<Body>, Arg, Rest...>> {
private:
    using reduced = Shift_t<-1, Subst_t<0, Shift_t<1, Arg>, Body>>;

public:
    using type = ReductionResult<Apply_t<reduced, Rest...>, true>;
};

template<typename HeadT, typename... Args>
struct Step<Call<HeadT, Args...>> {
private:
    using intrinsic = typename detail::IntrinsicStep<Call<HeadT, Args...>>::type;

public:
    using type = typename StepCallAfterIntrinsic<intrinsic::changed, intrinsic, HeadT, Args...>::type;
};

template<bool Changed, typename StepResultT, int Fuel>
struct NormalizeAfterStep;

template<typename StepResultT, int Fuel>
struct NormalizeAfterStep<true, StepResultT, Fuel> {
    using type = typename Normalize<typename StepResultT::term, Fuel - 1>::type;
};

template<typename StepResultT, int Fuel>
struct NormalizeAfterStep<false, StepResultT, Fuel> {
    using type = typename StepResultT::term;
};

template<typename Term, int Fuel>
struct Normalize {
private:
    using current = Canonicalize_t<Term>;
    using step = typename Step<current>::type;

public:
    using type = typename NormalizeAfterStep<step::changed, step, Fuel>::type;
};

template<typename Term>
struct Normalize<Term, 0> {
    using type = OutOfFuel<Canonicalize_t<Term>>;
};

template<bool SeenBefore, bool Changed, typename Current, typename StepResultT, int Fuel, typename Seen>
struct NormalizeCheckedAfterState;

template<bool Changed, typename Current, typename StepResultT, int Fuel, typename Seen>
struct NormalizeCheckedAfterState<true, Changed, Current, StepResultT, Fuel, Seen> {
    using type = CycleDetected<Current>;
};

template<typename Current, typename StepResultT, int Fuel, typename Seen>
struct NormalizeCheckedAfterState<false, true, Current, StepResultT, Fuel, Seen> {
    using type = typename NormalizeChecked<typename StepResultT::term, Fuel - 1, PushFront_t<Current, Seen>>::type;
};

template<typename Current, typename StepResultT, int Fuel, typename Seen>
struct NormalizeCheckedAfterState<false, false, Current, StepResultT, Fuel, Seen> {
    using type = Current;
};

template<typename Term, int Fuel, typename Seen>
struct NormalizeChecked {
private:
    using current = Canonicalize_t<Term>;
    using step = typename Step<current>::type;

public:
    using type = typename NormalizeCheckedAfterState<
        Contains<current, Seen>::value,
        step::changed,
        current,
        step,
        Fuel,
        Seen
    >::type;
};

template<typename Term, typename Seen>
struct NormalizeChecked<Term, 0, Seen> {
    using type = OutOfFuel<Canonicalize_t<Term>>;
};

} // namespace lc
