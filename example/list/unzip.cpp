/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(
        unzip(list(list(1, '2', 3.3), list('4', 5.5, 6)))
        ==
        list(list(1, '4'), list('2', 5.5), list(3.3, 6))
    );

    BOOST_HANA_STATIC_ASSERT(
        unzip(list(list(1, '2', 3.3), list('4', 5.5, 6, "ignored")))
        ==
        list(list(1, '4'), list('2', 5.5), list(3.3, 6))
    );
    //! [main]
}