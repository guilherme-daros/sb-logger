#pragma once

#include <concepts>

namespace sb::logger::meta {

template <typename Base, auto Predicate, typename... Types>
struct TypeFinder;

template <auto F, typename T>
concept PredicateLambda = requires {
  { F.template operator()<T>() } -> std::convertible_to<bool>;
};

template <auto F, typename T>
concept PredicateIsTrue = PredicateLambda<F, T> && F.template operator()<T>();

template <auto F, typename T>
concept PredicateIsFalse = PredicateLambda<F, T> && !F.template operator()<T>();

template <typename Base, auto Predicate, typename First, typename... Rest>
  requires PredicateIsTrue<Predicate, First>
struct TypeFinder<Base, Predicate, First, Rest...> {
  using type = First;
};

template <typename Base, auto Predicate, typename First, typename... Rest>
  requires PredicateIsFalse<Predicate, First>
struct TypeFinder<Base, Predicate, First, Rest...> {
  using type = typename TypeFinder<Base, Predicate, Rest...>::type;
};

template <typename Base, auto Predicate>
struct TypeFinder<Base, Predicate> {
  using type = typename Base::Default;
};

template <typename Base, auto Predicate, typename... Types>
using TypeFinder_t = typename TypeFinder<Base, Predicate, Types...>::type;

template <auto Predicate, typename... Types>
struct AnyOf;

template <auto Predicate, typename First, typename... Rest>
  requires PredicateIsTrue<Predicate, First>
struct AnyOf<Predicate, First, Rest...> {
  static constexpr bool value = true;
};

template <auto Predicate, typename First, typename... Rest>
  requires PredicateIsFalse<Predicate, First>
struct AnyOf<Predicate, First, Rest...> {
  static constexpr bool value = AnyOf<Predicate, Rest...>::value;
};

template <auto Predicate>
struct AnyOf<Predicate> {
  static constexpr bool value = false;
};

template <auto Predicate, typename... Types>
static constexpr bool any_of = AnyOf<Predicate, Types...>::value;

}  // namespace sb::logger::meta
