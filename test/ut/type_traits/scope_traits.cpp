//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <type_traits>
#include "boost/di/type_traits/scope_traits.hpp"
#if __has_include(<boost/shared_ptr.hpp>)
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

struct c { };

template<class T>
struct deleter {
    void operator()(T* ptr) const noexcept {
        delete ptr;
    }
};

test traits = [] {
    static_expect(std::is_same<scopes::unique, scope_traits<int>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<c>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<const int&>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<int*>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<const int*>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<std::shared_ptr<int>>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<const std::shared_ptr<int>&>::type>{});
#if __has_include(<boost/shared_ptr.hpp>)
    static_expect(std::is_same<scopes::singleton, scope_traits<boost::shared_ptr<int>>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<const boost::shared_ptr<int>&>::type>{});
#endif
    static_expect(std::is_same<scopes::singleton, scope_traits<std::weak_ptr<int>>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<const std::weak_ptr<int>&>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<int&>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<std::unique_ptr<int>>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<std::unique_ptr<int, deleter<int>>>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<std::shared_ptr<int>>::type>{});
    static_expect(std::is_same<scopes::singleton, scope_traits<std::weak_ptr<int>>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<int&&>::type>{});
    static_expect(std::is_same<scopes::unique, scope_traits<const int&&>::type>{});
};

}}}} // boost::di::v1::type_traits

