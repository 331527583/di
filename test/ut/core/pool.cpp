//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include <memory>
#include "boost/di/core/pool.hpp"

namespace boost { namespace di { namespace core {

template<typename T>
struct allocator {
    allocator(T* object) // non explicit
        : object(object)
    { }

    std::shared_ptr<T> object;
};

struct trivial_ctor {
    trivial_ctor()
        : i(0)
    { }

    int i = 0;
};

struct default_ctor {
    default_ctor(int i = 0)
        : i(i)
    { }

    int i = 0;
};

struct custom_ctor {
    explicit custom_ctor(int i)
        : i(i)
    { }

    int i = 0;
};

struct custom_ctor_other
{
    explicit custom_ctor_other(int i)
        : i(i)
    { }

    int i = 0;
};

test empty = [] {
    using pool_t = pool<aux::type_list<>>;
    pool_t p;
    (void)p;
};

test ctor_order_reverse = [] {
    using pool_t = pool<
        aux::type_list<
            trivial_ctor
          , default_ctor
        >
    >;

    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_t p(trivial_ctor_, default_ctor_);
    (void)p;
};

test default_ctor_base = [] {
    using pool_t = pool<
        aux::type_list<
            trivial_ctor
          , default_ctor
        >
    >;

    pool_t p;
    (void)p;
};

test get = [] {
    using custom_ctor_type = allocator<custom_ctor>;
    using trivial_ctor_type = allocator<trivial_ctor>;
    using default_ctor_type = allocator<default_ctor>;
    using pool_t = pool<
        aux::type_list<
            custom_ctor_type
          , trivial_ctor_type
          , default_ctor_type
        >
    >;

    custom_ctor_type custom_ctor_(new custom_ctor{0});
    trivial_ctor_type trivial_ctor_(new trivial_ctor);
    default_ctor_type default_ctor_(new default_ctor);

    pool_t p(custom_ctor_, trivial_ctor_, default_ctor_);

    expect_eq(
        trivial_ctor_.object
      , p.get<trivial_ctor_type>().object
    );

    expect_eq(
        custom_ctor_.object
      , p.get<custom_ctor_type>().object
    );

    expect_eq(
        default_ctor_.object
      , p.get<default_ctor_type>().object
    );
};

test pool_of_pools = [] {
    using trivial_ctor_type = allocator<trivial_ctor>;
    using default_ctor_type = allocator<default_ctor>;
    using p1_type = pool<aux::type_list<default_ctor_type>>;
    using p2_type = pool<aux::type_list<trivial_ctor_type>>;
    using pool_t = pool<aux::type_list<p1_type, p2_type>>;
    default_ctor_type default_ctor_(new default_ctor);
    trivial_ctor_type trivial_ctor_(new trivial_ctor);

    p1_type p1_(default_ctor_);
    p2_type p2_(trivial_ctor_);

    pool_t p(p1_, p2_);

    expect_eq(
        trivial_ctor_.object
      , p.get<trivial_ctor_type>().object
    );

    expect_eq(
        default_ctor_.object
      , p.get<default_ctor_type>().object
    );
};

test init_pool_from_other_empty_pool = [] {
    pool<aux::type_list<>> pempty_;
    pool<aux::type_list<default_ctor>> p(init{}, pempty_);

    expect_eq(0, p.get<default_ctor>().i);
};

test init_pfrom_other_subset_pool = [] {
    using pool_all_t = pool<
        aux::type_list<
            trivial_ctor
          , default_ctor
          , custom_ctor
        >
    >;

    using pool_subset_t = pool<
        aux::type_list<
            trivial_ctor
          , custom_ctor
        >
    >;

    trivial_ctor trivial_ctor_;
    custom_ctor custom_ctor_(42);

    pool_subset_t p1(trivial_ctor_, custom_ctor_);
    pool_all_t p2(init{}, p1);

    expect_eq(
        trivial_ctor_.i
      , p2.get<trivial_ctor>().i
    );

    expect_eq(
        0
      , p2.get<default_ctor>().i
    );

    expect_eq(
        custom_ctor_.i
      , p2.get<custom_ctor>().i
    );
};

test pool_from_pof_pools = [] {
    using pool_t = pool<aux::type_list<custom_ctor>>;
    using pool_sub_t = pool<aux::type_list<pool_t>>;
    constexpr auto i = 42;

    custom_ctor ctor(i);
    pool_t p1(ctor);
    pool_sub_t p2(p1);

    expect_eq(i, p2.get<custom_ctor>().i);
};

test pool_from_pof_pools_many = [] {
    using pool_t = pool<aux::type_list<custom_ctor, default_ctor>>;
    using pool_sub_t = pool<aux::type_list<pool_t>> ;
    constexpr auto i = 42;

    custom_ctor c(i);
    default_ctor d;
    pool_t p1(c, d);
    pool_sub_t p2(p1);

    expect_eq(i, p2.get<custom_ctor>().i);
    expect_eq(0, p2.get<default_ctor>().i);
};

template<typename T1, typename T2>
struct base : T1, T2 { };

test pool_flatten = [] {
    using pool_flatten_t = pool<aux::type_list<trivial_ctor, custom_ctor, default_ctor>>;
    using pool_t = pool<aux::type_list<custom_ctor, base<trivial_ctor, default_ctor>>>;
    constexpr auto i = 42;

    custom_ctor c(i);
    default_ctor d;
    trivial_ctor t;
    base<trivial_ctor, default_ctor> b;

    pool_t p1(c, b);
    pool_flatten_t p2(init{}, p1);

    expect_eq(i, p2.get<custom_ctor>().i);
    expect_eq(0, p2.get<default_ctor>().i);
    expect_eq(0, p2.get<trivial_ctor>().i);
};

}}} // boost::di::core

