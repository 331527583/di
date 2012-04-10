//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Aux/Value.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

BOOST_AUTO_TEST_CASE(Generic)
{
    EXPECT_FALSE((Value<int>::value));
}

BOOST_AUTO_TEST_CASE(mpl_int)
{
    const int i = 42;
    EXPECT_TRUE((Value<int_<0> >::value));
    EXPECT_EQ(i, Value<int_<i> >::create());
}

BOOST_AUTO_TEST_CASE(mpl_string)
{
    const std::string s = "s";
    EXPECT_TRUE((Value<string<'s'> >::value));
    EXPECT_EQ(s, Value<string<'s'> >::create());
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

