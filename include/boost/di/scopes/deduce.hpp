//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include "boost/di/type_traits/scope_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

class deduce {
public:
    template<class TExpected, class TGiven>
    class scope {
    public:
        template<class T>
        using is_referable = typename type_traits::scope_traits_t<T>::template
            scope<TExpected, TGiven>::template is_referable<T>;

        template<class T, class TProvider>
        static decltype(typename type_traits::scope_traits_t<T>::template
            scope<TExpected, TGiven>{}.template try_create<T>(std::declval<TProvider>()))
        try_create(const TProvider&);

        template<class T, class TProvider>
        auto create(const TProvider& provider) {
            using scope_traits = type_traits::scope_traits_t<T>;
            using scope = typename scope_traits::template scope<TExpected, TGiven>;
            return scope{}.template create<T>(provider);
        }
    };
};

}}}} // boost::di::v1::scopes

#endif

