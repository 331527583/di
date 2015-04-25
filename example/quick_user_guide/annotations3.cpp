//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;

auto int1 = []{};
auto int2 = []{};

struct c {
    BOOST_DI_INJECT(c
        , (named = int1) int a
        , (named = int2) int b);

    int a = 0;
    int b = 0;
};

c::c(int a, int b) : a(a), b(b) { }

int main() {
    auto injector = di::make_injector(
        di::bind<int>.named(int1).to(42)
      , di::bind<int>.named(int2).to(87)
    );

    auto object = injector.create<c>();
    assert(42 == object.a);
    assert(87 == object.b);
}

