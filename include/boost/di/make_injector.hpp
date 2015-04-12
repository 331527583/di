//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/core/injector.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<
     class TConfig = ::BOOST_DI_CFG
   , class... TDeps
   , BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type_list<TDeps...>>)
   //, BOOST_DI_REQUIRES_MSG(concepts::configurable<TConfig>)
> inline auto make_injector(const TDeps&... args) noexcept {
    return core::injector<TConfig, TDeps...>{core::init{}, args...};
}

}} // boost::di

#endif

