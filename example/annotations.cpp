//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[annotations
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

auto int_1 = []{};
auto int_2 = []{};

class annotations {
public:
    /*<<Constructor with named parameters of the same `int` type>>*/
    BOOST_DI_INJECT(annotations, (named = int_1) int i1, (named = int_2) int i2, int i3)
        : i1(i1), i2(i2), i3(i3) {
        assert(i1 == 42);
        assert(i2 == 87);
        assert(i3 == 123);
    }

private:
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
};

int main() {
    /*<<make injector and bind named parameters>>*/
    auto injector = di::make_injector(
        di::bind<int>().named(int_1).to(42)
      , di::bind<int>().named(int_2).to(87)
      , di::bind<int>().to(123)
    );

    /*<<create `annotations`>>*/
    injector.create<annotations>();

    return 0;
}

//]

