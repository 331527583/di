//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/scopable.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/unique.hpp"

namespace boost { namespace di { inline namespace v1 { namespace concepts {

test none = [] {
    struct none_scopable { };
    static_expect(!scopable<none_scopable>::value);
};

struct scope_type {
    template<class, class>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        static T try_create(const TProvider&);

        template<class T, class TProvider>
        T create(const TProvider&);
    };
};

test scopable_type = [] {
    static_expect(scopable<scope_type>::value);
};

class scope_private_access {
    template<class, class>
    struct scope {
        template<class T, class TProvider>
        T create(const TProvider&);

        template<class T, class TProvider>
        T try_create(const TProvider&);
    };
};

#if !defined(_MSC_VER)
    test private_access = [] {
        static_expect(!scopable<scope_private_access>::value);
    };
#endif

class scope_missing_create {
public:
    template<class, class>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        static T try_create(const TProvider&);
    };
};

test missing_create = [] {
    static_expect(!scopable<scope_missing_create>::value);
};

class scope_missing_try_create {
public:
    template<class, class>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        T create(const TProvider&);
    };
};

test missing_try_create = [] {
    static_expect(!scopable<scope_missing_try_create>::value);
};

class scope_missing_is_referable {
public:
    template<class, class>
    struct scope {
        template<class T, class TProvider>
        static T try_create(const TProvider&);

        template<class T, class TProvider>
        T create(const TProvider&);
    };
};

test missing_is_referable = [] {
    static_expect(!scopable<scope_missing_is_referable>::value);
};

test scopable_scopes = [] {
    static_expect(scopable<scopes::deduce>::value);
    static_expect(scopable<scopes::exposed<>>::value);
    static_expect(scopable<scopes::external>::value);
    static_expect(scopable<scopes::singleton>::value);
    static_expect(scopable<scopes::unique>::value);
};

}}}} // boost::di::v1::concepts

