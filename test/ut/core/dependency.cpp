//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/dependency.hpp"
#include "common/fakes/fake_injector.hpp"
#include "common/fakes/fake_scope.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

test is_dependency_types = [] {
    expect(!is_dependency<void>{});
    expect(!is_dependency<int>{});
    expect(is_dependency<dependency<scopes::deduce, int>>{});
    expect(is_dependency<dependency<scopes::deduce, double, double>>{});
};

struct name { };

test types = [] {
    using dep = dependency<fake_scope<>, int, double, name>;
    expect(std::is_same<fake_scope<>, typename dep::scope>{});
    expect(std::is_same<int, typename dep::expected>{});
    expect(std::is_same<double, typename dep::given>{});
    expect(std::is_same<name, typename dep::name>{});
};

test def_ctor = [] {
    dependency<scopes::deduce, int> dep;
    (void)dep;
};

test ctor = [] {
    fake_scope<>::ctor_calls() = 0;
    dependency<fake_scope<>, int> dep{0};
    expect(1 == fake_scope<>::ctor_calls());
};

test named = [] {
    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<no_name, typename dep1::name>{});

    using dep2 = decltype(dep1{}.named(name{}));
    expect(std::is_same<name, typename dep2::name>{});
};

test in = [] {
    using dep1 = dependency<fake_scope<>, int>;
    expect(std::is_same<fake_scope<>, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.in(scopes::deduce{}));
    expect(std::is_same<scopes::deduce, typename dep2::scope>{});
};

test to = [] {
    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<scopes::deduce, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.to(42));
    expect(std::is_same<scopes::external, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<int, typename dep2::given>{});

    expect(std::is_same<scopes::external, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<int, typename dep2::given>{});
};

test to_with_configure = [] {
    struct module {
        fake_injector<> configure() const noexcept { return {}; }
    };

    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<scopes::deduce, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.to(module{}));
    expect(std::is_same<scopes::exposed<scopes::deduce>, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<fake_injector<>, typename dep2::given>{});
};

test to_with_deps = [] {
    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<scopes::deduce, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.to(fake_injector<>{}));
    expect(std::is_same<scopes::exposed<scopes::deduce>, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<fake_injector<>, typename dep2::given>{});
};

}}}} // boost::di::v1::core

