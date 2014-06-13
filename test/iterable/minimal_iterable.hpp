/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP
#define BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


template <typename T, typename Datatype>
struct with_datatype_impl {
    T actual;
    using hana_datatype = Datatype;
};

template <int i>
struct MinimalIterable;

template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto minimal_iterable = [](auto ...xs) {
    return with_datatype_impl<
        decltype(boost::hana::list(xs...)), MinimalIterable<i>
    >{boost::hana::list(xs...)};
};

BOOST_HANA_CONSTEXPR_LAMBDA auto iterable = minimal_iterable<0>;

namespace boost { namespace hana {
    template <int i>
    struct Iterable<MinimalIterable<i>>
        : defaults<Iterable>::template with<MinimalIterable<i>>
    {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs)
        { return head(xs.actual); }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return with_datatype_impl<
                decltype(tail(xs.actual)), MinimalIterable<i>
            >{tail(xs.actual)};
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs)
        { return is_empty(xs.actual); }
    };

    template <int i>
    constexpr bool foldable_from_iterable<MinimalIterable<i>> = true;

    template <int i>
    constexpr bool comparable_from_iterable<MinimalIterable<i>> = true;

    //! @todo
    //! This is a workaround for a Clang 3.5 bug. Clang fails to see the
    //! partial specializations of these variable templates as constant
    //! expressions when used in some contexts requiring constant expressions.
    //! Using the partial specializations in a constant expression explicitly
    //! makes it work. Remove this when it's fixed.
    static_assert(foldable_from_iterable<MinimalIterable<0>>, "");
    static_assert(comparable_from_iterable<MinimalIterable<0>>, "");
    static_assert(comparable_from_iterable<MinimalIterable<1>>, "");
}}

#endif // !BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP