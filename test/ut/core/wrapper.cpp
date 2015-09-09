//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/core/wrapper.hpp"
#include "common/fakes/fake_wrapper.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

test successful_wrapper = [] {
    static_expect(std::is_same<int, successful::wrapper<std::false_type, int, fake_wrapper>::element_type>::value);
    expect(0 == static_cast<int>(wrapper<std::false_type, int, fake_wrapper>{}));
};

}}}} // boost::di::v1::core

