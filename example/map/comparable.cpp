/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair/instance.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


int main() {
    //! [main]
    assert(
        map(
            pair(char_<'a'>, std::string{"foobar"}),
            pair(int_<2>, nullptr)
        )
        ==
        map(
            pair(int_<2>, (void*)0),
            pair(char_<'a'>, "foobar")
        )
    );
    //! [main]
}