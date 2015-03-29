/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/mpl/quote.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/vector.hpp>


template <typename State, typename X>
struct f { using type = X; };

struct state { };

template <int i>
struct t { };

using vector = boost::mpl::vector<
    <%= (1..input_size).to_a.map { |n| "t<#{n}>" }.join(', ') %>
>;

using result = boost::mpl::reverse_fold<
    vector, state, boost::mpl::quote2<f>
>::type;


int main() { }