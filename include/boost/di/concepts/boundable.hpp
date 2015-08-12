//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BOUNDABLE_HPP
#define BOOST_DI_CONCEPTS_BOUNDABLE_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/bindings.hpp"

namespace boost { namespace di { inline namespace v1 { namespace concepts {

#define BOOST_DI_BOUNDABLE_ERROR \
    static_assert(aux::constraint_not_satisfied<T>::value, "boundable constraint not satisfied")

template<class T, class...>
struct bind {
    template<class TName>
    struct named {
        struct is_bound_more_than_once  { BOOST_DI_BOUNDABLE_ERROR; };
    };
    struct is_bound_more_than_once  { BOOST_DI_BOUNDABLE_ERROR; };
    struct is_neither_a_dependency_nor_an_injector { BOOST_DI_BOUNDABLE_ERROR; };
    struct has_disallowed_specifiers { BOOST_DI_BOUNDABLE_ERROR; };
    template<class> struct is_not_related_to { BOOST_DI_BOUNDABLE_ERROR; };
};

template<class T, class...>
struct any_of { BOOST_DI_BOUNDABLE_ERROR; };

#undef BOOST_DI_BOUNDABLE_ERROR

template<class... TDeps>
struct is_supported : std::is_same<
   aux::bool_list<aux::always<TDeps>::value...>
 , aux::bool_list<(core::is_injector<TDeps>::value || core::is_dependency<TDeps>::value)...>
> { };

template<class...>
struct get_not_supported;

template<class T>
struct get_not_supported<T> {
    using type = T;
};

template<class T, class... TDeps>
struct get_not_supported<T, TDeps...>
    : std::conditional<
          core::is_injector<T>::value || core::is_dependency<T>::value
        , typename get_not_supported<TDeps...>::type
        , T
      >
{ };

template<class>
struct is_unique;

template<class T>
struct unique_dependency {
    using type = aux::pair<
        aux::pair<typename T::expected, typename T::name>
      , typename T::priority
    >;
};

template<class... TDeps>
struct is_unique<aux::type_list<TDeps...>>
    : aux::is_unique<typename unique_dependency<TDeps>::type...>
{ };

template<class>
struct get_is_unique_error_impl
    : std::true_type
{ };

template<class T, class TName, class TPriority>
struct get_is_unique_error_impl<aux::not_unique<aux::pair<aux::pair<T, TName>, TPriority>>> {
    using type = typename bind<T>::template named<TName>::is_bound_more_than_once;
};

template<class T, class TPriority>
struct get_is_unique_error_impl<aux::not_unique<aux::pair<aux::pair<T, no_name>, TPriority>>> {
    using type = typename bind<T>::is_bound_more_than_once;
};

template<class T>
struct get_is_unique_error_impl<aux::not_unique<T>> {
    using type = typename bind<T>::is_bound_more_than_once;
};

template<class>
struct get_is_unique_error;

template<class... TDeps>
struct get_is_unique_error<aux::type_list<TDeps...>>
    : get_is_unique_error_impl<typename aux::is_unique<typename unique_dependency<TDeps>::type...>::type>
{ };

template<class... TDeps>
using get_bindings_error =
    std::conditional_t<
        is_supported<TDeps...>::value
      , typename get_is_unique_error<core::bindings_t<TDeps...>>::type
      , typename bind<typename get_not_supported<TDeps...>::type>::is_neither_a_dependency_nor_an_injector
    >;

template<class... Ts>
using get_any_of_error =
    std::conditional_t<
        std::is_same<
            aux::bool_list<aux::always<Ts>::value...>
          , aux::bool_list<std::is_same<std::true_type, Ts>::value...>
        >::value
      , std::true_type
      , any_of<Ts...>
    >;

template<class I, class T> // expected -> given
auto boundable_impl(I&&, T&&) ->
    std::conditional_t<
        !std::is_same<I, aux::remove_specifiers_t<I>>::value
      , typename bind<I>::has_disallowed_specifiers
      , std::conditional_t<
            !std::is_same<T, aux::remove_specifiers_t<T>>::value
          , typename bind<T>::has_disallowed_specifiers
          , std::conditional_t<
                std::is_base_of<I, T>::value || std::is_convertible<T, I>::value
              , std::true_type
              , typename bind<T>::template is_not_related_to<I>
            >
        >
    >;

template<class... TDeps> // bindings
auto boundable_impl(aux::type_list<TDeps...>&&) -> get_bindings_error<TDeps...>;

template<class T, class... Ts> // any_of
auto boundable_impl(aux::type_list<Ts...>&&, T&&) ->
    get_any_of_error<decltype(boundable_impl(std::declval<Ts>(), std::declval<T>()))...>;

template<class... TDeps> // make_injector
auto boundable_impl(aux::type<TDeps...>&&) ->
    typename get_is_unique_error_impl<typename aux::is_unique<TDeps...>::type>::type;

std::true_type boundable_impl(...);

template<class... Ts>
struct boundable__ {
    using type = decltype(boundable_impl(std::declval<Ts>()...));
};

template<class... Ts>
using boundable = typename boundable__<Ts...>::type;

}}}} // boost::di::v1::concepts

#endif

