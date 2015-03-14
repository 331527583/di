//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_HEAP_HPP
#define BOOST_DI_PROVIDERS_HEAP_HPP

#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { namespace providers {

class heap {
public:
    template<class, class T, class TMemory, class... TArgs
           , REQUIRES<concepts::creatable<type_traits::direct, T, TArgs...>()> = 0>
    auto get(const type_traits::direct&
           , const TMemory&
           , TArgs&&... args) const {
        return new T(std::forward<TArgs>(args)...);
    }

    template<class, class T, class TMemory, class... TArgs
           , REQUIRES<concepts::creatable<type_traits::uniform, T, TArgs...>()> = 0>
    auto get(const type_traits::uniform&
           , const TMemory&
           , TArgs&&... args) const {
        return new T{std::forward<TArgs>(args)...};
    }

    template<class, class T, class TInitialization, class TMemory, class... TArgs
           , REQUIRES<!concepts::creatable<TInitialization, T, TArgs...>()> = 0>
    auto get(const TInitialization&, const TMemory&, TArgs&&...) const noexcept {
        return concepts::creatable_error<TInitialization, T*, TArgs...>();
    }
};

}}} // boost::di::providers

#endif

