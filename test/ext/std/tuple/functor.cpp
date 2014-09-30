/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple/comparable.hpp>
#include <boost/hana/ext/std/tuple/functor.hpp>

#include <test/iso/list/functor.hpp>
using namespace boost::hana;


int main() {
    List_functor<StdTuple>();
}