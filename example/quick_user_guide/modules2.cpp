//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };

struct c {
    c(std::shared_ptr<i1> i1
    , std::shared_ptr<i2> i2
    , int i) : i1_(i1), i2_(i2), i(i)
    { }

    std::shared_ptr<i1> i1_;
    std::shared_ptr<i2> i2_;
    int i;
};

struct module {
    di::injector<c> configure() const noexcept;
    int i;
};

di::injector<c> module::configure() const noexcept {
    return di::make_injector(
        di::bind<i1, impl1>()
      , di::bind<i2, impl2>()
      , di::bind<int>().to(i)
    );
}

int main() {
    auto injector = di::make_injector(
        module{42}
    );

    auto object = injector.create<c>();
    assert(dynamic_cast<impl1*>(object.i1_.get()));
    assert(dynamic_cast<impl2*>(object.i2_.get()));
    assert(42 == object.i);

    //injector.create<std::unique_ptr<i1>>(); // compile error
    //injector.create<std::unique_ptr<i2>>(); // compile error
}

