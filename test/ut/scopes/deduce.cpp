//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/deduce.hpp"
#include "common/fakes/fake_provider.hpp"
#include "common/fakes/fake_scope.hpp"

namespace boost { namespace di { inline namespace v1 {

struct c { };

namespace type_traits {
template<>
struct scope_traits<c> {
    using type = fake_scope<>;
};
} // type_traits

namespace scopes {

test create = [] {
    expect_eq(0, static_cast<int>(deduce::scope<int, int>{}.create<int>(fake_provider<int>{})));
};

test create_from_scope = [] {
    fake_scope<>::calls() = 0;
    c c_ = deduce::scope<c, c>{}.create<c>(fake_provider<c>{});
    (void)c_;
    expect_eq(1, fake_scope<>::calls());
};

}}}} // boost::di::v1::scopes

