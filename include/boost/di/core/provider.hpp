//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class, class, class, class>
struct try_provider;

template<
    class TGiven
  , class TInjector
  , class TProvider
  , class TInitialization
  , class... TCtor
> struct try_provider<TGiven, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector, TProvider> {
    template<class TMemory>
    struct is_creatable {
        static constexpr auto value =
            TProvider::template is_creatable<
                TInitialization
              , TMemory
              , TGiven
              , typename injector__<TInjector>::template try_create<TCtor>::type...
            >::value;
    };

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const -> std::enable_if_t<
        is_creatable<TMemory>::value
      , std::conditional_t<std::is_same<TMemory, type_traits::stack>::value, TGiven, std::remove_reference_t<TGiven>*>
    >;
};

template<class, class, class, class, class>
struct provider;

template<
    class TExpected
  , class TGiven
  , class TName
  , class TInjector
  , class TInitialization
  , class... TCtor
> struct provider<TExpected, TGiven, TName, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector> {
    using provider_t = decltype(TInjector::config::provider(std::declval<TInjector>()));

    template<class TMemory, class... TArgs>
    struct is_creatable {
        static constexpr auto value =
            provider_t::template is_creatable<TInitialization, TMemory, TGiven, TArgs...>::value;
    };

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return get_impl(memory, static_cast<const injector__<TInjector>&>(injector_).create_impl(aux::type<TCtor>{})...);
    }

    template<class TMemory, class... TArgs, BOOST_DI_REQUIRES(is_creatable<TMemory, TArgs...>::value) = 0>
    auto get_impl(const TMemory& memory, TArgs&&... args) const {
        return TInjector::config::provider(injector_).template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , static_cast<TArgs&&>(args)...
        );
    }

    template<class TMemory, class... TArgs, BOOST_DI_REQUIRES(!is_creatable<TMemory, TArgs...>::value) = 0>
    auto get_impl(const TMemory&, TArgs&&...) const {
        return concepts::creatable_error<TInitialization, TName, TExpected*, TGiven*, TArgs...>();
    }

    const TInjector& injector_;
};

namespace successful {

template<class, class, class, class>
struct provider;

template<
    class TExpected
  , class TGiven
  , class TInjector
  , class TInitialization
  , class... TCtor
> struct provider<TExpected, TGiven, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector> {
    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return TInjector::config::provider(injector_).template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , static_cast<const injector__<TInjector>&>(injector_).create_successful_impl(aux::type<TCtor>{})...
        );
    }

    const TInjector& injector_;
};

} // successful

}}}} // boost::di::v1::core

#endif

