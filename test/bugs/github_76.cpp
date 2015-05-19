/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/assert.hpp>


// defined in github_76_other_tu.cpp
bool is_pointer_to_boost_hana_drop(void const* p);

int main() {
    BOOST_HANA_RUNTIME_CHECK(is_pointer_to_boost_hana_drop(&boost::hana::drop));
}
