//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/wrappers/unique.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

class unique {
public:
    template<class, class>
    class scope {
    public:
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        auto try_create(const TProvider& provider) const -> decltype(
            wrappers::unique<decltype(provider.try_get(type_traits::memory_traits_t<T>{}))>{
                provider.try_get(type_traits::memory_traits_t<T>{})
            }
        );

        template<class T, class TProvider>
        auto create(const TProvider& provider) const {
            using memory = type_traits::memory_traits_t<T>;
            using wrapper = wrappers::unique<decltype(provider.get(memory{}))>;
            return wrapper{provider.get(memory{})};
        }
    };
};

}}}} // boost::di::v1::scopes

#endif

