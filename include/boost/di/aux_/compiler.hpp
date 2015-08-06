//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_COMPILER_HPP
#define BOOST_DI_AUX_COMPILER_HPP

#if defined(__clang__)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
    #define BOOST_DI_TYPE_WKND(T)
#elif defined(__GNUC__)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
    #define BOOST_DI_TYPE_WKND(T)
#elif defined(_MSC_VER)
    #pragma warning(disable : 4503) // decorated name length exceeded, name was truncated
    #pragma warning(disable : 4822) // local class member function does not have a body
    #define BOOST_DI_UNUSED
    #define BOOST_DI_DEPRECATED(...) __declspec(deprecated(__VA_ARGS__)) // error C2059: syntax error: '<L_ATTRIBUTE_SPECIFIER>'
    #define BOOST_DI_TYPE_WKND(T) (T&&)
#endif

#endif

